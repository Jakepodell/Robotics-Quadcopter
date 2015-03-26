/*
http://techvalleyprojects.blogspot.com/2012/06/arduino-control-escmotor-tutorial.html

From ESC:
Data to pin 9
Ground to Ground

DO NOT connect power to 5V!!  
It will kill the board and the computer's USB port!!

Secure the motor.
Wear eye protection!

PROTOCOL FOR STARTING MOTORS:
No fancy programming needed.  

SET THROTTLE TO ZERO.
ATTACH MOTOR TO POWER.
WAIT FOR AUDIO SIGNAL.
APPLY THROTTLE TO START.

NOTE THAT ZERO THROTTLE == 90 DEGREES!!!

*/

// Need the Servo library
#include <Servo.h>

// These are our motors.
Servo motor1;
Servo motor2;

//USE THE MINIMUM AND MAXIMUM INPUT SPPEDS FOR THE BRUSHLESS MOTORS
int speedMin = 1500;
int speedMax = 2000;


/*-----( NUNCHUCK OPTION )-----*/

#include <Wire.h>
#include "nunchuck_funcs.h"

int loop_cnt=0;

byte joyx, joyy, accx, accy, zbut, cbut;
int joyX, joyY, accX, accY, zBut, cBut;


// Set everything up
void setup(){

  Serial.begin(9600);
  
  motor1.attach(9);
  motor2.attach(10);
  
  /*-----( NUNCHUCK OPTION )-----*/

  nunchuck_setpowerpins();
  nunchuck_init(); // send the initilization handshake
     
}


void loop(){  
  
  /*-----( NUNCHUCK OPTION )-----*/
      

        if( loop_cnt > 100 ) { // every 100 msecs get new data
        loop_cnt = 0;

        nunchuck_get_data();

        joyx = nunchuck_joyx(); //     neutral: 124...      ranges from 25 - 225 ??
        joyy = nunchuck_joyy(); //     neutral: 137,138...  ranges from 29 -232 ??
        accx  = nunchuck_accelx(); //  neutral 135+-        ranges from approx 80 - 190 (higher pitched right)
        accy  = nunchuck_accely(); //  neutral 135+-        ranges from approx 70 - 180 (higher pitched forwarrd)
        zbut = nunchuck_zbutton();
        cbut = nunchuck_cbutton(); 
        
        joyx = constrain(joyx, 30, 220);
        joyX = map(joyx, 30, 220, speedMin, speedMax);
        
        joyy = constrain(joyy, 30, 230); 
        joyY = map(joyy, 30, 230, speedMin, speedMax);
        
        //accx = constrain(accx, 110, 160); // adjust the constraint to make the nunchuck more sensitive
        accX = map(accx, 80, 190, speedMin, speedMax);
        
        //accy = constrain(accy, 80, 150);
        accY = map(accy, 70, 180, speedMin, speedMax); // adjust the constraint to make the nunchuck more sensitive
        
        
  motor1.writeMicroseconds(accX);
  motor2.writeMicroseconds(accY);
  
  Serial.print("1=");
  Serial.print("\t");
  Serial.print(joyX);
  Serial.print("\t");
  Serial.print("2= ");
  Serial.print("\t");
  Serial.println(joyY);
    
  }
        
  loop_cnt++;
  delay(1);



  delay(1);

}
