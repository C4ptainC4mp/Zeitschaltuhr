/* 
 * File:   relay.h
 * Author: markus
 *
 * Created on May 22, 2018, 21:23
 */

#ifndef _SHARE_H
#define _SHARE_H

#include <stdbool.h>
#include "time.h"

//DEFINES
#define LONGPRESSCOUNT  15  //in 0.1sec (10 = 1sec)
#define FLASHONTIME 70 //in sec*100
#define BACKLIGHTTIME   30 //in sec

#define RELAYSLATCH   LATC  

void SetLEDBrightness(uint8_t percent);

extern volatile bool btn0Flag;
extern volatile bool btn0LongPressFlag;
extern volatile bool btn1Flag;
extern volatile bool btn1LongPressFlag;

extern volatile bool msFlag;

extern volatile bool updateTimeFlag;

extern volatile bool flashFlag;

extern volatile uint8_t backlightCountdown;

extern volatile uint8_t pwmSetPercent;

extern volatile time systemTime;

#endif