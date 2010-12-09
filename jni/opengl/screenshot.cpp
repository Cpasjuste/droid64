/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - screenshot.c                                            *
 *   Mupen64Plus homepage: http://code.google.com/p/mupen64plus/           *
 *   Copyright (C) 2008 Richard42                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>

#include <SDL_opengl.h>
#include <SDL.h>
#include <png.h>

#include "osd.h"

extern "C" {
#include "../main/main.h"
#include "../main/util.h"
#include "../main/rom.h"
#include "../main/translate.h"
}

static char  l_SshotDir[PATH_MAX] = {0}; // pointer to screenshot dir specified at commandline

/*********************************************************************************************************
* PNG support functions for writing screenshot files
*/

static void mupen_png_error(png_structp png_write, const char *message)
{
    printf("PNG Error: %s\n", message);
}

static void mupen_png_warn(png_structp png_write, const char *message)
{
    printf("PNG Warning: %s\n", message);
}

static int SaveRGBBufferToFile(char *filename, unsigned char *buf, int width, int height, int pitch)
{
    int i;

    // allocate PNG structures
    png_structp png_write = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, mupen_png_error, mupen_png_warn);
    if (!png_write)
    {
        printf("Error creating PNG write struct.\n");
        return 1;
    }
    png_infop png_info = png_create_info_struct(png_write);
    if (!png_info)
    {
        png_destroy_write_struct(&png_write, (png_infopp)NULL);
        printf("Error creating PNG info struct.\n");
        return 2;
    }
    // Set the jumpback
    if (setjmp(png_jmpbuf(png_write)))
    {
        png_destroy_write_struct(&png_write, &png_info);
        printf("Error calling setjmp()\n");
        return 3;
    }
    // open the file to write
    FILE *savefile = fopen(filename, "wb");
    if (savefile == NULL)
    {
        printf("Error opening '%s' to save screenshot.\n", filename);
        return 4;
    }
    // give the file handle to the PNG compressor
    png_init_io(png_write, savefile);
    // set the info
    png_set_IHDR(png_write, png_info, width, height, 8, PNG_COLOR_TYPE_RGB,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    // allocate row pointers and scale each row to 24-bit color
    png_byte **row_pointers;
    row_pointers = (png_byte **) malloc(height * sizeof(png_bytep));
    for (i = 0; i < height; i++)
    {
        row_pointers[i] = (png_byte *) (buf + (height - 1 - i) * pitch);
    }
    // set the row pointers
    png_set_rows(png_write, png_info, row_pointers);
    // write the picture to disk
    png_write_png(png_write, png_info, 0, NULL);
    // free memory
    free(row_pointers);
    png_destroy_write_struct(&png_write, &png_info);
    // close file
    fclose(savefile);
    // all done
    return 0;
}

/*********************************************************************************************************
* Global screenshot functions
*/
extern "C" void SetScreenshotDir(const char *chDir)
{
    // copy directory path
    strncpy(l_SshotDir, chDir, PATH_MAX - 1);
    l_SshotDir[PATH_MAX - 1] = '\0';

    // if path is empty, then return
    if (l_SshotDir[0] == '\0')
        return;

    // make sure screenshots dir has a '/' on the end.
    if (l_SshotDir[strlen(l_SshotDir)-1] != '/')
        strcat(l_SshotDir, "/");

    // check for valid directory
    if (!isdir(l_SshotDir))
    {
        printf("Warning: Could not find screenshot dir: %s\n", l_SshotDir);
        l_SshotDir[0] = '\0';
    }

    return;
}

extern "C" int ValidScreenshotDir(void)
{
    if (l_SshotDir[0] != 0)
        return 1;
    else
        return 0;
}

extern "C" void TakeScreenshot(int iFrameNumber)
{
    // start by getting the base file path
    char filepath[PATH_MAX], filename[PATH_MAX];
    char *pch, ch;
    filepath[0] = 0;
    filename[0] = 0;
    strncpy(filepath, l_SshotDir, sizeof(filepath));
    filepath[PATH_MAX-1] = '\0';
    if (strlen(filepath) > 0 && filepath[strlen(filepath)-1] != '/')
        strcat(filepath, "/");

    // add the game's name to the end, convert to lowercase, convert spaces to underscores
    pch = filepath + strlen(filepath);
    strncpy(pch, (char*) ROM_HEADER->nom, sizeof(ROM_HEADER->nom));
    pch[20] = '\0';
    do
    {
        ch = *pch;
        if (ch == ' ')
            *pch++ = '_';
        else
            *pch++ = tolower(ch);
    } while (ch != 0);

    // look for a file
    int i;
    for (i = 0; i < 100; i++)
    {
        sprintf(filename, "%s-%03i.png", filepath, i);
        FILE *pFile = fopen(filename, "r");
        if (pFile == NULL)
            break;
        fclose(pFile);
    }
    if (i == 100) return;

    // get the SDL surface and find the width and height
    SDL_Surface *pSurf = SDL_GetVideoSurface();
    int width = pSurf->w;
    int height = pSurf->h;

    // allocate memory for the image
    unsigned char *pucFrame = (unsigned char *) malloc(width * height * 3);
    if (pucFrame == 0)
        return;

    // grab the back image from OpenGL
    GLint oldMode;
    glGetIntegerv(GL_READ_BUFFER, &oldMode);
    glReadBuffer(GL_BACK);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pucFrame);
    glReadBuffer(oldMode);

    // write the image to a PNG
    SaveRGBBufferToFile(filename, pucFrame, width, height, width * 3);
    // free the memory
    free(pucFrame);
    // print message -- this allows developers to capture frames and use them in the regression test
    main_message(1, 1, 1, OSD_BOTTOM_LEFT, tr("Captured screenshot for frame %i.\n"), iFrameNumber);
}

