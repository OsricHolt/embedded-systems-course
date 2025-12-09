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
    
//    /* start blinky3 thread*/
//    OSThread_start(&blinky3,
//                   &main_blinky3,
//                   stack_blinky3, sizeof(stack_blinky3));
    /* transfer control to the RTOS  to run the threads */
    OS_run();
    
    //return 0;
}

// Notes: Measurements
// 1. Last activity of thread -> SysTick Begin = 375ns; .375us * 50 ticks*MHz = 20 ticks
// 2. SysTick duration = 4.25 us; 4.25us * 50 ticks*MHz = 212 ticks
// 3. Context Switching Time = 5.2us; 5.2us * 50 ticks*MHz = 260 ticks
// 4. Longest time outside idle Thread = 18us; 18us * 50 ticks*MHz = 900 ticks
// 5. RTOS overhead = 5.2us / 1e6us * 100 ticks = .00052 = 0.052% of program