#include <stdint.h> // C99 standard integers
#include "bsp.h"

uint32_t stack_blinky1[40];
uint32_t *sp_blinky1 = &stack_blinky1[40]; // One beyond end since stack grows down (40 instead of 39)

int main_blinky1() {
    while (1) {
        BSP_ledGreenOn();
        BSP_delay(BSP_TICKS_PER_SEC / 4U);
        BSP_ledGreenOff();
        BSP_delay(BSP_TICKS_PER_SEC * 3U / 4U);
    }
}

uint32_t stack_blinky2[40];
uint32_t *sp_blinky2 = &stack_blinky2[40]; // One beyond end since stack grows down (40 instead of 39)

int main_blinky2() {
    while (1) {
        BSP_ledBlueOn();
        BSP_delay(BSP_TICKS_PER_SEC / 2U);
        BSP_ledBlueOff();
        BSP_delay(BSP_TICKS_PER_SEC / 3U);

    }
}

/* background code: sequential with blocking version */
int main(void) {
    uint32_t volatile run = 0U;
    
    BSP_init();
    
    /* Fabricate Cortex-M ISR stack frame for blinky1 (tricking the CPU to think we're returning from an interrupt*/
    /* Use datasheet for frame*/
    
    // decrement pointer to first unused free entry since SP points to last used entry
    *(--sp_blinky1) = (1U << 24); /* xPSR */
    *(--sp_blinky1) = (uint32_t)&main_blinky1; /* PC */
    *(--sp_blinky1) = 0x0000000EU; /* LR */
    *(--sp_blinky1) = 0x0000000CU; /* R12 */
    *(--sp_blinky1) = 0x00000003U; /* R3 */
    *(--sp_blinky1) = 0x00000002U; /* R2 */
    *(--sp_blinky1) = 0x00000001U; /* R1 */
    *(--sp_blinky1) = 0x00000000U; /* R0 */
    /* additionally fake registers R4-R11*/
    *(--sp_blinky1) = 0x0000000BU; /* R11 */
    *(--sp_blinky1) = 0x0000000AU; /* R10 */
    *(--sp_blinky1) = 0x00000009U; /* R9 */
    *(--sp_blinky1) = 0x00000008U; /* R8 */
    *(--sp_blinky1) = 0x00000007U; /* R7 */
    *(--sp_blinky1) = 0x00000006U; /* R6 */
    *(--sp_blinky1) = 0x00000005U; /* R5 */
    *(--sp_blinky1) = 0x00000004U; /* R4 */

    /* Fabricate Cortex-M ISR stack frame for blinky2 (tricking the CPU to think we're returning from an interrupt*/
    /* Use datasheet for frame*/
    
    *(--sp_blinky2) = (1U << 24); /* xPSR */
    *(--sp_blinky2) = (uint32_t)&main_blinky2; /* PC */
    *(--sp_blinky2) = 0x0000000EU; /* LR */
    *(--sp_blinky2) = 0x0000000CU; /* R12 */
    *(--sp_blinky2) = 0x00000003U; /* R3 */
    *(--sp_blinky2) = 0x00000002U; /* R2 */
    *(--sp_blinky2) = 0x00000001U; /* R1 */
    *(--sp_blinky2) = 0x00000000U; /* R0 */
    /* additionally fake registers R4-R11*/
    *(--sp_blinky1) = 0x0000000BU; /* R11 */
    *(--sp_blinky1) = 0x0000000AU; /* R10 */
    *(--sp_blinky1) = 0x00000009U; /* R9 */
    *(--sp_blinky1) = 0x00000008U; /* R8 */
    *(--sp_blinky1) = 0x00000007U; /* R7 */
    *(--sp_blinky1) = 0x00000006U; /* R6 */
    *(--sp_blinky1) = 0x00000005U; /* R5 */
    *(--sp_blinky1) = 0x00000004U; /* R4 */

    
    while(1) {
    }
    
    //return 0;
}

// Notes: We context switch by hand
// 1. use our fake stack pointer and paste it into SP to enter thread 1
// 2. We save our new stack pointer and R4-R11 into stack 1 and then enter thread 2
// 3. Repeat step 2 for stack 2 and then paste stack pointer 1 back into SP to resume thread 1 where we left off
// Repeat steps 2 and 3