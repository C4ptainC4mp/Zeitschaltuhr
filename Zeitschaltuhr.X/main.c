/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs 

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
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

#include "mcc_generated_files/mcc.h"
#include <xc.h>
#include <pic18f26k22.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "lcd.h"
#include "mcp7940n.h"
#include "time.h"
#include "relay.h"

//BUILD CONFIGURATION
//#pragma warning disable(1404)

//DEFINES
#define RELAYSCOUNT         9
#define EEPROMSTARTADDRESS  0x00
#define ON                  1
#define OFF                 0
#define BACKLIGHTDURATION   10      //seconds
#define SYSTEM_TIME_LEN     5
#define NAMELENGTHMAX       22

//PROTOTYPES
void SetLEDBrightness(uint8_t percent);
void SetLCDBrightness(uint8_t percent);
void WriteDataEEPROM(void);
void ReadDataEEPROM(void);
void PrintSystemSetup(void);
void PrintListEntries(uint8_t listPosition);
void EditListEntry(uint8_t listPosition);
void PrintSystemTime(void);

//GLOBAL VARIABLES
relay relays[RELAYSCOUNT];
//time systemTime;
//FLAGS
unsigned uint8_t brightness = 50;

volatile bool btn0Flag = false;
volatile bool btn0LongPressFlag = false;
volatile bool btn1Flag = false;
volatile bool btn1LongPressFlag = false;
volatile bool msFlag = false;
volatile bool updateTimeFlag = false;
volatile bool flashFlag = false;
volatile uint8_t backlightCountdown = 30;
volatile time systemTime;
volatile uint8_t pwmSetPercent = 0;

const uint16_t pwmtable[101] =
{
    0,3,3,3,3,4,4,4,4,5,5,5,6,6,6,7,7,7,8,8,9,9,10,10,11,11,12,13,14,
    15,16,17,18,19,21,22,23,25,26,28,29,31,33,35,37,39,42,44,47,50,53,
    56,60,63,67,71,76,80,85,90,96,102,108,114,121,129,137,145,154,163,
    173,184,195,207,219,233,247,262,278,295,313,332,352,374,397,421,446,
    474,503,533,566,600,637,676,717,761,807,856,909,964,1023
};

void main(void) 
{
    uint8_t listPosition = 0;
    bool state = false;
    
    // Initialize the device
    SYSTEM_Initialize();
    
    LCD_Init();
    ReadDataEEPROM();
    
    systemTime.hr = 0;
    systemTime.min = 0;
    systemTime.sec = 0;
    
    relays[0].pin = 0;
    relays[1].pin = 1;
    relays[2].pin = 2;
    relays[3].pin = 3;
    relays[4].pin = 4;
    relays[5].pin = 5;
    relays[6].pin = 6;
    relays[7].pin = 7;
    relays[8].pin = 0;  //dummy pin
    
    relays[0].isPWM = false;
    relays[1].isPWM = false;
    relays[2].isPWM = false;
    relays[3].isPWM = false;
    relays[4].isPWM = false;
    relays[5].isPWM = false;
    relays[6].isPWM = false;
    relays[7].isPWM = false;
    relays[8].isPWM = true;
    
    SetLCDBrightness(brightness);
    SetLEDBrightness(0);
    
    //Boot screen
    LCD_string_line(1,(char*)("Hello! :)"));
    __delay_ms(1000);
    LCD_clear();
    
    INTERRUPT_PeripheralInterruptEnable();
    INTERRUPT_GlobalInterruptEnable();
    
    //This code initializes the switches to a default value, decomment, program
    /*
    relays[0].timeOn.hr = 8;
    relays[0].timeOn.min = 30;
    relays[0].timeOn.sec = 0;
    relays[0].timeOff.hr = 21;
    relays[0].timeOff.min = 30;
    relays[0].timeOff.sec = 0; 

    relays[1].timeOn.hr = 8;
    relays[1].timeOn.min = 35;
    relays[1].timeOn.sec = 0;
    relays[1].timeOff.hr = 21;
    relays[1].timeOff.min = 25;
    relays[1].timeOff.sec = 0; 
    
    relays[2].timeOn.hr = 8;
    relays[2].timeOn.min = 40;
    relays[2].timeOn.sec = 0;
    relays[2].timeOff.hr = 21;
    relays[2].timeOff.min = 20;
    relays[2].timeOff.sec = 0; 
    
    relays[3].timeOn.hr = 8;
    relays[3].timeOn.min = 45;
    relays[3].timeOn.sec = 0;
    relays[3].timeOff.hr = 21;
    relays[3].timeOff.min = 15;
    relays[3].timeOff.sec = 0; 
    
    relays[4].timeOn.hr = 8;
    relays[4].timeOn.min = 50;
    relays[4].timeOn.sec = 0;
    relays[4].timeOff.hr = 21;
    relays[4].timeOff.min = 10;
    relays[4].timeOff.sec = 0; 
    
    relays[5].timeOn.hr = 8;
    relays[5].timeOn.min = 55;
    relays[5].timeOn.sec = 0;
    relays[5].timeOff.hr = 21;
    relays[5].timeOff.min = 5;
    relays[5].timeOff.sec = 0; 
    
    relays[6].timeOn.hr = 9;
    relays[6].timeOn.min = 0;
    relays[6].timeOn.sec = 0;
    relays[6].timeOff.hr = 21;
    relays[6].timeOff.min = 0;
    relays[6].timeOff.sec = 0; 
    
    relays[7].timeOn.hr = 12;
    relays[7].timeOn.min = 30;
    relays[7].timeOn.sec = 0;
    relays[7].timeOff.hr = 13;
    relays[7].timeOff.min = 0;
    relays[7].timeOff.sec = 0; 
    
    relays[8].timeOn.hr = 8;
    relays[8].timeOn.min = 28;
    relays[8].timeOn.sec = 0;
    relays[8].timeOff.hr = 21;
    relays[8].timeOff.min = 30;
    relays[8].timeOff.sec = 0; 
    
    strcpy(relays[0].name,"Spot 1");
    strcpy(relays[1].name,"Spot 2");
    strcpy(relays[2].name,"Spot 3");
    strcpy(relays[3].name,"Spot 4");
    strcpy(relays[4].name,"Spot 5");
    strcpy(relays[5].name,"LED Roehre 1");
    strcpy(relays[6].name,"LED Roehre 2");
    strcpy(relays[7].name,"UV-Lampe");
    strcpy(relays[8].name,"LED Streifen");
    WriteDataEEPROM();
    while(1);*/
    
    INTERRUPT_GlobalInterruptEnable();
    
    //Check if the RTC is still active, or the user wants to do a reset.
    if(!MCP_CheckActive() || ((!BTN0) & (!BTN1)))
    {
        LCD_string_line(2,(char*)("Time deleted!"));
        __delay_ms(1000);
        while((!BTN0) & (!BTN1));
        btn0Flag = false;
        btn1Flag = false;
        
        PrintSystemSetup();
        MCP_WriteTime(systemTime);
        WriteDataEEPROM();  //save brightness
    }
    else
    {
        systemTime = MCP_ReadTime();
    }
    
    for(uint8_t i = 0; i < RELAYSCOUNT; i++)
    {
        if((state = RELAY_CheckTimes(&relays[i], systemTime)) != relays[i].active)
        {
            RELAY_Switch(&relays[i], state);
        }
    }
    
    LCD_clear();
    
    PrintSystemTime();
    PrintListEntries(listPosition);
  
    while(1)
    {   
        if(btn0Flag)
        {
            btn0Flag = false;
            SetLCDBrightness(brightness);
            
            listPosition++;
            if(listPosition == RELAYSCOUNT)
                listPosition = 0;
            
            PrintListEntries(listPosition);
        }
        
        if(btn1Flag)
        {
            btn1Flag = false;
            SetLCDBrightness(brightness);
            EditListEntry(listPosition);
            WriteDataEEPROM();
            LCD_clear();
            PrintSystemTime();
            PrintListEntries(listPosition);
        }
        
        if(updateTimeFlag)
        {   
            bool switched = false;
            updateTimeFlag = false;
            
            
            TMR0_StopTimer();
            time tempTime = MCP_ReadTime();
            TMR0_StartTimer();
            
            if(!TIME_equal(tempTime, systemTime))
            {
                systemTime = tempTime;
                PrintSystemTime();
            }

            for(uint8_t i = 0; i < RELAYSCOUNT; i++)
            {
                if((state = RELAY_CheckTimes(&relays[i], systemTime)) != relays[i].active)
                {
                    RELAY_Switch(&relays[i], state);
                    switched = true;
                }
            }
            
            if(switched) PrintListEntries(listPosition);
        }
        
        if(backlightCountdown == 0)
        {
            SetLCDBrightness(0);
        }
    }
}

void WriteDataEEPROM(void)
{
    uint16_t address = EEPROMSTARTADDRESS;
    for(uint8_t i = 0; i < RELAYSCOUNT; i++)
    {
        DATAEE_WriteByte(address++,relays[i].timeOn.hr);
        DATAEE_WriteByte(address++,relays[i].timeOn.min);
        DATAEE_WriteByte(address++,relays[i].timeOff.hr);
        DATAEE_WriteByte(address++,relays[i].timeOff.min);
    }
    
    for(uint8_t i = 0; i < RELAYSCOUNT; i++)
    {
        uint8_t j = 0;
        while((relays[i].name[j]) != '\0')
        {
            DATAEE_WriteByte(address++, relays[i].name[j]);
            j++;
        }
        DATAEE_WriteByte(address++, relays[i].name[j]);
    }
    
    DATAEE_WriteByte(address++, brightness);
    
}

void ReadDataEEPROM(void)
{
    unsigned char address = EEPROMSTARTADDRESS;
    for(uint8_t i = 0; i < RELAYSCOUNT; i++)
    {
        relays[i].timeOn.hr = DATAEE_ReadByte(address++);
        relays[i].timeOn.min = DATAEE_ReadByte(address++);
        relays[i].timeOff.hr = DATAEE_ReadByte(address++);
        relays[i].timeOff.min = DATAEE_ReadByte(address++);
    }
    
    for(uint8_t i = 0; i < RELAYSCOUNT; i++)
    {
        uint8_t j = 0;
        char c;
        while((c = DATAEE_ReadByte(address++)) != '\0')
        {
            relays[i].name[j] = c;
            j++;
        }
        relays[i].name[j] = c;
    }
    
    brightness = DATAEE_ReadByte(address++);
}

void SetLEDBrightness(uint8_t percent)
{
    if(percent == 0)
    {
        TRISBbits.TRISB5 = 1;
    }
    else
    {
        TRISBbits.TRISB5 = 0;
    }
    
    if(percent > 100)
    {
        percent = 100;
    }
    
    EPWM3_LoadDutyValue(pwmtable[percent]);
}

void SetLCDBrightness(uint8_t percent)
{   
    if(percent == 0)
    {
        TRISBbits.TRISB4 = 1;
    }
    else
    {
        TRISBbits.TRISB4 = 0;
    }
    
    if(percent > 100)
    {
        percent = 100;
    }
    
    EPWM1_LoadDutyValue(pwmtable[percent]);
}

void PrintSystemSetup(void)
{
    uint8_t pos = 0;
    char str[40];
    
    LCD_clear();
    LCD_string_line(1, (char*)"Uhrzeit:");
    
    while(pos < 4)
    {
        if(btn0Flag || (btn0LongPressFlag && msFlag))
        {
            msFlag = false;
            btn0Flag = false;
            if (pos == 0)
            {
                systemTime.hr++;
                if(systemTime.hr > 23)systemTime.hr = 0;
            }
            else if(pos == 3)
            {
                systemTime.min++;
                if(systemTime.min > 59) systemTime.min = 0;
            }  
        }
        
        if(btn1Flag)
        {
            btn1Flag = false;
            pos = (uint8_t)3 + pos;
        }
        
        sprintf(str,"%02d:%02d", systemTime.hr, systemTime.min);
        if(flashFlag && !btn0LongPressFlag)
        {
            str[pos] = '_';
            str[pos + (uint8_t)1] = '_';
        } 
        LCD_string_line(2, str); 
    }
    
    LCD_string_line(3, (char*)"Displayhelligkeit:");
    
    while(!btn1Flag)
    {
        if(btn0Flag || (btn0LongPressFlag && msFlag))
        {
            msFlag = false;
            btn0Flag = false;
            brightness++;
            if(brightness > 100) brightness = 0;
            SetLCDBrightness(brightness);
        }
        
        sprintf(str,"%3d", brightness);
        if(flashFlag && !btn0LongPressFlag)
        {
            str[0] = '_';
            str[1] = '_';
            str[2] = '_';
        } 
        LCD_string_line(4, str); 
    }
    
    btn1Flag = false;
}

void PrintListEntries(uint8_t listPosition)
{
    char str1[41];
    char str2[41];
    char str3[41];
    
    if(relays[listPosition].active) 
    {
        sprintf(str1,"%-22s Ein %02d:%02d-%02d:%02d   ",relays[listPosition].name, relays[listPosition].timeOn.hr, relays[listPosition].timeOn.min, relays[listPosition].timeOff.hr, relays[listPosition].timeOff.min);
    }
    else
    {
        sprintf(str1,"%-22s Aus %02d:%02d-%02d:%02d   ",relays[listPosition].name, relays[listPosition].timeOn.hr, relays[listPosition].timeOn.min, relays[listPosition].timeOff.hr, relays[listPosition].timeOff.min);
    }
    LCD_string_line(2,str1);
    
    listPosition++;
    if(listPosition == RELAYSCOUNT)
    {
        listPosition -= RELAYSCOUNT;
    }
    if(relays[listPosition].active) 
    {
        sprintf(str2,"%-22s Ein %02d:%02d-%02d:%02d   ",relays[listPosition].name, relays[listPosition].timeOn.hr, relays[listPosition].timeOn.min, relays[listPosition].timeOff.hr, relays[listPosition].timeOff.min);
    }    
    else
    {
        sprintf(str2,"%-22s Aus %02d:%02d-%02d:%02d   ",relays[listPosition].name, relays[listPosition].timeOn.hr, relays[listPosition].timeOn.min, relays[listPosition].timeOff.hr, relays[listPosition].timeOff.min);
    }
    LCD_string_line(3,str2);
    listPosition++;
    if(listPosition == RELAYSCOUNT)
    {
        listPosition -= RELAYSCOUNT;
    }
    
    if(relays[listPosition].active) 
    {
        sprintf(str3,"%-22s Ein %02d:%02d-%02d:%02d   ",relays[listPosition].name, relays[listPosition].timeOn.hr, relays[listPosition].timeOn.min, relays[listPosition].timeOff.hr, relays[listPosition].timeOff.min);
    }
    else
    {
        sprintf(str3,"%-22s Aus %02d:%02d-%02d:%02d   ",relays[listPosition].name, relays[listPosition].timeOn.hr, relays[listPosition].timeOn.min, relays[listPosition].timeOff.hr, relays[listPosition].timeOff.min);
    }
    
    LCD_string_line(4,str3);
}

void EditListEntry(uint8_t listPosition)
{
    char str[41];
    uint8_t pos = 0;
    char c[] = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-";
    uint8_t i = 0;
    
    LCD_clear();
    
    while(pos < 22)
    {
        sprintf(str,"%-22s %02d:%02d-%02d:%02d",relays[listPosition].name, relays[listPosition].timeOn.hr, relays[listPosition].timeOn.min, relays[listPosition].timeOff.hr, relays[listPosition].timeOff.min);
        
        if(btn0Flag || (btn0LongPressFlag && msFlag))
        {
            msFlag = false;
            btn0Flag = false;
            i++;
            if(i >= strlen(c))i = 0;
            relays[listPosition].name[pos] = c[i];
        }
        
        if(btn1Flag)
        {
            btn1Flag = false;
            int j = 0;
            for(; j < strlen(c); j++)
            {
                if(relays[listPosition].name[pos] == c[j])
                {
                    break;
                }
            }
            if(j == strlen(c))
            {
                relays[listPosition].name[pos] = ' ';
            }
            
            pos++;
            i = 0;
        }
        
        if(flashFlag && !btn0LongPressFlag)
        {
            str[pos] = '_';
        } 
        
        LCD_string_line(2, str); 
    }
    
    relays[listPosition].name[NAMELENGTHMAX] = '\0';
    
    pos = 0;
    
    while(pos < 10)
    {       
        if(btn0Flag || (btn0LongPressFlag && msFlag))
        {
            msFlag = false;
            btn0Flag = false;
            switch(pos)
            {
                case 0:
                    relays[listPosition].timeOn.hr++;
                    if(relays[listPosition].timeOn.hr > 23)relays[listPosition].timeOn.hr = 0;
                    break;
                case 3:
                    relays[listPosition].timeOn.min++;
                    if(relays[listPosition].timeOn.min > 59) relays[listPosition].timeOn.min = 0;
                    break;
                case 6:
                    relays[listPosition].timeOff.hr++;
                    if(relays[listPosition].timeOff.hr > 23)relays[listPosition].timeOff.hr = 0;
                    break;
                case 9:
                    relays[listPosition].timeOff.min++;
                    if(relays[listPosition].timeOff.min > 59) relays[listPosition].timeOff.min = 0;
                    break;
                default:
                    break;
            }
        }
        
        if(btn1Flag)
        {
            btn1Flag = false;
            pos = (uint8_t)(pos + 3);
        }
        
        sprintf(str,"%-22s %02d:%02d-%02d:%02d",relays[listPosition].name, relays[listPosition].timeOn.hr, relays[listPosition].timeOn.min, relays[listPosition].timeOff.hr, relays[listPosition].timeOff.min);
        
        if(flashFlag && !btn0LongPressFlag)
        {
            str[(uint8_t)23 + pos] = '_';
            str[(uint8_t)24 + pos] = '_';
        }
        
        LCD_string_line(2, str); 
    }
}

void PrintSystemTime(void)
{
    char str[41];
    sprintf(str,"Uhrzeit: %02d:%02d%26s", systemTime.hr, systemTime.min,"");
    LCD_string_line(1,str);
}

/**
 End of File
*/