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

#include <stdint.h>
#include "miros.h"
 
 void OSThread_start(
    OSThread *me,
    OSThreadHandler threadHandler, /* Typedef to a pointer -> fxn no output null args*/
    void *stkSto, uint32_t stkSize){ /* Stack parameters (and others as added) */
        
    uint32_t *sp = (uint32_t *)((((uint32_t)stkSto + stkSize) / 8) * 8); /* start from the end of stack */
    // div by 8 then mult by 8 aligns addresses since we are doing pointer arithmetic
    // Clearing last 3 bits means we are a multiple of 8 and hence counting by 8's
    *(--sp) = (1U << 24); /* xPSR */
    *(--sp) = (uint32_t)&threadHandler; /* PC */
    *(--sp) = 0x0000000EU; /* LR */
    *(--sp) = 0x0000000CU; /* R12 */
    *(--sp) = 0x00000003U; /* R3 */
    *(--sp) = 0x00000002U; /* R2 */
    *(--sp) = 0x00000001U; /* R1 */
    *(--sp) = 0x00000000U; /* R0 */
    /* additionally fake registers R4-R11*/
    *(--sp) = 0x0000000BU; /* R11 */
    *(--sp) = 0x0000000AU; /* R10 */
    *(--sp) = 0x00000009U; /* R9 */
    *(--sp) = 0x00000008U; /* R8 */
    *(--sp) = 0x00000007U; /* R7 */
    *(--sp) = 0x00000006U; /* R6 */
    *(--sp) = 0x00000005U; /* R5 */
    *(--sp) = 0x00000004U; /* R4 */

}

