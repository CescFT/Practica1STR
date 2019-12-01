/*********************************************************************************
* Name: DCMotor.h
* Description: header for a the motor library
* Date: 27/11/2019
* Programmer: David Gamez (david.gameza@urv.cat)
*********************************************************************************/


#ifndef MBED_MOTOR_H
#define MBED_MOTOR_H

#include "mbed.h"

#define M_FORWARD 0
#define M_BACKGUARD 1
#define M_RELEASE 2

class Motor
{
public:
    Motor(int M1);
    void Direction(int direction);
    void setSpeed(float percentage_M1, float percentage_M2, float percentage_M3, float percentage_M4);
private:
    PwmOut _pwm1;//pwm1
    PwmOut _pwm2;//pwm2
    PwmOut _pwm3;//pwm3
    PwmOut _pwm4;//pwm4
    
    DigitalOut _SERIALDATA;//serial data
    DigitalOut _CLOCK;//clock
    DigitalOut _LATCH;//latch
    DigitalOut _ENABLE;//enable, set low to be in duty
};

#endif