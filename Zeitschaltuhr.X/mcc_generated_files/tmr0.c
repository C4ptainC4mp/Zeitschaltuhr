/**
  TMR0 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    tmr0.c

  @Summary
    This is the generated driver implementation file for the TMR0 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs 

  @Description
    This source file provides APIs for TMR0.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs  - 1.45
        Device            :  PIC18F26K22
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/

#include <xc.h>
#include "tmr0.h"
#include "../share.h"
#include "../lcd.h"

/**
  Section: Global Variables Definitions
*/

void (*TMR0_InterruptHandler)(void);

volatile uint16_t timer0ReloadVal;

/**
  Section: TMR0 APIs
*/


void TMR0_Initialize(void)
{
    // Set TMR0 to the options selected in the User Interface

    // T0PS 1:256; T08BIT 16-bit; T0SE Increment_hi_lo; T0CS FOSC/4; TMR0ON off; PSA not_assigned; 
    T0CON = 0x1F;

    // TMR0H; 
    TMR0H = 0x63;

    // TMR0L; 
    TMR0L = 0xBF;

    // Load TMR0 value to the 16-bit reload variable
    timer0ReloadVal = (uint16_t) ((TMR0H << 8) | TMR0L);

    // Clear Interrupt flag before enabling the interrupt
    INTCONbits.TMR0IF = 0;

    // Enabling TMR0 interrupt.
    INTCONbits.TMR0IE = 1;

    // Set Default Interrupt Handler
    TMR0_SetInterruptHandler(TMR0_DefaultInterruptHandler);

    // Start TMR0
    TMR0_StartTimer();
}

void TMR0_StartTimer(void)
{
    // Start the Timer by writing to TMR0ON bit
    T0CONbits.TMR0ON = 1;
}

void TMR0_StopTimer(void)
{
    // Stop the Timer by writing to TMR0ON bit
    T0CONbits.TMR0ON = 0;
}

uint16_t TMR0_ReadTimer(void)
{
    uint16_t readVal;
    uint8_t readValLow;
    uint8_t readValHigh;

    readValLow  = TMR0L;
    readValHigh = TMR0H;
    readVal  = ((uint16_t)readValHigh << 8) + readValLow;

    return readVal;
}

void TMR0_WriteTimer(uint16_t timerVal)
{
    // Write to the Timer0 register
    TMR0H = timerVal >> 8;
    TMR0L = (uint8_t) timerVal;
}

void TMR0_Reload(void)
{
    // Write to the Timer0 register
    TMR0H = timer0ReloadVal >> 8;
    TMR0L = (uint8_t) timer0ReloadVal;
}

void TMR0_ISR(void)
{

    // clear the TMR0 interrupt flag
    INTCONbits.TMR0IF = 0;

    // reload TMR0
    // Write to the Timer0 register
    TMR0H = (uint8_t) (timer0ReloadVal >> 8);
    TMR0L = (uint8_t) timer0ReloadVal;

    if(TMR0_InterruptHandler)
    {
        TMR0_InterruptHandler();
    }

    // add your TMR0 interrupt custom code
}


void TMR0_SetInterruptHandler(void (* InterruptHandler)(void)){
    TMR0_InterruptHandler = InterruptHandler;
}

void TMR0_DefaultInterruptHandler(void)
{
    //check for button presses
    static bool btn0_old = 1;
    static bool btn1_old = 1;
    static int count = 0;
    static int countSec = 0;
    static int countPWM = 0;
    static int btn0_longPressCount = 0;
    static int btn1_longPressCount = 0;
    static uint8_t pwmPercent = 0;
    
    countSec++;
    count++;
    countPWM++;
    
    if(countPWM >= 120)
    {
        countPWM = 0;
        if(pwmPercent < pwmSetPercent)
        {
            pwmPercent++;
            SetLEDBrightness(pwmPercent);
        }
        else if(pwmPercent > pwmSetPercent)
        {
            pwmPercent--;
            SetLEDBrightness(pwmPercent);
        }
    }
    
    if(countSec >= 99) //every second
    {
        updateTimeFlag = true;
        countSec = 0;
        
        if(backlightCountdown > 0) backlightCountdown--;
    }
    
    //flash Flag
    if(countSec >= (FLASHONTIME))
    {
        flashFlag = true;
    }
    else
    {
        flashFlag = false;
    }
    
    if(count >= 9)
    {
        count = 0;
        msFlag = true;
        
        if(BTN0 == 0 && btn0_old == 1)  //button 0 got pressed
        {
            btn0Flag = true;
            btn0_old = BTN0;
            btn0_longPressCount = LONGPRESSCOUNT;
            backlightCountdown = BACKLIGHTTIME;
        }
        else if(BTN0 == 0 && btn0_old == 0) //button 0 is pressed
        {
            if(btn0_longPressCount > 0)
            {
                btn0_longPressCount--;
            }
            
            if(btn0_longPressCount == 0)
            {
                btn0LongPressFlag = true;
            }
            
            backlightCountdown = BACKLIGHTTIME;
        }
        else if(BTN0 == 1 && btn0_old == 0) //button 0 is released
        {
            //btn0Flag = false;
            btn0LongPressFlag = false;
            btn0_longPressCount = 0;
            btn0_old = BTN0;
        }
        else
        {
            btn0_old = BTN0;
        }
        
        if(BTN1 == 0 && btn1_old == 1)  //button 1 got pressed
        {
            btn1Flag = true;
            btn1_old = BTN1;
            btn1_longPressCount = LONGPRESSCOUNT;
            backlightCountdown = BACKLIGHTTIME;
            
        }
        else if(BTN1 == 0 && btn1_old == 0) //button 1 is pressed
        {
            if(btn1_longPressCount > 0)
            {
                btn1_longPressCount--;
            }
            
            if(btn1_longPressCount == 0)
            {
                btn1LongPressFlag = true;
            }
            
            backlightCountdown = BACKLIGHTTIME;
        }
        else if(BTN1 == 1 && btn1_old == 0) //button 1 is released
        {
            //btn1Flag = false;
            btn1LongPressFlag = false;
            btn1_longPressCount = 0;
            btn1_old = BTN1;
        }
        else
        {
            btn1_old = BTN1;
        }
    }
}

/**
  End of File
*/