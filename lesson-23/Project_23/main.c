#include <stdint.h> // C99 standard integers
#include "miros.h"
#include "bsp.h"

uint32_t stack_blinky1[40];
OSThread blinky1;

void main_blinky1() {
    while (1) {
        BSP_ledGreenOn();
        BSP_delay(BSP_TICKS_PER_SEC / 4U);
        BSP_ledGreenOff();
        BSP_delay(BSP_TICKS_PER_SEC * 3U / 4U);
    }
}

uint32_t stack_blinky2[40];
OSThread blinky2;

void main_blinky2() {
    while (1) {
        BSP_ledBlueOn();
        BSP_delay(BSP_TICKS_PER_SEC / 2U);
        BSP_ledBlueOff();
        BSP_delay(BSP_TICKS_PER_SEC / 3U);

    }
}

/* background code: sequential with blocking version */
int main(void) {
    BSP_init();
    OS_Init();
    
    /* Fabricate Cortex-M ISR stack frame for blinky1 (tricking the CPU to think we're returning from an interrupt*/
    OSThread_start(&blinky1,
                   &main_blinky1,
                   stack_blinky1, sizeof(stack_blinky1));
    /* Fabricate Cortex-M ISR stack frame for blinky2 (tricking the CPU to think we're returning from an interrupt*/
    OSThread_start(&blinky2,
                   &main_blinky2,
                   stack_blinky2, sizeof(stack_blinky2));

    
    while(1) {
    }
    
    //return 0;
}

// Notes: We context switch by hand
// 1. use our fake stack pointer and paste it into SP to enter thread 1
// 2. We save our new stack pointer and R4-R11 into stack 1 and then enter thread 2
// 3. Repeat step 2 for stack 2 and then paste stack pointer 1 back into SP to resume thread 1 where we left off
// Repeat steps 2 and 3