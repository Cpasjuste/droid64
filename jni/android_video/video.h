/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - video.h                                                 *
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

#ifndef VIDEO_H
#define VIDEO_H

#ifndef __LINUX__
#include <windows.h>
#else
# include "../main/winlnxdefs.h"
#endif

#include "SDL.h"
#include "../main/plugin.h"

extern char         pluginName[];

extern void (*CheckInterrupts)( void );
extern char *screenDirectory;
extern char configdir[PATH_MAX];


void CaptureScreen ( char * Directory );
void ChangeWindow (void);
void CloseDLL (void);
void DllAbout ( HWND hParent );
void DllConfig ( HWND hParent );
void DllTest ( HWND hParent );
void DrawScreen (void);
void GetDllInfo ( PLUGIN_INFO * PluginInfo );
BOOL InitiateGFX (GFX_INFO Gfx_Info);
void MoveScreen (int xpos, int ypos);
void ProcessDList(void);
void ProcessRDPList(void);
void RomClosed (void);
void RomOpen (void);
void ShowCFB (void);
void UpdateScreen (void);
void ViStatusChanged (void);
void ViWidthChanged (void);
void ReadScreen (void **dest, int *width, int *height);
void SetConfigDir (char *configDir);

#endif

