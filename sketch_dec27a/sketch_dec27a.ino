#include <SoftwareSerial.h>
#include <Wire.h>
SoftwareSerial I2CBT(8,9);
byte serialA;
void setup(){

  Serial.begin(9600);
  I2CBT.begin(9600);
}
///////////////////main///////////////////

void loop (){
    byte Data[3];
    byte cmmd[20];
    int insize;
    char str[256];
    int i=analogRead(A0);//read sensor value
    serialA=I2CBT.read();
    Data[0]='a';
    Data[1]=i/256;
    Data[2]=i%256;
    sprintf(str,"i: %d 0: %d 1: %d 2: %d",i,Data[0],Data[1],Data[2]);
     Serial.println(str);
     if (serialA == 49){
         for(int j=0;j<3;j++)
         I2CBT.write(Data[j]);
         
         serialA=0;
      } 
  //delay(100);    
}
