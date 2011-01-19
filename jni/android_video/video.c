/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - video.cpp                                               *
 *   Mupen64Plus homepage: http://code.google.com/p/mupen64plus/           *
 *   Copyright (C) 2008 John Chadwick (NMN)                                *
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

#include <limits.h>
#include <string.h>
#include <stdio.h>
#include "Graphics_1.3.h"

char pluginName[] = "Android No Video";
char *screenDirectory;
unsigned int last_good_ucode = -1;
void (*CheckInterrupts)( void );
char configdir[PATH_MAX] = {0};


void GFX_CaptureScreen ( char * Directory )
{

}

void GFX_ChangeWindow (void)
{

}

void GFX_CloseDLL (void)
{
}

void GFX_DllAbout ( HWND hParent )
{

}

void GFX_DllConfig ( HWND hParent )
{

}

void GFX_DllTest ( HWND hParent )
{
}

void GFX_DrawScreen (void)
{
}

void GFX_GetDllInfo ( PLUGIN_INFO * PluginInfo )
{
    PluginInfo->Version = 0x103;
    PluginInfo->Type = PLUGIN_TYPE_GFX;
    strcpy( PluginInfo->Name, pluginName );
    PluginInfo->NormalMemory = FALSE;
    PluginInfo->MemoryBswaped = TRUE;
}


BOOL GFX_InitiateGFX (GFX_INFO Gfx_Info)
{
    return TRUE;
}

void GFX_MoveScreen (int xpos, int ypos)
{
}

void GFX_ProcessDList(void)
{

}

void GFX_ProcessRDPList(void)
{

}

void GFX_RomClosed (void)
{

}

void GFX_RomOpen (void)
{
	printf( "GFX: RomOpen()\n" );
}

void GFX_ShowCFB (void)
{
//	printf( "GFX: GFX_ShowCFB()\n" );
}

void GFX_UpdateScreen (void)
{
//	printf( "GFX: GFX_UpdateScreen()\n" );
}

void GFX_ViStatusChanged (void)
{

}

void GFX_ViWidthChanged (void)
{

}


void GFX_ReadScreen (void **dest, int *width, int *height)
{

}

void GFX_SetConfigDir (char *configDir)
{

}

