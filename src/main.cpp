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
double kp = 2;
double ki = 5;
double Kd = 0;
PID pid(&input, &output, &setPoint, kp, ki, Kd, 0 ,DIRECT);

int steps=0;
char reached=0;
int teclat = 0;
int printControl=0;
int timeElapsed;

Timer t;
InterruptIn interrupt((PinName)PIN_SENSOR);
Serial serial1(USBTX, USBRX);


int askKeyboardSpeed()
{
  serial1.printf("Please, set a speed for the motor (0-100):");
  int keyboardSpeed;
  scanf("%d", &keyboardSpeed);
  keyboardSpeed = (int) sqrt(keyboardSpeed*keyboardSpeed)%100;

  if(keyboardSpeed == 0) m.Direction(M_RELEASE);
  return keyboardSpeed;
}

void count(){
  steps++;
}

int main() {

  char outputStr[100];
  char inputStr[100];
  char velMotorStr[100];

  // put your setup code here, to run once:
  interrupt.enable_irq();
  interrupt.rise(&count);
  pid.SetMode(AUTOMATIC);
  m.Direction(M_FORWARD);
  m.setSpeed(100.0, 100.0, 100.0, 100.0);

  setPoint= 100;//askKeyboardSpeed();
  t.start();
  while(1) {
    // put your main code here, to run repeatedly:
    if(reached==1){
        reached=0;
        setPoint=askKeyboardSpeed();
    }

    if(steps >= 80){//Cada dos voltes perque el sensor no esta ben calibrat
                    //una volta real son dos del sensor
      t.stop();
      timeElapsed = t.read();
      input = (PERIMETER*(steps/80)/timeElapsed) *1000; //cm/s la speed
      steps=0;
      printControl++;
      if(printControl == 5){
        sprintf(outputStr, "PID: %lf \n", output);
        serial1.printf(outputStr);
        sprintf(inputStr, "Input: %lf cm/s\n", input);
        serial1.printf(inputStr);
        sprintf(velMotorStr, "Vel: %lf cm/s\n", setPoint);
        serial1.printf(velMotorStr);
        printControl=0;
      }
    }
    pid.Compute();
    
  }
}
