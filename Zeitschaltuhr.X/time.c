/*
 * File:   time.c
 * Author: Markus
 *
 * Created on 22. Mai 2018, 18:22
 */

#include <stdbool.h>
#include "time.h"

/*this function returns 1 if time t1 equal to t2, 0 otherwise*/
bool TIME_equal(time t1, time t2)
{
    return ((t1.hr == t2.hr) && (t1.min == t2.min) && (t1.sec == t2.sec));
}

/*this function returns 1 if time t1 is lower than t2, 0 otherwise*/
bool TIME_lower(time t1, time t2)
{
    return (t1.hr < t2.hr) || ((t1.hr == t2.hr) && (t1.min < t2.min)) || ((t1.hr == t2.hr) && (t1.min == t2.min) && (t1.sec < t2.sec));
}

/*this function returns 1 if time t1 is higher or equal to t2, 0 otherwise*/
bool TIME_greaterEqual(time t1, time t2)
{
    return (t1.hr > t2.hr) || ((t1.hr == t2.hr) && (t1.min > t2.min)) || ((t1.hr == t2.hr) && (t1.min == t2.min) && (t1.sec >= t2.sec));
}