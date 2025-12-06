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
#include "TM4C123GH6PM.h" /* the TM4C MCU Peripheral Access Layer (TI) */


/* pointer is after * since we want the pointer to be volatile */
OSThread * volatile OS_curr; /* pointer to the current thread */
OSThread * volatile OS_next; /* pointer to the next thread to run */

void OS_Init(void){
    /* sent the PENDSV interrupt priority to the lowest level (last interrupt) */
    *(uint32_t volatile *) 0xE000ED20 |= (0xFFU << 16);
}

void OS_sched(void){
    /* OS_next = ... */
    if (OS_next != OS_curr) {
        *(uint32_t volatile *) 0xE000ED04 |= (0x1U << 28); /* triggers PendSV */
    }
}

 void OSThread_start(
    OSThread *me,
    OSThreadHandler threadHandler, /* Typedef to a pointer -> fxn no output null args*/
    void *stkSto, uint32_t stkSize){ /* Stack parameters (and others as added) */
        
    /* start from the end of stack 
    *  div by 8 then mult by 8 aligns addresses since we are doing pointer arithmetic
    *  Clearing last 3 bits means we are a multiple of 8 and hence counting by 8's 
    */
    uint32_t *sp = (uint32_t *)((((uint32_t)stkSto + stkSize) / 8) * 8); 
    uint32_t *stk_limit;
    
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
    
    /* save the top of the stack in the thread's attribute */
    me->sp = sp;
    
    /* round up the bottom of the stack to the 8-byte boundary */
    /* Case 1: already aligned. 4th bit decreases by 1, then shift right 3
    *  discarding junk. Then add the value back and realign. Case 2: reduce
    *  junk by 1 (does nothing) then shift right 3. Add 1 (rounding up) and
    *  then shift back to 8 byte alignment
    */
    stk_limit = (uint32_t *)(((((uint32_t)stkSto - 1U) / 8) + 1U) * 8);
    
    /* pre-fill the unused part of the stack with 0xDEADBEEF */
    /* since sp is a pointer, this is pointer arithemetic. Then
    *  dereference pointer to write 0xDEADBEEF. Continue until
    *  reaching stk_limit.
    */
    for (sp = sp - 1U; sp >= stk_limit; --sp) {
        *sp = 0xDEADBEEF;
    }

}

__ASM;
void PendSV_Handler(void) {
    IMPORT OS_curr
    IMPORT OS_next
    
    /*     __disable_irq(); */
    CPSID         I
    /* if (OS_curr != (OSThread *) 0) { */
    LDR           r0,=OS_curr
    LDR           r0,[r0,#0x00]
    CBZ           r0,PendSV_restore
    /* push registers R4-R11 on the stack */ 
    PUSH          {r4-r11}
    /* OS_curr->sp = sp; // this is SP */ 
    LDR           r1,=OS_curr
    LDR           r1,[r1,#0x00]
    STR           sp,[r1,#0x00]
   108:     } /* saves context of current thread */ 
PendSV_restore
    /* sp = OS_next->sp; */
    LDR           r1,=OS_next
    LDR           r1,[r1,#0x00]
    STR           sp,[r1,#0x00]
    /* OS_curr = OS_next; */
   113:  
    LDR           r0,=OS_next
    LDR           r1,[r1,#0x00] 
    LDR           r1,=OS_curr
    STR           r0,[r1,#0x00] /* I think this is backwards */
    /* pop registers R4-R11 */ 
    POP           {r4-r11}
    /* __enable_irq(); */
    CPSIE         I
    B             lr
}

