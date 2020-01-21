#include <mbed.h>
#include "DCMotor.h"
#include "PID.h"
#include <math.h>
#include <stdio.h>

// Definicio de les constants
#define MAX_SPEED 255
#define MIN_SPEED 0
#define DIRECT 0
#define DIAMETER 6.6 //cm
#define PERIMETER 20.7345 //2O.7345 cm
#define PIN_SENSOR PG_0

Motor motor(0);
double setPoint=0.0;
double input=0.0;
double tunning=0.0;
//2 5
double kp = 0.15; //40
double ki = 0.85;
double Kd = 0;


double steps=0;
char inputStr[100];
char tunningString[100];
PID pid(&input, &tunning, &setPoint, kp, ki, Kd, 1 ,DIRECT); //input -> velocitat actual (sensor)
InterruptIn interrupt((PinName)PIN_SENSOR);
Ticker interruptVel;
Serial serial1(USBTX, USBRX);


void askKeyboardSpeed(){
  serial1.printf("Introdueix velocitat a arribar:");
  scanf("%lf", &setPoint);
}

void calculateVelocity(){
  input = (steps*5/80)*PERIMETER; //31 cm/s vel max
  sprintf(inputStr, "Velocitat: %lf cm/s\t", input);
  serial1.printf(inputStr);
  
  sprintf(tunningString, "Output: %lf\n", tunning);
  serial1.printf(tunningString);
  steps=0;
  pid.Compute();
  motor.setSpeed(tunning*100,0,0,0);
}

void count(){
  steps++;
}

int main() {

  // put your setup code here, to run once:
  interrupt.enable_irq();
  interrupt.rise(&count);
  askKeyboardSpeed();
  //setPoint=100;
  motor.Direction(M_FORWARD);
  motor.setSpeed(tunning,0,0,0);
  
  interruptVel.attach(&calculateVelocity, 0.2);
  pid.SetMode(AUTOMATIC);
  while(1) {   
    pid.Compute();
  }
}
