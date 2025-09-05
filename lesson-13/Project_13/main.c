#include <stdint.h>
// #include "TM4C123.h"                    // Device header
#include "TM4C123GH6PM.h"
#include "delay.h"

#define LED_RED (1U << 1)
#define LED_BLUE (1U << 2)
#define LED_GREEN (1U << 3)
#define GPIOF_AHB_BITS ((unsigned long volatile *) GPIOF_AHB) // Casting GPIOF_AHB_BITS to a pointer to the address defined by the macro, GPIOF_AHB

// typedef struct Point Point;
typedef struct /*__PACKED*/ {               // __PACKED = __attribute__((packed,aligned(1)))
    uint8_t y;
    uint16_t x;
} Point;

Point p1, p2;

typedef struct {
    Point top_left;
    Point bottom_right;
} Window;

typedef struct {
    Point corners[3];
} Triangle;

Window w, w2; // variable of structure types are instances. 'w' and 't' are called instances of 'Window' and 'Triangle' structs
Triangle t;

int main() {
    Point *pp;
    Window *wp;
    
    p1.x = sizeof(Point);
    p1.y = 0xAAU;
    
    w.top_left.x = 1U;
    w.bottom_right.y = 2U;
    
    t.corners[0].x = 1U;
    t.corners[2].y = 2U;
    
    p2 = p1;
    w2 = w; // structures are big so copying structures can be expensive. Better to use pointers to structures.
    
    pp = &p1;
    wp = &w2;
    
    (*pp).x = 1U; // Dereferenceing pp gives you the values of a point. Namely, the list of x and y. Then you access x or y as if you're using p1 (since pp points to p1)
    // Must put '()' around dereferencing because accessing members (.x) has really high priority
    (*wp).top_left = *pp; // this sets thei top left point of the window structure to the point p1
    
    pp->x = 1U; //These statements are equivalent to the statements on lines 50 and 52
    wp->top_left = *pp;
    
    
    
    unsigned int flashes = 0;
    
    SYSCTL->RCGCGPIO |= (1U << 5); /* enable clock for GPIOF*/
    SYSCTL->GPIOHBCTL |= (1U << 5); /* enable AHB for GPIOF */
    GPIOF_AHB->DIR |= (LED_RED | LED_BLUE | LED_GREEN);  // set pins 1,2, and 3 as outputs
    GPIOF_AHB->DEN |= (LED_RED | LED_BLUE | LED_GREEN);

    GPIOF_AHB_BITS[LED_BLUE] = LED_BLUE;
    
    while (flashes <= 30) {
        //GPIOF->DATA |= LED_RED;
        GPIOF_AHB_BITS[LED_RED] = LED_RED;
        delay(500000);
       
        GPIOF_AHB_BITS[LED_RED] = 0;
       delay(500000);
       ++flashes;
   }
          GPIOF_AHB->DATA = 0x00;

//    return 0;
}
