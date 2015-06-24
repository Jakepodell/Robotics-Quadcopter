
/*-----( Import needed libraries )-----*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include<Servo.h>


/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN   9  // Used when creating the radio object below...
#define CSN_PIN 10  // Used when creating the radio object below...


// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe


/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

/*-----( Declare Variables )-----*/
int joystick[6];  

void setup()   
{
  Serial.begin(9600); 
  
  radio.begin(); // Fire up the raido
  radio.openReadingPipe(1,pipe); // 'pipe' is the LongLong that was itialized above. 
  radio.startListening();; 
}


void loop()  
{
  if (radio.available())
  {
    // Read the data payload until we've received everything
    bool done = false;
    while (!done)
    {
      // Fetch the data payload
      done = radio.read(joystick, sizeof(joystick));
      
     for(int i =0; i<6; i++){
       Serial.print(joystick[i]);
       Serial.print("  |  ");
     }
     Serial.println();
     delay(15);
    }
  }
  else
  {         
      Serial.println("No radio available");
  }

}
   
