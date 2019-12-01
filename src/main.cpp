#include <mbed.h>
#include "DCMotor.h"
#include "PID.h"
#include <math.h>

// Definicio de les constants
#define FULL_SPEED 255
#define STOPPED 0
#define W_DIAMETER 6.6 //cm
#define L_RODA (M_PI * W_DIAMETER) //2O.7345 cm

Motor m(1, MOTOR12_64KHZ);
double motorSpeed=0.0; //Setpoint
double Input=0.0;
double Output=0.0;
//2 5
double Kp = 2;
double Ki = 5;
double Kd = 0;
PID pid(&Input, &Output, &motorSpeed, Kp, Ki, Kd, DIRECT);

volatile int num_sensor=0;

int teclat = 0;
int contador = 0;
int control_contador=0;
int TotalTime;


int sensor = 18;
Timer t;
InterruptIn interrupt;
Serial serial1(USBTX, USBRX);


void count(){
  num_sensor++;
}


int main() {

  // put your setup code here, to run once:
  interrupt.enable_irq();
  interrupt.rise(&count);
  t.start();
  pid.SetMode(AUTOMATIC);
  pid.SetOutputLimits(STOPPED, FULL_SPEED);
  m.run(M_FORWARD);
  m.setSpeed(Output);
  char output[100];
  char input[100];
  char velMotor[100];
  char numsensor[100];

  while(1) {
    // put your main code here, to run repeatedly:
    serial1.printf("Introduex la velocitat a la que vols que arribi el motor:");
    char  vel = serial1.getc();
    const char * str;
    sscanf(str, "%d", &vel);
    int velocitat = atoi(str);
    while(velocitat > FULL_SPEED || velocitat <= STOPPED){
      serial1.printf("Velocitat fora de rangs o es zero, introduex una velocitat correcte.");
      vel = serial1.getc();
      sscanf(str, "%d", &vel);
      velocitat = atoi(str);
      if(velocitat == 0)
        m.run(M_RELEASE);
    }
    
    motorSpeed=velocitat;
    contador++;

    if(num_sensor >= 80){
      t.stop();
      TotalTime = t.read();
      Input = (L_RODA /TotalTime) *1000; //cm/s la velocitat
      num_sensor=0;
      control_contador++;
      if(control_contador == 8){
        serial1.printf("PID: ");
        sprintf(output, "%lf \n", Output);
        serial1.printf(output);
        serial1.printf("-----------------");
        sprintf(input, "%lf\n", Input);
        serial1.printf(input);
        serial1.printf("cm/s");
        sprintf(velMotor, "%lf\n", motorSpeed);
        serial1.printf(velMotor);
        serial1.printf("cm/s");
        serial1.printf("Num sensor:");
        sprinf(numsensor, "%d\n", num_sensor);
        serial1.printf(numsensor);
        control_contador=0;
      }
    }
    pid.Compute();
    m.setSpeed(Output);
  }
}
