#include <SoftwareSerial.h>
#include <Wire.h>
SoftwareSerial I2CBT(8,9);
byte serialA;
void setup(){

  Serial.begin(9600);
  I2CBT.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
serialA=I2CBT.read();
    if (serialA == 49) {

      
I2CBT.print("1000099999");
      serialA = 0;
    }
}
