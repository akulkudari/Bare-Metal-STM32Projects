#include <stdio.h>
#include <stdlib.h>
#include <gpio.h>
#include <delay.h>

int main(void) {
    init_pin(GPIOA, 2, 1);    
    init_pin(GPIOA, 1, 1);
    while(1) {
        gpio_write(GPIOA, 2, 1);  // Reset PC13 → LED ON (active-low)
        gpio_write(GPIOA, 1, 0);   // Reset PA1 → Set low
        delay(1000);
        gpio_write(GPIOA, 2, 0);         // Set PC13 → LED OFF
        gpio_write(GPIOA, 1, 1);         // Set PA1 → Set high
        delay(1000);
    }
}
