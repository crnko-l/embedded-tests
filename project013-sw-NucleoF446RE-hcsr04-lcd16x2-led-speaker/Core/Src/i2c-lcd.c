#include "i2c-lcd.h"

#define LCD_BACKLIGHT 0x08
#define ENABLE 0x04

void lcd_send_cmd(char cmd)
{
    char data_u, data_l;
    uint8_t data_t[4];
    data_u = (cmd & 0xf0);
    data_l = ((cmd << 4) & 0xf0);
    data_t[0] = data_u | LCD_BACKLIGHT | ENABLE;
    data_t[1] = data_u | LCD_BACKLIGHT;
    data_t[2] = data_l | LCD_BACKLIGHT | ENABLE;
    data_t[3] = data_l | LCD_BACKLIGHT;
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, (uint8_t *)data_t, 4, 100);
}

void lcd_send_data(char data)
{
    char data_u, data_l;
    uint8_t data_t[4];
    data_u = (data & 0xf0);
    data_l = ((data << 4) & 0xf0);
    data_t[0] = data_u | LCD_BACKLIGHT | ENABLE | 0x01;
    data_t[1] = data_u | LCD_BACKLIGHT | 0x01;
    data_t[2] = data_l | LCD_BACKLIGHT | ENABLE | 0x01;
    data_t[3] = data_l | LCD_BACKLIGHT | 0x01;
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, (uint8_t *)data_t, 4, 100);
}

void lcd_clear(void)
{
    lcd_send_cmd(0x01);
    HAL_Delay(2);
}

void lcd_put_cur(int row, int col)
{
    switch (row)
    {
    case 0:
        lcd_send_cmd(0x80 + col);
        break;
    case 1:
        lcd_send_cmd(0xC0 + col);
        break;
    }
}

void lcd_init(void)
{
    HAL_Delay(100);
    lcd_send_cmd(0x30);
    HAL_Delay(5);
    lcd_send_cmd(0x30);
    HAL_Delay(1);
    lcd_send_cmd(0x30);
    lcd_send_cmd(0x20);  // 4-bit mode
    HAL_Delay(10);

    lcd_send_cmd(0x28);  // 4-bit, 2 line, 5x8 font
    lcd_send_cmd(0x08);  // display off
    lcd_send_cmd(0x01);  // clear display
    HAL_Delay(2);
    lcd_send_cmd(0x06);  // entry mode
    lcd_send_cmd(0x0C);  // display on, cursor off
}

void lcd_send_string(char *str)
{
    while (*str)
        lcd_send_data(*str++);
}
