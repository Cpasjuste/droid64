/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - plugin.c                                                *
 *   Mupen64Plus homepage: http://code.google.com/p/mupen64plus/           *
 *   Copyright (C) 2002 Hacktarux                                          *
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

#include <dlfcn.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#include "plugin.h"
#include "main.h"
#include "util.h"
#include "rom.h"
#include "../memory/memory.h"
#include "../r4300/interupt.h"
#include "../r4300/r4300.h"

CONTROL Controls[4];

static char l_PluginDir[PATH_MAX] = {0};

static GFX_INFO gfx_info;
static AUDIO_INFO audio_info;
static CONTROL_INFO control_info;
static RSP_INFO rsp_info;

void (*getDllInfo)(PLUGIN_INFO *PluginInfo);
void (*dllConfig)(HWND hParent);
void (*dllTest)(HWND hParent);
void (*dllAbout)(HWND hParent);

/* dummy functions to prevent mupen from crashing if a plugin is missing */
static void dummy_void() {}
static BOOL dummy_initiateGFX(GFX_INFO Gfx_Info) { return TRUE; }
static BOOL dummy_initiateAudio(AUDIO_INFO Audio_Info) { return TRUE; }
static void dummy_initiateControllers(CONTROL_INFO Control_Info) {}
static void dummy_old_initiateControllers(HWND hMainWindow, CONTROL Controls[4]) {}
static void dummy_aiDacrateChanged(int SystemType) {}
static DWORD dummy_aiReadLength() { return 0; }
static void dummy_setSpeedFactor(int percent) {}
static const char * dummy_volumeGetString() { return NULL; }
//static void dummy_aiUpdate(BOOL Wait) {}
static void dummy_controllerCommand(int Control, BYTE * Command) {}
static void dummy_getKeys(int Control, BUTTONS *Keys) {}
static void dummy_readController(int Control, BYTE *Command) {}
static void dummy_keyDown(WPARAM wParam, LPARAM lParam) {}
static void dummy_keyUp(WPARAM wParam, LPARAM lParam) {}
static void dummy_setConfigDir(char *configDir) {}
static void dummy_setInstallDir(char *installDir) {}
static unsigned int dummy;
static DWORD dummy_doRspCycles(DWORD Cycles) { return Cycles; };
static void dummy_initiateRSP(RSP_INFO Rsp_Info, DWORD * CycleCount) {};
static void dummy_fBRead(DWORD addr) {};
static void dummy_fBWrite(DWORD addr, DWORD size) {};
static void dummy_fBGetFrameBufferInfo(void *p) {};

void (*changeWindow)() = dummy_void;
void (*closeDLL_gfx)() = dummy_void;
BOOL (*initiateGFX)(GFX_INFO Gfx_Info) = dummy_initiateGFX;
void (*processDList)() = dummy_void;
void (*processRDPList)() = dummy_void;
void (*romClosed_gfx)() = dummy_void;
void (*romOpen_gfx)() = dummy_void;
void (*showCFB)() = dummy_void;
void (*updateScreen)() = dummy_void;
void (*viStatusChanged)() = dummy_void;
void (*viWidthChanged)() = dummy_void;
void (*readScreen)(void **dest, int *width, int *height) = 0;
void (*captureScreen)(char *dirpath) = 0;
void (*setRenderingCallback)(void (*callback)()) = dummy_void;

void (*aiDacrateChanged)(int SystemType) = dummy_aiDacrateChanged;
void (*aiLenChanged)() = dummy_void;
DWORD (*aiReadLength)() = dummy_aiReadLength;
//void (*aiUpdate)(BOOL Wait) = dummy_aiUpdate;
void (*closeDLL_audio)() = dummy_void;
BOOL (*initiateAudio)(AUDIO_INFO Audio_Info) = dummy_initiateAudio;
void (*processAList)() = dummy_void;
void (*romClosed_audio)() = dummy_void;
void (*romOpen_audio)() = dummy_void;
void (*setSpeedFactor)(int percent) = dummy_setSpeedFactor;
void (*volumeUp)() = dummy_void;
void (*volumeDown)() = dummy_void;
void (*volumeMute)() = dummy_void;
const char * (*volumeGetString)() = dummy_volumeGetString;

void (*closeDLL_input)() = dummy_void;
void (*controllerCommand)(int Control, BYTE * Command) = dummy_controllerCommand;
void (*getKeys)(int Control, BUTTONS *Keys) = dummy_getKeys;
void (*old_initiateControllers)(HWND hMainWindow, CONTROL Controls[4]) = dummy_old_initiateControllers;
void (*initiateControllers)(CONTROL_INFO ControlInfo) = dummy_initiateControllers;
void (*readController)(int Control, BYTE *Command) = dummy_readController;
void (*romClosed_input)() = dummy_void;
void (*romOpen_input)() = dummy_void;
void (*keyDown)(WPARAM wParam, LPARAM lParam) = dummy_keyDown;
void (*keyUp)(WPARAM wParam, LPARAM lParam) = dummy_keyUp;
void (*setConfigDir)(char *configDir) = dummy_setConfigDir;
void (*setInstallDir)(char *installDir) = dummy_setInstallDir;

void (*closeDLL_RSP)() = dummy_void;
DWORD (*doRspCycles)(DWORD Cycles) = dummy_doRspCycles;
void (*initiateRSP)(RSP_INFO Rsp_Info, DWORD * CycleCount) = dummy_initiateRSP;
void (*romClosed_RSP)() = dummy_void;

void (*fBRead)(DWORD addr) = dummy_fBRead;
void (*fBWrite)(DWORD addr, DWORD size) = dummy_fBWrite;
void (*fBGetFrameBufferInfo)(void *p) = dummy_fBGetFrameBufferInfo;

list_t g_PluginList = NULL;

HINSTANCE g_ProgramInstance = 0;
HWND g_RenderWindow = 0;
HWND g_StatusBar = 0;

void GFX_CaptureScreen ( char * Directory );
void GFX_ChangeWindow (void);
void GFX_CloseDLL (void);
void GFX_DllAbout ( HWND hParent );
void GFX_DllConfig ( HWND hParent );
void GFX_DllTest ( HWND hParent );
void GFX_DrawScreen (void);
void GFX_GetDllInfo ( PLUGIN_INFO * PluginInfo );
BOOL GFX_InitiateGFX (GFX_INFO Gfx_Info);
void GFX_MoveScreen (int xpos, int ypos);
void GFX_ProcessDList(void);
void GFX_ProcessRDPList(void);
void GFX_RomClosed (void);
void GFX_RomOpen (void);
void GFX_ShowCFB (void);
void GFX_UpdateScreen (void);
void GFX_ViStatusChanged (void);
void GFX_ViWidthChanged (void);
void GFX_ReadScreen (void **dest, int *width, int *height);
void GFX_SetConfigDir (char *configDir);

void RSP_CloseDLL (void);
void RSP_DllAbout ( HWND hParent );
void RSP_DllConfig ( HWND hParent );
void RSP_DllTest ( HWND hParent );
DWORD RSP_DoRspCycles ( DWORD Cycles );
void RSP_GetDllInfo ( PLUGIN_INFO * PluginInfo );
void RSP_InitiateRSP ( RSP_INFO Rsp_Info, DWORD * CycleCount);
void RSP_RomClosed (void);

static void sucre(){}

void plugin_load_plugins(const char *gfx_name,
                         const char *audio_name,
                         const char *input_name,
                         const char *RSP_name)
{
   plugin_load_gfx_plugin(gfx_name);
   plugin_load_audio_plugin(audio_name);
   plugin_load_input_plugin(input_name);
   plugin_load_rsp_plugin(RSP_name);
}

void plugin_load_rsp_plugin(const char* RSP_name)
{
    closeDLL_RSP = RSP_CloseDLL;
    doRspCycles = RSP_DoRspCycles;
    initiateRSP = RSP_InitiateRSP;
    romClosed_RSP = RSP_RomClosed;
    
    if (closeDLL_RSP == NULL) closeDLL_RSP = dummy_void;
    if (doRspCycles == NULL) doRspCycles = dummy_doRspCycles;
    if (initiateRSP == NULL) initiateRSP = dummy_initiateRSP;
    if (romClosed_RSP == NULL) romClosed_RSP = dummy_void;
    
    rsp_info.MemoryBswaped = TRUE;
    rsp_info.RDRAM = (BYTE*)rdram;
    rsp_info.DMEM = (BYTE*)SP_DMEM;
    rsp_info.IMEM = (BYTE*)SP_IMEM;
    rsp_info.MI_INTR_REG = &MI_register.mi_intr_reg;
    rsp_info.SP_MEM_ADDR_REG = &sp_register.sp_mem_addr_reg;
    rsp_info.SP_DRAM_ADDR_REG = &sp_register.sp_dram_addr_reg;
    rsp_info.SP_RD_LEN_REG = &sp_register.sp_rd_len_reg;
    rsp_info.SP_WR_LEN_REG = &sp_register.sp_wr_len_reg;
    rsp_info.SP_STATUS_REG = &sp_register.sp_status_reg;
    rsp_info.SP_DMA_FULL_REG = &sp_register.sp_dma_full_reg;
    rsp_info.SP_DMA_BUSY_REG = &sp_register.sp_dma_busy_reg;
    rsp_info.SP_PC_REG = &rsp_register.rsp_pc;
    rsp_info.SP_SEMAPHORE_REG = &sp_register.sp_semaphore_reg;
    rsp_info.DPC_START_REG = &dpc_register.dpc_start;
    rsp_info.DPC_END_REG = &dpc_register.dpc_end;
    rsp_info.DPC_CURRENT_REG = &dpc_register.dpc_current;
    rsp_info.DPC_STATUS_REG = &dpc_register.dpc_status;
    rsp_info.DPC_CLOCK_REG = &dpc_register.dpc_clock;
    rsp_info.DPC_BUFBUSY_REG = &dpc_register.dpc_bufbusy;
    rsp_info.DPC_PIPEBUSY_REG = &dpc_register.dpc_pipebusy;
    rsp_info.DPC_TMEM_REG = &dpc_register.dpc_tmem;
    rsp_info.CheckInterrupts = sucre;
    rsp_info.ProcessDlistList = processDList;
    rsp_info.ProcessAlistList = processAList;
    rsp_info.ProcessRdpList = processRDPList;
    rsp_info.ShowCFB = showCFB;
    rsp_info.hInst = g_ProgramInstance;
    initiateRSP(rsp_info,(DWORD*) NULL);
}

void plugin_load_input_plugin(const char* input_name)
{
    closeDLL_input = dummy_void;
    controllerCommand = dummy_controllerCommand;
    getKeys = dummy_getKeys;
    initiateControllers = dummy_initiateControllers;
    readController = dummy_readController;
    romClosed_input = dummy_void;
    romOpen_input = dummy_void;
    keyDown = dummy_keyDown;
    keyUp = dummy_keyUp;
}

void plugin_load_audio_plugin(const char* audio_name)
{
    aiDacrateChanged = dummy_aiDacrateChanged;
    aiLenChanged = dummy_void;
    aiReadLength = dummy_aiReadLength;
    //aiUpdate = dummy_aiUpdate;
    closeDLL_audio = dummy_void;
    initiateAudio = dummy_initiateAudio;
    processAList = dummy_void;
    romClosed_audio = dummy_void;
    romOpen_audio = dummy_void;
    setSpeedFactor = dummy_setSpeedFactor;
}

void plugin_load_gfx_plugin(const char* gfx_name)
{
    changeWindow = GFX_ChangeWindow;
    closeDLL_gfx = GFX_CloseDLL;
    dllAbout = GFX_DllAbout;
    dllConfig = GFX_DllConfig;
    dllTest = GFX_DllTest;
    initiateGFX = GFX_InitiateGFX;
    processDList = GFX_ProcessDList;
    processRDPList = GFX_ProcessRDPList;
    romClosed_gfx = GFX_RomClosed;
    romOpen_gfx = GFX_RomOpen;
    showCFB = GFX_ShowCFB;
    updateScreen = GFX_UpdateScreen;
    viStatusChanged = GFX_ViStatusChanged;
    viWidthChanged = GFX_ViWidthChanged;
    readScreen = GFX_ReadScreen;
    captureScreen = GFX_CaptureScreen;

    setRenderingCallback = dummy_void;

    gfx_info.MemoryBswaped = TRUE;
    gfx_info.HEADER = rom;
    gfx_info.RDRAM = (BYTE*)rdram;
    gfx_info.DMEM = (BYTE*)SP_DMEM;
    gfx_info.IMEM = (BYTE*)SP_IMEM;
    gfx_info.MI_INTR_REG = &(MI_register.mi_intr_reg);
    gfx_info.DPC_START_REG = &(dpc_register.dpc_start);
    gfx_info.DPC_END_REG = &(dpc_register.dpc_end);
    gfx_info.DPC_CURRENT_REG = &(dpc_register.dpc_current);
    gfx_info.DPC_STATUS_REG = &(dpc_register.dpc_status);
    gfx_info.DPC_CLOCK_REG = &(dpc_register.dpc_clock);
    gfx_info.DPC_BUFBUSY_REG = &(dpc_register.dpc_bufbusy);
    gfx_info.DPC_PIPEBUSY_REG = &(dpc_register.dpc_pipebusy);
    gfx_info.DPC_TMEM_REG = &(dpc_register.dpc_tmem);
    gfx_info.VI_STATUS_REG = &(vi_register.vi_status);
    gfx_info.VI_ORIGIN_REG = &(vi_register.vi_origin);
    gfx_info.VI_WIDTH_REG = &(vi_register.vi_width);
    gfx_info.VI_INTR_REG = &(vi_register.vi_v_intr);
    gfx_info.VI_V_CURRENT_LINE_REG = &(vi_register.vi_current);
    gfx_info.VI_TIMING_REG = &(vi_register.vi_burst);
    gfx_info.VI_V_SYNC_REG = &(vi_register.vi_v_sync);
    gfx_info.VI_H_SYNC_REG = &(vi_register.vi_h_sync);
    gfx_info.VI_LEAP_REG = &(vi_register.vi_leap);
    gfx_info.VI_H_START_REG = &(vi_register.vi_h_start);
    gfx_info.VI_V_START_REG = &(vi_register.vi_v_start);
    gfx_info.VI_V_BURST_REG = &(vi_register.vi_v_burst);
    gfx_info.VI_X_SCALE_REG = &(vi_register.vi_x_scale);
    gfx_info.VI_Y_SCALE_REG = &(vi_register.vi_y_scale);
    gfx_info.CheckInterrupts = sucre;
    gfx_info.hWnd = g_RenderWindow;
    gfx_info.hStatusBar = g_StatusBar;
    initiateGFX(gfx_info);
}

