#include <xc.h>
#include <stdint.h>
#include "lcd.h"
#include "mcc_generated_files/mcc.h"

#define _LCDPORTA

#ifdef _LCDPORTA
#define E1          LATAbits.LA0
#define T_E1		TRISAbits.TRISA0
#define E2          LATAbits.LA1
#define T_E2		TRISAbits.TRISA1
#define RS          LATAbits.LA2
#define T_RS		TRISAbits.TRISA2
#define RW          LATAbits.LA3
#define T_RW		TRISAbits.TRISA3

#define WRITE_DATA  LATA
#define TRIS_DATA	TRISA
#define READ_DATA   PORTA
#endif

#define DELAYUS     1
#define DELAYMS     2
#define DELAYINIT   20

unsigned char activeChip = 1;

//*****************************************************************************
void enablePulse(void)
{
    switch(activeChip)
    {
        case CHIP1:
            E1=1;
            E2=0;
            __delay_us(DELAYUS);
            E1=0;
            break;
        case CHIP2:
            E1=0;
            E2=1;
            __delay_us(DELAYUS);
            E2=0;
            break;
        case CHIP12:
            E1=1;
            E2=1;
            __delay_us(DELAYUS);
            E1=0;
            E2=0;
            break;
    }
}
//*****************************************************************************
void DisplayWrite(char reg, uint8_t data, unsigned char cs)
{
    activeChip = cs;
    
    TRIS_DATA = 0x00;
    RW = 0;
    RS = reg;
    
    WRITE_DATA &= 0x0f;
    WRITE_DATA |= (data & (uint8_t)0xf0);
    enablePulse();
    WRITE_DATA &= 0x0f;
    WRITE_DATA |= ((data << 4) & 0xf0);
    enablePulse();
}
//*****************************************************************************
void LCD_Busy(void)
{   
    uint8_t trisTemp = TRIS_DATA;
    TRIS_DATA = 0xf0;
    RW = 1;
    RS = 0;
    
    uint8_t r1;
    uint8_t r2;
    
    do
    {
        E1 = 1;
        __delay_us(DELAYUS);
        r1 = READ_DATA & 0xf0;
        E1 = 0;
        __delay_us(DELAYUS);
        E1 = 1;
        __delay_us(DELAYUS);
        r1 |= ((READ_DATA >> 4) & 0x0f);
        E1 = 0;
        E2 = 1;
        __delay_us(DELAYUS);
        r2 = READ_DATA & 0xf0;
        E2 = 0;
        __delay_us(DELAYUS);
        E2 = 1;
        __delay_us(DELAYUS);
        r2 |= ((READ_DATA >> 4) & 0x0f);
        E2 = 0;
    } while((r1 | r2) & 0x80);
    
    TRIS_DATA = trisTemp;
}
//*****************************************************************************
void LCD_Init(void)
{
    WRITE_DATA = 0x00;
    TRIS_DATA = 0x00;

    __delay_ms(DELAYINIT);
    activeChip = CHIP12;
    //select 8 bit mode
    WRITE_DATA = 0x30;
    enablePulse();
    __delay_ms(DELAYMS);
    WRITE_DATA = 0x30;
    enablePulse();
    __delay_ms(DELAYMS);
    WRITE_DATA = 0x30;
    enablePulse();
    __delay_ms(DELAYMS);
    //Select 4 bit mode
    WRITE_DATA = 0x20;
    enablePulse();
    __delay_ms(DELAYMS);
    LCD_Busy();
    
    //Functionset, 4-Bit-Modus, 5*7 Punkte
    DisplayWrite(0,0x28,CHIP12);
    __delay_ms(DELAYMS);
    LCD_Busy();

    // Display on/off, Cursor
    DisplayWrite(0,0x0c,CHIP12);
    __delay_ms(DELAYMS);
    LCD_Busy();

    // Display clear
    DisplayWrite(0,0x1,CHIP12);
    __delay_ms(DELAYMS);
    LCD_Busy();

    // Set entry mode
    DisplayWrite(0,0x6,CHIP12);
    __delay_ms(DELAYMS);
    LCD_Busy();
}
//*****************************************************************************
// links oben: x=1, y=1
//*****************************************************************************
void LCD_gotoxy(unsigned char x, unsigned char y)
{
    unsigned char adresse;

    switch(y)
    {
        case 1:
            activeChip = 1;
            adresse=0x00;
            break;
        case 2:
            activeChip = 1;
            adresse=0x40;
            break;
        case 3:
            activeChip = 2;
            adresse=0x00;
            break;
        case 4:
            activeChip = 2;
            adresse=0x40;
            break;
    }

    adresse += x-1;
    adresse |= 0x80;
    LCD_Busy();
    DisplayWrite(0,adresse,activeChip);
}
//*****************************************************************************
void LCD_string(char *text)
{
    while (*text != '\0')
    {
        __delay_us(DELAYUS);
        LCD_Busy();
        DisplayWrite(1,*text,activeChip);
        text++;
    }
}
//*****************************************************************************
void LCD_string_xy(unsigned char x, unsigned char y,char *text)
{
    LCD_gotoxy(x,y);
    LCD_string(text);
}
//*****************************************************************************
void LCD_string_line(unsigned char line,char *text)
{
    LCD_gotoxy(1,line);

    uint8_t i = 0;
    while ((*text != '\0') && (i < 40))
    {
        __delay_us(DELAYUS);
        LCD_Busy();
        DisplayWrite(1,*text,activeChip);
        text++;
        i++;
    }
}
//*****************************************************************************
void LCD_putc(unsigned char c)
{
    __delay_us(DELAYUS);
    LCD_Busy();
    DisplayWrite(1,c,1);
}
//*****************************************************************************
void LCD_putc2(unsigned char c)
{
    __delay_us(DELAYUS);
    LCD_Busy();
    DisplayWrite(1,c,2);
}
//*****************************************************************************
void LCD_Cursor_ON(void)
{
    __delay_us(DELAYUS);
    LCD_Busy();
    DisplayWrite(0,0x0F,1);
    __delay_us(DELAYUS);
    LCD_Busy();
    DisplayWrite(0,0x0F,2);
}
//*****************************************************************************
void LCD_Cursor_OFF(void)
{
    __delay_us(DELAYUS);
    LCD_Busy();
    DisplayWrite(0,0x0C,1);
    __delay_us(DELAYUS);
    LCD_Busy();
    DisplayWrite(0,0x0C,2);
}
//*****************************************************************************
void LCD_clear(void)
{
    __delay_us(DELAYUS);
    LCD_Busy();
    DisplayWrite(0,0x01,1);
    __delay_us(DELAYUS);
    LCD_Busy();
    DisplayWrite(0,0x01,2);
}
//*****************************************************************************