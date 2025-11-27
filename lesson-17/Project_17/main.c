#include <stdint.h>
// #include "TM4C123.h"                    // Device header
#include "TM4C123GH6PM.h"
#include "bsp.h"

unsigned int flashes = 0;


int main() {
    
    
    SYSCTL->RCGCGPIO |= (1U << 5); /* enable clock for GPIOF*/
    SYSCTL->GPIOHBCTL |= (1U << 5); /* enable AHB for GPIOF */
    GPIOF_AHB->DIR |= (LED_RED | LED_BLUE | LED_GREEN);  // set pins 1,2, and 3 as outputs
    GPIOF_AHB->DEN |= (LED_RED | LED_BLUE | LED_GREEN);

    SysTick->LOAD = SYS_CLOCK_HZ/2U - 1U;
    SysTick->VAL  = 0U;
    SysTick->CTRL = (1U << 2) | (1U <<1) | 1U;
    
//    GPIOF_AHB_BITS[LED_BLUE] = LED_BLUE;
    
    SysTick_Handler(); // Appreciate the ability to call any interrupt in Cortex-M. On other architectures, need to manually set timer increment to 1 tick before trigger.
    
    __enable_irq();
    while (flashes <= 100) {
        GPIOF_AHB_BITS[LED_GREEN] = LED_GREEN;
        GPIOF_AHB_BITS[LED_GREEN] = 0U;

   }
          GPIOF_AHB->DATA = 0x00;

//    return 0;
}
