#include "i2c.c"

#define LCD_ADDR 0x27  // I2C address of LCD

// Control bits
#define LCD_RS  0x01  // Register select
#define LCD_RW  0x02  // Read/Write (we only write)
#define LCD_EN  0x04  // Enable
#define LCD_BL  0x08  // Backlight

void lcd_write_nibble(uint8_t nibble) {
    // Send 4 bits + backlight
    I2C2->CR1 |= (1 << 8);             // START
    while (!(I2C2->SR1 & (1 << 0)));

    I2C2->DR = (LCD_ADDR << 1) | 0;    // Address + write
    while (!(I2C2->SR1 & (1 << 7))); //Wait until TXE is set

    I2C2->DR = nibble | LCD_BL | LCD_EN; // EN high
    while (!(I2C2->SR1 & (1 << 7))); //Wait until TXE is set

    I2C2->DR = nibble | LCD_BL;          // EN low
    while (!(I2C2->SR1 & (1 << 7))); //Wait until TXE is set

    I2C2->CR1 |= (1 << 9);               // STOP
}

void lcd_command(uint8_t cmd) {
    // Upper nibble
    lcd_write_nibble((cmd & 0xF0));
    // Lower nibble
    lcd_write_nibble((cmd << 4) & 0xF0);
}

void lcd_write_char(char c) {
    // Upper nibble
    lcd_write_nibble((c & 0xF0) | LCD_RS);
    // Lower nibble
    lcd_write_nibble(((c << 4) & 0xF0) | LCD_RS);
}

void lcd_init(void) {
    // Wait 50ms after power-on
    for (volatile int i = 0; i < 50000; i++);

    // 4-bit mode init
    lcd_write_nibble(0x30);
    for (volatile int i = 0; i < 10000; i++);
    lcd_write_nibble(0x30);
    for (volatile int i = 0; i < 10000; i++);
    lcd_write_nibble(0x30);
    for (volatile int i = 0; i < 10000; i++);
    lcd_write_nibble(0x20);  // 4-bit mode

    // Function set: 2 lines, 5x8 dots
    lcd_command(0x28);

    // Display ON, Cursor OFF
    lcd_command(0x0C);

    // Clear display
    lcd_command(0x01);

    // Entry mode set: increment cursor
    lcd_command(0x06);
}

void lcd_write_string(const char *str) {
    while (*str) {
        lcd_write_char(*str++);
    }
}

