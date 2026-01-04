#include <delay.h>
#define HSI_Clock 0xF42400UL


void delay(uint32_t ms) { 
    volatile uint32_t count = HSI_Clock / 1000 * ms / 7.3; //rough estimate of cycles per ms
    while(count--);
}
