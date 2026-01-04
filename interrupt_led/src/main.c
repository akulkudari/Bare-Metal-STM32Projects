#include <stdio.h>
#include <stdlib.h>
#include <gpio.h>
/*

int main(void){
    gpio_init(GPIOB, 4, 1);
    gpio_init(GPIOB, 5, 1);
    gpio_init(GPIOB, 6, 1);
    gpio_init(GPIOB, 7, 1);
    en_GPIO_clk('B');
    while(1){
        gpio_write(GPIOB, 4, 1);
        for(volatile int i = 0; i < 1000000; i++);
        gpio_write(GPIOB, 4, 0);
        for(volatile int i = 0; i < 1000000; i++);  
        gpio_write(GPIOB, 5, 1);
        for(volatile int i = 0; i < 1000000; i++);
        gpio_write(GPIOB, 5, 0);
        for(volatile int i = 0; i < 1000000; i++);  
        gpio_write(GPIOB, 6, 1);
        for(volatile int i = 0; i < 1000000; i++);
        gpio_write(GPIOB, 6, 0);
        for(volatile int i = 0; i < 1000000; i++);  
        gpio_write(GPIOB, 7, 1);
        for(volatile int i = 0; i < 1000000; i++);
        gpio_write(GPIOB, 7, 0);
        for(volatile int i = 0; i < 1000000; i++);
    }
    return 0;
}
*/

/*
#include <stdint.h>



void init_PC13(void) {
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOCEN;     // Enable GPIOC clock
    GPIOC->MODER &= ~(0x3 << (13*2));       // Clear mode
    GPIOC->MODER |=  (0x1 << (13*2));       // Set output
    GPIOC->OTYPER &= ~(1 << 13);            // Push-pull
    GPIOC->PUPDR &= ~(0x3 << (13*2));       // No pull-up/down
}

void init_PA1(void){
    RCC_AHB1ENR |= (1 << 0);                // Enable GPIOA clock
    GPIOA->MODER &= ~(0x3 << (1*2));        // Clear mode
    GPIOA->MODER |=  (0x1 << (1*2));        // Set output
    GPIOA->OTYPER &= ~(1 << 1);             // Push-pull
    GPIOA->PUPDR &= ~(0x3 << (1*2));        // No pull-up/down
}

void LED_on(void) {
    GPIOC->BSRR = (1 << (13 + 16));  // Reset PC13 → LED ON (active-low)
    GPIOA->BSRR = (1 << (1 + 16));   // Reset PA1 → Set low
}

void LED_off(void) {
    GPIOC->BSRR = (1 << 13);         // Set PC13 → LED OFF
    GPIOA->BSRR = (1 << 1);          // Set PA1 → Set high
}

void delay(volatile uint32_t count) { while(count--); }


int main(void) {
    init_PC13();   // Initialize the LED pin
    init_PA1();
    while(1) {
        LED_on();
        delay(500000);
        LED_off();
        delay(500000);
    }
}
*/
void init_PA2(void) {
    en_GPIO_clk(GPIOA); // Enable GPIOA clock
    gpio_init(GPIOA, 2, 1); // Set PCA2 as output type
}

void init_PA1(void){
    en_GPIO_clk(GPIOA); // Enable GPIOA clock
    gpio_init(GPIOA, 1, 1); // Set PA1 as output type
}

void LED_on(void) {
    gpio_write(GPIOA, 2, 1);  // Reset PC13 → LED ON (active-low)
    gpio_write(GPIOA, 1, 0);   // Reset PA1 → Set low
}

void LED_off(void) {
    GPIOA->BSRR = (1 << (2 + 16));   // Set PC13 → LED OFF
    GPIOA->BSRR = (1 << 1);          // Set PA1 → Set high
}

void delay(volatile uint32_t count) { while(count--); }


int main(void) {
    init_PA2();   // Initialize the LED pin
    init_PA1();
    while(1) {
        LED_on();
        delay(500000);
        LED_off();
        delay(500000);
    }
}
