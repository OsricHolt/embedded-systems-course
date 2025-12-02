/****************************************************************************
* MInimal Real-time Operating System (MIROS)
* version 0.23 (matching lesson 23)
*
* This software is a teaching aid to illustrate the concepts underlying
* a Real-Time Operating System (RTOS). The main goal of the software is
* simplicity and clear presentation of the concepts, but without dealing
* with various corner cases, portability, or error handling. For these
* reasons, the software is generally NOT intended or recommended for use
* in commercial applications.
*
* Copyright (C) 2018 Miro Samek. All Rights Reserved.
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
* https://www.state-machine.com
*
* Editted by Osric Holt
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
    OSThreadHandler theradHandler, /* Typedef*/
    void *stkSto, uint32_t stkSize); /* Stack parameters (and others as added) */
        

#endif // __MIROS_H__