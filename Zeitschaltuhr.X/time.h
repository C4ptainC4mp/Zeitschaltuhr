/* 
 * File:   time.h
 * Author: markus
 *
 * Created on May 22, 2018, 18:25
 */
#ifndef _TIME_H
#define _TIME_H

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

//STRUCTURES
typedef struct
{
    uint8_t sec;
    uint8_t min;
    uint8_t hr;
}
time;

//PROTOTYPES
bool TIME_equal(time t1, time t2);
bool TIME_lower(time t1, time t2);
bool TIME_greaterEqual(time t1, time t2);

#endif