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

Motor m(0);
double setPoint=0.0;
double input=0.0;
double output=0.0;
//2 5
double kp = 40; //40
double ki = 15;
double Kd = 0;


double steps=0;
char reached=0;
int teclat = 0;
int printControl=0;
double velocitatActual=0;
char inputStr[100];
char outputStr[100];
char velMotorStr[100];
PID pid(&input, &output, &setPoint, kp, ki, Kd, 1 ,DIRECT); //input -> velocitat actual (sensor)
InterruptIn interrupt((PinName)PIN_SENSOR);
Ticker iVelocity;
Serial serial1(USBTX, USBRX);


void askKeyboardSpeed()
{
  serial1.printf("Introdueix velocitat a arribar:");
  scanf("%lf", &setPoint);
}

void calculateVelocity(){
  input = (steps*5/80)*PERIMETER; //31 cm/s vel max
  sprintf(inputStr, "input: %lf cm/s\n", input);
  serial1.printf(inputStr);
  /*sprintf(velMotorStr, "Steps: %lf\n", steps);
  serial1.printf(velMotorStr);*/
  steps=0;
  pid.Compute();
  m.setSpeed(output*100,0,0,0);
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
  m.Direction(M_FORWARD);
  m.setSpeed(output,0,0,0);
  
  iVelocity.attach(&calculateVelocity, 0.2);
  pid.SetMode(AUTOMATIC);
  while(1) {
    // put your main code here, to run repeatedly:
    
    
    
  }
}
