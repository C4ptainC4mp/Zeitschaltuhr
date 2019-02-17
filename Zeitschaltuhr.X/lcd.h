/* 
 * File:   lcd.h
 * Author: Markus
 */
#ifndef _LCD_H
#define _LCD_H

#include <stdint.h>

#define CHIP1   1
#define CHIP2   2
#define CHIP12  3

void enablePulse(void);
void DisplayWrite(char reg, uint8_t data, unsigned char cs);
void LCD_Busy(void);
void LCD_Init(void);
void LCD_gotoxy(unsigned char x, unsigned char y);
void LCD_string(char *text);
void LCD_string_xy(unsigned char x, unsigned char y, char *text);
void LCD_string_line(unsigned char line,char *text);
void LCD_putc(unsigned char c);
void LCD_putc2(unsigned char c);
void LCD_Cursor_ON(void);
void LCD_Cursor_OFF(void);
void LCD_clear(void);

#endif

