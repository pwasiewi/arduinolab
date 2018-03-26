/*
 * TimerOne.h
 *
 *  Created on: 30 wrz 2015
 *      Author: pwas
 */

#ifndef TIMERONE_H_
#define TIMERONE_H_


#include <avr/io.h>
#include <avr/interrupt.h>

#define RESOLUTION 65536    // Timer1 is 16 bit

class TimerOne
{
  public:

    // properties
    unsigned int pwmPeriod;
    unsigned char clockSelectBits;

    // methods
    void initialize(long microseconds=1000000);
    void start();
    void stop();
    void restart();
    void pwm(char pin, int duty, long microseconds=-1);
    void disablePwm(char pin);
    void attachInterrupt(void (*isr)(), long microseconds=-1);
    void detachInterrupt();
    void setPeriod(long microseconds);
    void setPwmDuty(char pin, int duty);
    void (*isrCallback)();
};

extern TimerOne Timer1;


#endif /* TIMERONE_H_ */
