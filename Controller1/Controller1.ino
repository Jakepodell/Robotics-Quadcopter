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
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
int values[6];


void setup(){
  
  Serial.begin(9600);
  radio.begin(); // Fire up the raido
  radio.openWritingPipe(pipe); // 'pipe' is the LongLong that was itialized above.
  pinMode(JOYSTICK_BUTTON, INPUT_PULLUP);
  pinMode(JOYSTICK_LED, OUTPUT);
  values[0] = 5253;
  
}


void loop(){
  

  updateValues();
  printValues();
  updateHeightStable();
  
  digitalWrite(JOYSTICK_LED,heightStable);
  
  values[1]=dPitch;
  values[2]=dRoll;
  values[3]=dHeight;
  values[4]=dYaw;
  values[5]=heightStable;
  
  radio.write(values, sizeof(values));

  
  delay(5);
  
}


int getDPitch(){
  int pitch = analogRead(PITCH_INPUT);
  //if(pitch<150)pitch-=(150-pitch);
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

void printValues(){
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
}

void updateValues(){
  dPitch = getDPitch();
  dRoll = getDRoll();
  dHeight = getDHeight();
  dYaw = getDYaw();
}

void updateHeightStable(){
  int jButtonInput = digitalRead(JOYSTICK_BUTTON);
  if(oldHeightState!=jButtonInput && jButtonInput==0)
    heightStable=!heightStable;
  oldHeightState = jButtonInput;
}




