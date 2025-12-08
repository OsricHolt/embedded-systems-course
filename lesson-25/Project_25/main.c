#include <stdint.h> // C99 standard integers
#include "miros.h"
#include "bsp.h"

uint32_t stack_blinky1[40];
OSThread blinky1;

void main_blinky1() {
    while (1) {
        BSP_ledGreenOn();
        OS_delay(BSP_TICKS_PER_SEC / 4U);
        BSP_ledGreenOff();
        OS_delay(BSP_TICKS_PER_SEC * 3U / 4U);
    }
}

uint32_t stack_blinky2[40];
OSThread blinky2;

void main_blinky2() {
    while (1) {
        BSP_ledBlueOn();
        OS_delay(BSP_TICKS_PER_SEC / 2U);
        BSP_ledBlueOff();
        OS_delay(BSP_TICKS_PER_SEC / 3U);

    }
}

uint32_t stack_blinky3[40];
OSThread blinky3;

void main_blinky3() {
    while (1) {
        BSP_ledRedOn();
        OS_delay(BSP_TICKS_PER_SEC / 3U);
        BSP_ledRedOff();
        OS_delay(BSP_TICKS_PER_SEC * 3U / 5U);

    }
}

/* Idle Thread Initialization */
uint32_t stack_idleThread[40];

/* background code: sequential with blocking version */
int main(void) {
    BSP_init();
    OS_Init(stack_idleThread, sizeof(stack_idleThread));
    
    /* start blinky1 thread*/
    OSThread_start(&blinky1,
                   &main_blinky1,
                   stack_blinky1, sizeof(stack_blinky1));
    
    /* start blinky2 thread*/
    OSThread_start(&blinky2,
                   &main_blinky2,
                   stack_blinky2, sizeof(stack_blinky2));
    
    /* start blinky3 thread*/
    OSThread_start(&blinky3,
                   &main_blinky3,
                   stack_blinky3, sizeof(stack_blinky3));
    /* transfer control to the RTOS  to run the threads */
    OS_run();
    
    //return 0;
}

// Notes: We context switch by hand
// 1. use our fake stack pointer and paste it into SP to enter thread 1
// 2. We save our new stack pointer and R4-R11 into stack 1 and then enter thread 2
// 3. Repeat step 2 for stack 2 and then paste stack pointer 1 back into SP to resume thread 1 where we left off
// Repeat steps 2 and 3