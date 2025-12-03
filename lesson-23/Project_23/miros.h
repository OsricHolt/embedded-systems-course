/****************************************************************************
* MInimal Real-time Operating System (MIROS)
* version 0.23 (matching lesson 23)
*
* This software is a minimal Real-Time Operating System and was created as an
* exercise to understand the underlying structure of RTOS. Main concepts are 
* based on Miros Samek's Quantum Leap Embedded Systems course.
*
* Copyright (C) 2025 Osric Holt. All Rights Reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <https://www.gnu.org/licenses/>.
*
* Contact Information:
* https://www.linkedin.com/in/osric-holt/
*
****************************************************************************/


#ifndef __MIROS_H__
#define __MIROS_H__

/* Thread Control Block (TCB) */
typedef struct {
    void *sp; /* stack pointer */
    /* ...other attributes associated with a thread */
} OSThread;

typedef void (*OSThreadHandler)(); /* Pointer to function returning void (1-line function)*/

void OSThread_start(
    OSThread *me,
    OSThreadHandler threadHandler, /* Typedef to a pointer -> fxn no output null args*/
    void *stkSto, uint32_t stkSize); /* Stack parameters (and others as added) */
        

#endif // __MIROS_H__