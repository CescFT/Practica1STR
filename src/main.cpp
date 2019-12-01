#include <mbed.h>
//#include <AFMotor.h>

// Definicio de les constants
#define FULL_SPEED 255
#define STOPPED 0
#define W_DIAMETER 6.6 //cm
#define L_RODA (M_PI * W_DIAMETER) //2O.7345 cm

AF_DCMotor m(1, MOTOR12_64KHZ);
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
int iTime;
int fTime;

int sensor = 18;
Timer t;
InterruptIn interrupt;
Serial serial1(USBTX, USBRX);
int main() {

  // put your setup code here, to run once:
  interrupt.enable_irq();
  t.start();
  pid.SetMode(AUTOMATIC);
  pid.SetOutputLimits(STOPPED, FULL_SPEED);
  m.run(FORWARD);
  m.setSpeed(Output);

  while(1) {
    // put your main code here, to run repeatedly:
    
    
  }
}
