/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - debugger.c                                              *
 *   Mupen64Plus homepage: http://code.google.com/p/mupen64plus/           *
 *   Copyright (C) 2008 DarkJeztr                                          *
 *   Copyright (C) 2002 davFr                                              *
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

//#include <glib.h>
#include "debugger.h"

// State of the Emulation Thread:
// 0 -> pause, 2 -> run.

int g_DebuggerEnabled = 0;    // whether the debugger is enabled or not
int debugger_mode;
int run;

SDL_cond  *debugger_done_cond;
SDL_mutex *mutex;

uint32 previousPC;

//]=-=-=-=-=-=-=-=-=-=-=[ Initialisation du Debugger ]=-=-=-=-=-=-=-=-=-=-=-=[

void init_debugger()
{
    debugger_mode = 1;
    run = 0;

    init_debugger_frontend();

    init_host_disassembler();

    mutex = SDL_CreateMutex();
    debugger_done_cond = SDL_CreateCond();
}

void destroy_debugger()
{
    SDL_DestroyMutex(mutex);
    mutex = NULL;
    SDL_DestroyCond(debugger_done_cond);
    debugger_done_cond = NULL;
    debugger_mode = 0;
}

//]=-=-=-=-=-=-=-=-=-=-=-=-=[ Mise-a-Jour Debugger ]=-=-=-=-=-=-=-=-=-=-=-=-=[

void update_debugger(uint32 pc)
// Update debugger state and display.
// Should be called after each R4300 instruction
// Checks for breakpoint hits on PC
{
    int bpt;
    
    if(run==2) {
        bpt = check_breakpoints(pc);
        if( bpt==-1 ) {
            //previousPC = pc;
            return;
        }
        else {
            run = 0;
            switch_button_to_run();
            
            if(BPT_CHECK_FLAG(g_Breakpoints[bpt], BPT_FLAG_LOG))
                log_breakpoint(pc, BPT_FLAG_EXEC, 0);
        }
    }
    else if ( previousPC == pc ) {
        return;
    }
    if(run==0) {
        update_debugger_frontend( pc );

        // Emulation thread is blocked until a button is clicked.
        SDL_mutexP(mutex);
        SDL_CondWait(debugger_done_cond, mutex);
        SDL_mutexV(mutex);
    }

    previousPC = pc;
}

void debugger_step()
{
    SDL_CondSignal(debugger_done_cond);
}
