#include <stdint.h>
#include <stdio.h>

#define GPIOB_BASE 0x40020400UL 
/*  Each Register has a base address for accessing its internal values. 
*   We define this address here to make it easier to understand the modular code.
*/
#define RCC_BASE 0x40023800UL //RCC Base register address
/*
*   RCC is the Reset Clock Configuration. The Base Address for RCC is where it starts.
*   RCC controls many different clocks, PLL configs, peripheral control, and more.
*/

typedef struct{
    /*
    *   There are 16 'pins' per GPIO, and each of these values can hold 4 bytes of info each or 32 bits
    *   (with the exception of the Alternate Function Register(AFR)). 
    *   
    *   to access a specific pin value and set it equal to something, you can perform a
    *   left shift operation (<<)
    */
    volatile uint32_t MODER; 
    /*
    *  [R15[31:30]  |R14[29:28]  |R13[27:26]  |R12[25:24] |
    *   R11[23:22]  |R10[21:20]  |R9 [19:18]  |R8 [17:16] |
    *   R7 [15:14]  |R6 [13:12]  |R5 [11:10]  |R4 [9:8]   |
    *   R3 [7:6]    |R2 [5:4]    |R1 [3:2]    |R0 [1:0]   ]
    */
    /* 00 - Input
    *  01 - General Purpose Output 
    *  10 - Alternate Function (for I2C, SPI, UART, etc.)
    *  11 - Analog
    */
    volatile uint32_t OTYPER;
    /*
    *  [R15[15]  |R14[14]  |R13[13]     |R12[12] 
    *   R11[11]  |R10[10]  |R9 [9]      |R8 [8] 
    *   R7 [7]   |R6 [6]   |R5 [5]      |R4 [4]
    *   R3 [3]   |R2 [2]   |R1 [1]      |R0 [0]   ]
    */
    /*
    *   1 - Open Drain
    *   0 - Push Pull
    */
    volatile uint32_t OSPEEDR;
    /*
    *  [R15[31:30]  |R14[29:28]  |R13[27:26]  |R12[25:24] |
    *   R11[23:22]  |R10[21:20]  |R9 [19:18]  |R8 [17:16] |
    *   R7 [15:14]  |R6 [13:12]  |R5 [11:10]  |R4 [9:8]   |
    *   R3 [7:6]    |R2 [5:4]    |R1 [3:2]    |R0 [1:0]   ]
    */
    /* 00 - Slow Speed (good for LEDs or simple GPIO) good for around 2MHz or less.
    *  01 - Medium Speed (Good for I2C, UART, SPI Signals, etc.) maxes out at around 25 MHz
    *  10 - Fast Speed (Good for SPI signals that are in 10MHz range+) Maxes out at around 50 MHz
    *  11 - Very High Speed (Good for Camera interfaces,critical timing GPIO Toggling, can have 
    *                         larger EMI). Operates at max frequency of chip. */
    volatile uint32_t PUPDR;
    /* [R15[31:30]  |R14[29:28]  |R13[27:26]  |R12[25:24] |
    *   R11[23:22]  |R10[21:20]  |R9 [19:18]  |R8 [17:16] |
    *   R7 [15:14]  |R6 [13:12]  |R5 [11:10]  |R4 [9:8]   |
    *   R3 [7:6]    |R2 [5:4]    |R1 [3:2]    |R0 [1:0]   ]
    * 
    *   00 - Floating (High Impedance)
    *   01 - Pull-up (Source is connected to VDD)
    *   10 - Pull-down (source is connected to GND)
    *   11 - reserved */
    volatile uint32_t IDR;
    /* READ ONLY PINS | HARDWARE CONTROLLED
    * [R15[15]  |R14[14]  |R13[13]     |R12[12] 
    *   R11[11]  |R10[10]  |R9 [9]      |R8 [8] 
    *   R7 [7]   |R6 [6]   |R5 [5]      |R4 [4]
    *   R3 [3]   |R2 [2]   |R1 [1]      |R0 [0]   ]
    * 
    *  0 - Pin is set low
    *  1 - Pin is set high */
    volatile uint32_t ODR;
    /*   CAN WRITE | Setting an ODR pin value to 1 or 0 will drive that pin high IF configured as
    *   output by MODER = 01
    * 
    *  [R15[15]  |R14[14]  |R13[13]     |R12[12] 
    *   R11[11]  |R10[10]  |R9 [9]      |R8 [8] 
    *   R7 [7]   |R6 [6]   |R5 [5]      |R4 [4]
    *   R3 [3]   |R2 [2]   |R1 [1]      |R0 [0]   ]
    * 
    *  0 - Pin is set low
    *  1 - Pin is set high */
    volatile uint32_t BSRR;
    /* BSRR = Bit Set / Reset Register
      Used to set or reset individual GPIO pins atomically
      Width: 32 bits
      Lower 16 bits (0–15): Set pins
      Upper 16 bits (16–31): Reset pins   

    *Set:  [R15[15]  |R14[14]  |R13[13]     |R12[12] 
    *       R11[11]  |R10[10]  |R9 [9]      |R8 [8] 
    *       R7 [7]   |R6 [6]   |R5 [5]      |R4 [4]
    *       R3 [3]   |R2 [2]   |R1 [1]      |R0 [0]   ]
    * 
    *Reset:   [R15[31]  |R14[30]   |R13[29]     |R12[28] 
    *          R11[27]  |R10[26]   |R9 [25]     |R8 [24] 
    *          R7 [23]  |R6 [22]   |R5 [21]     |R4 [20]
    *          R3 [19]  |R2 [18]   |R1 [17]     |R0 [16]   ]
    * 
    * only writing 1 to either pin takes effect. writing 1 to set, sets output high
    * writing Reset to 1 sets output low */
    volatile uint32_t LCKR;
    /*
    Lock Register, locks register types until next reset to prevent critical failure.
    Only affects MODER, OTYPER, OSPEEDR, PUPDR, and AFR registers.
    17 bit register:
    [R15[15]  |R14[14]  |R13[13]     |R12[12] 
    R11[11]  |R10[10]  |R9 [9]      |R8 [8] 
    R7 [7]   |R6 [6]   |R5 [5]      |R4 [4]
    R3 [3]   |R2 [2]   |R1 [1]      |R0 [0]   ]

    R16 is the LCKK register.

    *Write 1 to the pin(s) you want to lock in LCKR (LCKy bits)

    Write 1 to LCKK (bit 16) while keeping the LCKy bits set

    Write 0 to LCKK (bit 16) while keeping LCKy bits set

    Write 1 to LCKK again
    Read LCKK to confirm the lock
    *
    * 
    * */
    volatile uint32_t AFR[2];
    /*  Alternate Function Register. Used to specify SPI, UART, etc.
    *   AFR[0] controls pins 0-7, and AFR[1] controls pins 8-15
    *   
    *   There are 64 bits total and 4 bits per Register, allowing for up to 16 different Alternate
    *   functions per pin (although they don't have 16 available, usually only 2-3).
    * 
    *   
    * 
    * 
    *   0000	System functions (like MCO, SWJ, TIMx_ETR)
        0001	TIM1, TIM2 (Timers)
        0010	TIM3, TIM4, TIM5 (Timers)
        0011	TIM8, TIM9, TIM10 (Timers)
        0100	I2C1, I2C2, I2C3
        0101	SPI1, SPI2, SPI3
        0110	SPI3 (some pins), SAI1
        0111	USART1, USART2, USART3
        1000	UART4, UART5, USART6
        1001	CAN1, CAN2, TIM12
        1010	OTG_FS / OTG_HS USB
        1011	Ethernet (ETH)
        1100	FMC (Flexible Memory Controller), SDIO
        1101	DCMI (Camera interface)
        1110	LTDC (LCD-TFT display controller)
        1111	EVENTOUT / System signals*/
} GPIO_TypeDef;


#define GPIOB ((GPIO_TypeDef *) GPIOB_BASE)
/*
*   We define the struct above as a series of 4 byte unsigned integers that follows the general
*   structure of each GPIO Register we control. 
*/
#define RCC_AHB1ENR (*(volatile uint32_t*)(RCC_BASE + 0x30))
//RCC_Base + 0x30 takes us to the AHB1 Register block from 0x4002000 to 0x400267FF
#define RCC_AHB1ENR_GPIOBEN (1 << 1)
//GPIOA is set to bit 0
//GPIOB is set to bit 1
//GPIOC is set to bit 2


void init_PB6and7_asI2C(void) {
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    // Set PB0 as output (MODER0 = 01)
    GPIOB->MODER &= ~(0x3 << (6 * 2)); //set PB6 Mode to 0 initially
    GPIOB->MODER |=  (0x2 << (6 * 2)); // Set PB6 mode to AFR
    GPIOB->AFR[0] &= ~(0xF << (6 * 4));//  set AFR to I2C 
    GPIOB->AFR[0] |= (0x4 << (6 * 4));//  set AFR to I2C 
    // Open Drain
    GPIOB->OTYPER |= (0x1 << (6));
    //Speed to Medium
    GPIOB->OSPEEDR &= ~(0x3 << (6 * 2)); //reset OSPEEDR
    GPIOB->OSPEEDR |= (0x3 << (6 * 2)); //set OSPEEDR to Fastest for I2C
    // Pullup
    GPIOB->PUPDR &= ~(0x3 << (6*2));
    GPIOB->PUPDR |= (0x1 << (6*2));
    // Set PB7 as output (MODER0 = 01)
    GPIOB->MODER &= ~(0x3 << (7 * 2)); //set PB7 Mode to 0 initially
    GPIOB->MODER |=  (0x2 << (7 * 2)); // Set PB7 mode to AFR
    GPIOB->AFR[0] &= ~(0xF << (7 * 4));//  set AFR to I2C 
    GPIOB->AFR[0] |= (0x4 << (7 * 4));//  set AFR to I2C 
    // Open Drain
    GPIOB->OTYPER |= (0x1 << (7));
    //Speed to Medium
    GPIOB->OSPEEDR &= ~(0x3 << (7 * 2)); //reset OSPEEDR
    GPIOB->OSPEEDR |= (0x3 << (7 * 2)); //set OSPEEDR to Fastest for I2C
    // Pullup
    GPIOB->PUPDR &= ~(0x3 << (7*2));
    GPIOB->PUPDR |= (0x1 << (7*2));
}

