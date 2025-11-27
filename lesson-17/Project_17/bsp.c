/* Board Support Package*/
#include "TM4C123GH6PM.h"
#include "bsp.h"

extern unsigned int flashes;
//Fault Handler Assertions
void assert_failed (char const *file, int line){
    /* TBD: damage control*/  
    NVIC_SystemReset();
}

void SysTick_Handler(void) {
    GPIOF_AHB_BITS[LED_RED] ^= LED_RED;
    
    ++flashes;
}
