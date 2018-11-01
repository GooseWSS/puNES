/*
 *  Copyright (C) 2010-2019 Fabio Cavallo (aka FHorse)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#if defined (__unix__)
#include <pthread.h>
#endif
#include <stdio.h>
#include <string.h>
#include "emu.h"
#include "info.h"
#include "gui.h"

enum emu_thread_states {
	ET_UNINITIALIZED,
	ET_RUN,
	ET_PAUSE
};

#if defined (__unix__)
static void *emu_thread_loop(void *arg);
#elif defined (__WIN32__)
static DWORD WINAPI emu_thread_loop(void *arg);
#endif

struct _emu_thread {
#if defined (__unix__)
	pthread_t thread;
	pthread_mutex_t lock;
#elif defined (__WIN32__)
	HANDLE thread;
	HANDLE lock;
#endif
	BYTE in_run;
	BYTE action;
} emu_thread;

BYTE emu_thread_init(void) {
	memset(&emu_thread, 0x00, sizeof(emu_thread));

#if defined (__unix__)
	emu_thread.action = ET_PAUSE;

	if (pthread_mutex_init(&emu_thread.lock, NULL) != 0) {
		fprintf(stderr, "Unable to allocate the emu mutex\n");
		return (EXIT_ERROR);
	}
	pthread_create(&emu_thread.thread, NULL, emu_thread_loop, NULL);
#elif defined (__WIN32__)
	if ((emu_thread.lock = CreateSemaphore(NULL, 1, 2, NULL)) == NULL) {
		fprintf(stderr, "Unable to allocate the emu mutex\n");
		return (EXIT_ERROR);
	}
	emu_thread.thread = CreateThread(NULL, 0, emu_thread_loop, NULL, 0, 0);
#endif
	return (EXIT_OK);
}
void emu_thread_quit(void) {
#if defined (__unix__)
	pthread_mutex_destroy(&emu_thread.lock);
#elif defined (__WIN32__)
	CloseHandle(emu_thread.lock);
	CloseHandle(emu_thread.thread);
#endif
}

void emu_thread_pause(void) {
	if (emu_thread.action != ET_UNINITIALIZED) {
		emu_thread.action = ET_PAUSE;

		while (emu_thread.in_run == TRUE) {
			gui_sleep(1);
		}
	}
}
void emu_thread_continue(void) {
	if (emu_thread.action != ET_UNINITIALIZED) {
		emu_thread.action = ET_RUN;

		while (emu_thread.in_run == FALSE) {
			gui_sleep(1);
		}
	}
}

#if defined (__unix__)
static void *emu_thread_loop(void *arg) {
#elif defined (__WIN32__)
static DWORD WINAPI emu_thread_loop(void *arg) {
#endif
	// gestione uscita
	while (TRUE) {
		if (info.stop == TRUE) {
			emu_thread.in_run = FALSE;
			break;
		} else if (emu_thread.action == ET_PAUSE) {
			emu_thread.in_run = FALSE;
			gui_sleep(1);
			continue;
		}
		emu_thread.in_run = TRUE;
		emu_frame();
	}
#if defined (__unix__)
	return (NULL);
#elif defined (__WIN32__)
	return (0);
#endif
}
