#include "mbed.h"
#include "DCMotor.h"

//To begin using this library, define these pin in main.cpp
//pwm1 = pwm pin of motor 1 and so on
//data, clock, latch, enable pin

/* Pin definitions for the XXXXXX-ZZZZZ board */
const PinName pwm1 = D11;
const PinName pwm2 = D3;
const PinName pwm3 = D6;
const PinName pwm4 = D5;
const PinName data_pin = D8;
const PinName clock_pin = D4;
const PinName latch_pin = D12;
const PinName enable_pin = D7;

Motor::Motor(int M1) : _pwm1(pwm1), _pwm2(pwm2), _pwm3(pwm3), _pwm4(pwm4), _SERIALDATA(data_pin), _CLOCK(clock_pin), _LATCH(latch_pin), _ENABLE(enable_pin)
{
    _ENABLE.write(0);//set low to enable output - active low
    _LATCH.write(1);//set high to send if any data is storage
    _LATCH.write(0);//set back to low
}

//define turning direction of 4 motors - if set M1a value to be 1 and M1b value to be 0, will make terminant M1A on the shield have higher volatage than terminant M1B
void Motor::Direction(int direction)
{
	int M1a, M1b, M2a, M2b, M3a, M3b, M4a, M4b;
    if(direction == M_FORWARD) /* Forward direction */
	{
		M1a = 1;
		M1b = 0;
		M2a = 1;
		M2b = 0;
		M3a = 1;
		M3b = 0;
		M4a = 1;
		M4b = 0;
	}
	else if(direction == M_BACKGUARD) /* Backguard direction */
	{
		M1a = 0;
		M1b = 1;
		M2a = 0;
		M2b = 1;
		M3a = 0;
		M3b = 1;
		M4a = 0;
		M4b = 1;
	} 
	else   /* Release the motor */
	{
		M1a = 0;
		M1b = 0;
		M2a = 0;
		M2b = 0;
		M3a = 0;
		M3b = 0;
		M4a = 0;
		M4b = 0;
	}
	
	
	//define direction of each motor
    //Note: due to unknow reason, the order of these M motor pins may differrent from schematic info but it works for my case
    int M_ab[8];
    M_ab[0] = M3a;
    M_ab[1] = M4a;
    M_ab[2] = M3b;
    M_ab[3] = M2a;
    M_ab[4] = M1a;
    M_ab[5] = M1b;
    M_ab[6] = M2b;
    M_ab[7] = M4b;
    
    //operation: shift register to make output as desire direction
    for (int i = 0; i < 8; i++)
    {
        if (M_ab[i] == 1)
        {
            _SERIALDATA.write(1);
        }
        else 
        {
            _SERIALDATA.write(0);
        }
        _CLOCK.write(1);
        _CLOCK.write(0);
    }
    //send and run result
    _LATCH.write(1);
    _LATCH.write(0);    
}

//define speed of 4 motors in percentage in main.cpp
void Motor::setSpeed(float percentage_M1, float percentage_M2, float percentage_M3, float percentage_M4)
{
    float speedM1 = percentage_M1/100;
    float speedM2 = percentage_M2/100;
    float speedM3 = percentage_M3/100;
    float speedM4 = percentage_M4/100;
    _pwm3 = speedM3;
    _pwm4 = speedM4;
    _pwm1 = speedM1;
    _pwm2 = speedM2;
}