#ifndef __BSP_H__
#define __BSP_H__

#define SYS_CLOCK_HZ 16000000U

#define LED_RED (1U << 1)
#define LED_BLUE (1U << 2)
#define LED_GREEN (1U << 3)
#define GPIOF_AHB_BITS ((unsigned long volatile *) GPIOF_AHB) // Casting GPIOF_AHB_BITS to a pointer to the address defined by the macro, GPIOF_AHB


void assert_failed (char const *file, int line);

void SysTick_Handler(void);

#endif // __BSP_H__

