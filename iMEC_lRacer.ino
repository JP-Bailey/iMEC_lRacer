/**
* iMEC_lRacer - Simple program to control a line following robot
* @Authors - Josiah Bailey & Gabe O'Brien  
* @Version 1.0
*/

#include <Servo.h>
#include <stdio.h>
#include "Adafruit_TCS34725.h"

//Defines

#define LEFT 0
#define RIGHT 1
#define FWD 0
#define RVS 1

#define RED 0
#define GREEN 1
#define BLUE 3
#define YELLOW 4

//Create a servo object to control the servo
Servo left_servo;
Servo right_servo;

/* Initialise with default values (int time = 2.4ms, gain = 1x) */
Adafruit_TCS34725 tcs = Adafruit_TCS34725();

// Color Struct
struct color {
  float R;
  float G;
  float B;
  float C;
};


//global(s)
color red_tape;
color green_tape;
color yellow_tape;
color measured;
float right_control = 0;
float left_control = 0;


void setup() {
  // put your setup code here, to run once:

  //Setup Servos---------------------------------------------------------------- 
  left_servo.attach(9);       // attaches the servo on pin 9 to the servo object
  right_servo.attach(8);      // attaches the servo on pin 8 to the servo object
  
  //Stucts to store the color values--------------------------------------------

  //Assign Values to the red tape struct

  red_tape.R = 271;
  red_tape.G = 118;
  red_tape.B = 79;
  red_tape.C = 476;

  //Assign values to the green tape struct

  green_tape.R = 105;
  green_tape.G = 152;
  green_tape.B = 80;
  green_tape.C = 348;

  //Assign values to the yellp tape struct

  yellow_tape.R = 435;
  yellow_tape.G = 343;
  yellow_tape.B = 118;
  yellow_tape.C = 926;

  //Assign intial value to measured

  measured.R = 0;
  measured.G = 0;
  measured.B = 0;
  measured.C = 0;

  // Start Color Sensor
  tcs.begin();
  tcs.setIntegrationTime(TCS34725_INTEGRATIONTIME_154MS);

}

void loop() {
  // put your main code here, to run repeatedly:

  //2. Read Values from color sensor ----------------------------------


  //3. Calculate distance to each color.  


  //1. Control Servos 

  delay(5);
}

//Start of functions


//Function command_zero commands the servo to the home position
void command_zero(Servo &theServo) {
  theServo.write(90);
}

//Function command_full commands the servo to full position
void command_full(Servo &theServo) {
  theServo.write(0);
}

//Function command_forward commands the servos to spin in the forward direction

void command_forward(Servo &theLeft, Servo &theRight) {
  //Forward is 91 to 180 - The left
  //Forward is 89 to 0 - The right
  command_vector(theLeft, 10, LEFT, FWD);
  command_vector(theRight, 10, RIGHT, FWD);
}

//Function command_vector commands the servo to spin forward or back ward at a percentage of maximum
void command_vector(Servo &theServo, float percentSpeed, int leftOrright, int fwdOrrev) {
  float commandValue = 0;

  if (leftOrright == LEFT) {
    if (fwdOrrev == FWD) {
      commandValue = 90 + (90 * (percentSpeed / 100));
    } else {
      commandValue = 90 - (90 * (percentSpeed / 100));
    }
  } else {
    if (fwdOrrev == FWD) {
      commandValue = 90 - (90 * (percentSpeed / 100));
    } else {
      commandValue = 90 + (90 * (percentSpeed / 100));
    }
  }

  //Write calculated command value to the servo
  theServo.write((int)commandValue);
}

float color_distance(color &theTarget, color &theMeasured) {
  float distance = 0.0;

  //Uses 4D distnace formula

  distance = sqrt(pow((theTarget.R - theMeasured.R), 2) + pow((theTarget.G - theMeasured.G), 2) + pow((theTarget.B - theMeasured.B), 2) + pow((theTarget.C - theMeasured.C), 2));

  return distance;
}

//simple control using what is the closest color
void simple_control(float distanceRed, float distanceGreen, float distanceYellow, float left_control, Servo &left_servo, float right_control, Servo &right_servo) {
  
    int closest_color;
    int change = 10;
    //Determine which color is closest 
    closest_color = GREEN;
    if(distanceRed < distanceGreen && distanceRed < distanceYellow)
    {
      closest_color = RED;
    }
    else if(distanceYellow < distanceGreen && distanceYellow < distanceRed)
    {
      closest_color = YELLOW;
    }
    
    //Stay the corse if green, turn right if red, and turn left if yellow 

    if (closest_color == GREEN)
    {
      //move forward 
      command_forward(left_servo, right_servo);
      delay(250);
      command_zero(left_servo);   // Commands servo to its intial position
      command_zero(right_servo);  // Commands servo to its intial position
    }
    else if(closest_color == RED)
    {
      //turn right - left servo forward right servo reverse 
      command_vector(left_servo, 10, LEFT, FWD);
      command_vector(right_servo, 10, RIGHT, RVS);
      delay(250);
      command_zero(left_servo);   // Commands servo to its intial position
      command_zero(right_servo);  // Commands servo to its intial position
    }
    else
    {
       //turn left - left servo reverse right servo forward 
      command_vector(right_servo, 10, RIGHT, FWD);
      command_vector(left_servo, 10, LEFT, RVS);
      delay(250);
      command_zero(left_servo);   // Commands servo to its intial position
      command_zero(right_servo);  // Commands servo to its intial position
    }

    //Update Values 

      
      
   
  }

  // Start of debug messages
  void db_print_color(int r, int g, int b, int c, float cd) {
    Serial.begin(9600);
    Serial.print("R: ");
    Serial.print(r, DEC);
    Serial.print(" ");
    Serial.print("G: ");
    Serial.print(g, DEC);
    Serial.print(" ");
    Serial.print("B: ");
    Serial.print(b, DEC);
    Serial.print(" ");
    Serial.print("C: ");
    Serial.print(c, DEC);
    Serial.print(" ");
    Serial.print("Color Distance: ");
    Serial.print(cd, DEC);
    Serial.print(" ");
    Serial.println(" ");
  }

  void db_print_color_distance(float cd_red, float cd_green, float cd_yellow)
  {
    //Debug
    Serial.begin(9600);
    Serial.print("Color Distance red: ");
    Serial.print(cd_red, DEC);
    Serial.print(" ");
    Serial.print("Color Distance green: ");
    Serial.print(cd_green, DEC);
    Serial.print(" ");
    Serial.print("Color Distance yellow: ");
    Serial.print(cd_yellow, DEC);
    Serial.print(" ");
    Serial.println(" ");
    delay(1000); 
    Serial.end();
  }

