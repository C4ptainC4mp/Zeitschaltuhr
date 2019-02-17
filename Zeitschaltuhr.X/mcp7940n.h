/* 
 * File:   mcp7940n.h
 * Author: markus
 *
 * Created on April 28, 2018, 21:01
 */
#ifndef _MCP7940N_H
#define _MCP7940N_H

#include <stdbool.h>
#include "time.h"


//DEFINES
#define MCP7940N_ADDR       0b1101111

//PROTOTYPES
void MCP_WriteTime(time systemTime);
time MCP_ReadTime(void);
bool MCP_CheckActive(void);

#endif