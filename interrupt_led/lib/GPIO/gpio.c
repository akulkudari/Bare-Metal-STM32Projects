/*
 * GPIO driver implementation
*
*
*
*/



#include "gpio.h"
#include <stdint.h>


void gpio_init(GPIO_TypeDef *port, uint8_t pin, uint8_t mode){
    port->MODER &= ~(0x3 << (pin * 2));
    port->MODER |= (mode << (pin * 2));
    port->OTYPER &= ~(1 << pin);             // Push-pull
    port->PUPDR &= ~(0x3 << (pin*2));        // No pull-up/down
}

void gpio_write(GPIO_TypeDef *port, uint8_t pin, uint8_t value){
    if(~value){
        port->BSRR |= 1 << (pin);
    }
    else{
        port->BSRR |= 1 << (pin + 0x10);
    }
}

uint8_t gpio_read(GPIO_TypeDef *port, uint8_t pin){
    return (port->IDR >> pin) & 0x1; 
}

void en_GPIO_clk(GPIO_TypeDef *port){
    //enables clock for GPIO at specified port
    if(port == GPIOA){
        RCC_AHB1ENR |= (1 << 0);
    }
    else if(port == GPIOB){
        RCC_AHB1ENR |= (1 << 1);
    }
    else if(port == GPIOC){
        RCC_AHB1ENR |= (1 << 2);
    }
    else{
        return; //invalid port
    }
}



void init_pin(GPIO_TypeDef *port, uint8_t pin, uint8_t mode) {
    en_GPIO_clk(port); //enable port specific GPIO clock
    gpio_init(port, pin, mode); //set pin to mode
}

/*

*/

// void delay(volatile uint32_t count) { while(count--); }


