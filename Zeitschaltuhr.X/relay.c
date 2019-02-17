/*
 * File:   relay.c
 * Author: Markus
 *
 * Created on 22. Mai 2018, 18:45
 */

#include <xc.h>
#include <stdbool.h>
#include "relay.h"
#include "time.h"
#include "share.h"

/*returns the switching state of relay r, 1 if it should be on, 0 if it should be off*/
bool RELAY_CheckTimes(relay* r, time systemTime)
{ 
    if(TIME_equal(r->timeOn, r->timeOff))
    {
        return false;
    }
    else if(TIME_lower(r->timeOn, r->timeOff)) //on time is lower than off time
    {
        if(TIME_greaterEqual(systemTime, r->timeOn) && TIME_lower(systemTime, r->timeOff))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else    //on time is higher than off time. relay is on at 00:00
    {
        if(TIME_greaterEqual(systemTime, r->timeOn) || TIME_lower(systemTime, r->timeOff))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

/*switches the relay r to state*/
void RELAY_Switch(relay* r, bool state)
{
    if(r->isPWM)
    {
        if(state)
            pwmSetPercent = 100;
        else
            pwmSetPercent = 0;
    }
    else
    {
        if(state)
            RELAYSLATCH |= 1 << r->pin;
        else
            RELAYSLATCH &= 0 << r->pin;
    }
    
    r->active = state;
}