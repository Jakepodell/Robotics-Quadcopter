#include <Servo.h> 
 
Servo esc1;
const int arm=1000;
int speedvalue = 2000;
 
 
void setup() {
  esc1.attach(9); 
  esc1.writeMicroseconds(arm);
}
 
 
void loop() {
  esc1.writeMicroseconds(speedvalue);
  delay(1000);
} 
 
 
