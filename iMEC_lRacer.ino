/**
* iMEC_lRacer - Simple program to control a line following robot
* @Authors - Josiah Bailey & Gabe O'Brien  
* @Version 1.0
*/

#include <Servo.h>
#include <stdio.h>

//Defines 

#define LEFT 0 
#define RIGHT 1
#define FWD 0
#define RVS 1

//Create a servo object to control the servo
Servo left_servo;
Servo right_servo; 

void setup() {
  // put your setup code here, to run once:

  left_servo.attach(9);       // attaches the servo on pin 9 to the servo object
  right_servo.attach(8);      // attaches the servo on pin 9 to the servo object
  command_zero(left_servo);   // Commands servo to its intial position
  command_zero(right_servo);   // Commands servo to its intial position
}

void loop() {
  // put your main code here, to run repeatedly:

  //First attempt to spin the servo
  command_forward(left_servo, right_servo);
  delay(500);
  command_zero(left_servo);
  command_zero(right_servo);
  delay(500);

}

//Start of functions 


//Function command_zero commands the servo to the home position
void command_zero (Servo &theServo)
{
  theServo.write(90);
}

//Function command_full commands the servo to full position 
void command_full (Servo &theServo)
{
  theServo.write(0);
}

//Function command_forward commands the servos to spin in the forward direction 

void command_forward (Servo &theLeft, Servo &theRight)
{
  //Forward is 91 to 180 - The left 
  //Forward is 89 to 0 - The right 
  command_vector(theLeft, 10, LEFT, FWD);
  command_vector(theRight, 10, RIGHT, FWD);
}

//Function command_vector commands the servo to spin forward or back ward at a percentage of maximum 
void command_vector (Servo &theServo, float percentSpeed, int leftOrright, int fwdOrrev)
{
  float commandValue = 0;

  if(leftOrright == LEFT)
  {
    if(fwdOrrev == FWD)
    {
      commandValue = 90 + (90 * (percentSpeed/100));
    }
    else
    {
      commandValue = 90 - (90 * (percentSpeed/100));
    }
  }
  else
  {
    if(fwdOrrev == FWD)
    {
      commandValue = 90 - (90 * (percentSpeed/100));
    }
    else
    {
      commandValue = 90 + (90 * (percentSpeed/100));
    }
  }

  //Write calculated command value to the servo 
  theServo.write((int)commandValue);

}

