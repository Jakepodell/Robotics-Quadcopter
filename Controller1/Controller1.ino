/* 

 Adapted from code found at:
 http://arduino-info.wikispaces.com/Nrf24L01-2.4GHz-HowTo
 
 - WHAT IT DOES: 
   Reads Analog values from a potentiometer on A0, 
   maps them to a value usable by an LED,
   and transmits them over a nRF24L01 Radio Link to another transceiver.

 - CONNECTIONS: nRF24L01 Modules 
   1 - GND
   2 - VCC 3.3V !!! NOT 5V
   3 - CE to Arduino pin 9
   4 - CSN to Arduino pin 10
   5 - SCK to Arduino pin 13
   6 - MOSI to Arduino pin 11
   7 - MISO to Arduino pin 12
   8 - UNUSED
   - 
  
*/

/*-----( Import needed libraries )-----*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <DistanceSRF04.h>


/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN   9 // Used when creating the radio object below...
#define CSN_PIN 10 // Used when creating the radio object below...
#define PITCH_INPUT A0
#define ROLL_INPUT A1
#define HEIGHT_INPUT A2
#define YAW_INPUT A3
#define JOYSTICK_BUTTON 6
#define JOYSTICK_LED 5
#define PITCH_INPUT_MIN 94
#define ROLL_INPUT_MIN 450

boolean heightStable = false;
int dPitch, dRoll, dYaw, dHeight;
int oldHeightState=1;

// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe


/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio


/*-----( Declare Variables )-----*/

// Arrays kill two birds with one stone here.
// These kinds of variables requires POINTERS.  
// Why we need a pointer in this situation is beyond me right now, but apparently a workaround is using an array. 
// Fine by me.  Using an array here works nicely.  One variable, two values.
int joystick[4];
 

void setup()   
{
  Serial.begin(9600);
  radio.begin(); // Fire up the raido
  radio.openWritingPipe(pipe); // 'pipe' is the LongLong that was itialized above.
  pinMode(JOYSTICK_BUTTON, INPUT_PULLUP);
  pinMode(JOYSTICK_LED, OUTPUT);
  
}


void loop()   
{
  /*joystick[0] = 166;
  joystick[1] = analogRead(JOYSTICK_Y);
  joystick[2] = map(analogRead(JOYSTICK_X),80,1023,0,1023);
  if(joystick[2]<0)joystick[2]=0;
  
  if(digitalRead(8)>0)
    joystick[3]=1;
  else
    joystick[3]=0;
   
  if(digitalRead(7)>0)
    joystick[4]=1;
  else
    joystick[4]=0;*/   
  
  //Serial.print(radio.write(nums, sizeof(nums))); // The write() function requires the size of the thing being sent in bytes.  sizeof() plays nicely with arrays here.
  
 /* Serial.print(joystick[1]);  
  Serial.print(" , ");
  Serial.println(joystick[2]);  */
 /* if(digitalRead(JOYSTICK_BUTTON)==1){
    heightStable=true;
    Serial.println("Pressed");*/

  
  //Serial.println(digitalRead(JOYSTICK_BUTTON));
  
  dPitch = getDPitch();
  dRoll = getDRoll();
  dHeight = getDHeight();
  dYaw = getDYaw();
 
  Serial.print("PITCH: ");
  Serial.print(dPitch);
  Serial.print(" | ROLL: ");
  Serial.print(dRoll);
  Serial.print("     HEIGHT: ");
  Serial.print(dHeight);
  Serial.print(" | YAW: ");
  Serial.print(dYaw);
  Serial.print("     STABLE: ");
  if(heightStable==1)
    Serial.println("YES");
  else
    Serial.println("No");
  
  int jButtonInput = digitalRead(JOYSTICK_BUTTON);
  
  if(oldHeightState!=jButtonInput && jButtonInput==0){
    heightStable=!heightStable;
    
  }
  oldHeightState = jButtonInput;
   
  //Serial.println(heightStable);
  
 digitalWrite(JOYSTICK_LED,heightStable);
 
 
 delay(50);
  

}

int getDPitch(){
  int pitch = analogRead(PITCH_INPUT);
  if(pitch<180)pitch-=(180-pitch);
  pitch = map(pitch,0, 1023,0,100);
  return pitch;
}

int getDRoll(){
  int roll = analogRead(ROLL_INPUT);
  roll = map(roll,0,1023,0,100);
  return roll;
}

int getDHeight(){
  int height = analogRead(HEIGHT_INPUT);
  height = map(height,0,1023,0,100);
  return height;
}

int getDYaw(){
  int yaw = analogRead(YAW_INPUT);
  yaw = map(yaw,0,1023,0,100);
  return yaw;
}






