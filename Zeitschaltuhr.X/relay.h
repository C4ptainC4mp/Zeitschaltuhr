/* 
 * File:   relay.h
 * Author: markus
 *
 * Created on May 22, 2018, 18:43
 */
#ifndef _RELAY_H
#define _RELAY_H
#include <stdbool.h>
#include "time.h"

//DEFINES

//STRUCTURES
typedef struct
{
    bool active;
    char name[23];
    time timeOn;
    time timeOff;
    uint8_t pin;
    bool isPWM;
}
relay;

//PROTOTYPES
void RELAY_Switch(relay* r, bool state);
bool RELAY_CheckTimes(relay* r, time systemTime);

#endif

