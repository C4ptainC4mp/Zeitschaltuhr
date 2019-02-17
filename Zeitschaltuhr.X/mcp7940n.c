/*
 * File:   mcp7940n.c
 * Author: markus
 *
 * Created on May 8, 2018, 3:59 PM
 */

#include <xc.h>
#include <stdbool.h>
#include "mcp7940n.h"
#include "mcc_generated_files/i2c2.h"
#include "lcd.h"
#include "time.h"

time MCP_ReadTime(void)
{
    uint8_t regAddr = 0x00;
    uint8_t data[3];
    I2C2_MESSAGE_STATUS status;
    uint8_t writeBuffer[1];
    uint8_t timeOut;
    uint8_t *pD;
    time newTime;
    
    newTime.hr = 0;
    newTime.min = 0;
    newTime.sec = 0;

    pD = data;

    for (uint8_t i = 0; i < 3; i++, regAddr++, pD++)
    {
        writeBuffer[0] = regAddr;
        timeOut = 0;
        while(status != I2C2_MESSAGE_FAIL)
        {
            I2C2_MasterWrite(writeBuffer, 1, MCP7940N_ADDR, &status);
            
            while(status == I2C2_MESSAGE_PENDING);                  // wait for the message to be sent or status has changed.

            if (status == I2C2_MESSAGE_COMPLETE) break;

            if (timeOut == 100)
                break;
            else
                timeOut++;
        }

        if (status == I2C2_MESSAGE_COMPLETE)
        {
            timeOut = 0;
            while(status != I2C2_MESSAGE_FAIL)
            {
                I2C2_MasterRead(pD, 1, MCP7940N_ADDR, &status);
  
                while(status == I2C2_MESSAGE_PENDING);              // wait for the message to be sent or status has changed.

                if(status == I2C2_MESSAGE_COMPLETE) break;

                if(timeOut == 100) 
                    break;
                else
                    timeOut++;
            }
        }
        // exit if the last transaction failed
        if (status == I2C2_MESSAGE_FAIL)
        {
            return newTime;
        }
        
        switch(i)
        {
            case 0:
                data[i] &= 0x7f;
                newTime.sec = ((data[i]>>4)*10) + (data[i] & 0x0f);
                break;
            case 1:
                data[i] &= 0x7f;
                newTime.min = ((data[i]>>4)*10) + (data[i] & 0x0f);
                break;
            case 2:
                data[i] &= 0x3f;
                newTime.hr = ((data[i]>>4)*10) + (data[i] & 0x0f);
                break;
            default:
                break;
        }
    }
    
    //ignore seconds
    newTime.sec = 0;
    
    return newTime;

}

void MCP_WriteTime(time systemTime)
{
        
    uint8_t regAddr = 0x00;
    uint8_t sourceData[4] = { 0x80, 0x00, 0x00, 0x08 }; 
    uint8_t writeBuffer[2];
    uint8_t *pD;
    uint8_t timeOut;

    I2C2_MESSAGE_STATUS status = I2C2_MESSAGE_PENDING;

    sourceData[0] = 0x80 | ((systemTime.sec / 10) << 4) | (systemTime.sec % 10);
    sourceData[1] = ((systemTime.min / 10) << 4) | (systemTime.min % 10);
    sourceData[2] = (((systemTime.hr / 10) << 4) | (systemTime.hr % 10)) & 0x3f;
    
    pD = sourceData;

    for (uint8_t i = 0; i < 4; i++, regAddr++)
    {
        writeBuffer[0] = regAddr;

        // data to be written
        writeBuffer[1] = *pD++;

        timeOut = 0;
        while(status != I2C2_MESSAGE_FAIL)
        {
            I2C2_MasterWrite(writeBuffer, 2, MCP7940N_ADDR, &status);

            // wait for the message to be sent or status has changed.
            while(status == I2C2_MESSAGE_PENDING);

            if (status == I2C2_MESSAGE_COMPLETE) break;

            // check for max retry and skip this byte
            if (timeOut == 100)
                break;
            else
                timeOut++;
        }

        if (status == I2C2_MESSAGE_FAIL)
        {
            break;
        }
    }
}

bool MCP_CheckActive(void)
{
    uint8_t regAddr = 0x00;
    uint8_t data[1];
    I2C2_MESSAGE_STATUS status;
    uint8_t writeBuffer[1];
    uint8_t timeOut;
    uint8_t *pD;

    pD = data;

    writeBuffer[0] = regAddr;
    timeOut = 0;
    while(status != I2C2_MESSAGE_FAIL)
    {
        I2C2_MasterWrite(writeBuffer, 1, MCP7940N_ADDR, &status);

        while(status == I2C2_MESSAGE_PENDING);                  // wait for the message to be sent or status has changed.

        if (status == I2C2_MESSAGE_COMPLETE) break;

        if (timeOut == 100)
            break;
        else
            timeOut++;
    }

    if (status == I2C2_MESSAGE_COMPLETE)
    {
        timeOut = 0;
        while(status != I2C2_MESSAGE_FAIL)
        {
            I2C2_MasterRead(pD, 1, MCP7940N_ADDR, &status);

            while(status == I2C2_MESSAGE_PENDING);              // wait for the message to be sent or status has changed.

            if(status == I2C2_MESSAGE_COMPLETE) break;

            if(timeOut == 100) 
                break;
            else
                timeOut++;
        }
    }
    // exit if the last transaction failed
    if (status == I2C2_MESSAGE_FAIL)
    {
        return false;
    }
    
    return (data[0] & 0x80);
}