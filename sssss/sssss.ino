/*
  Web client

 This sketch connects to a website (http://www.google.com)
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe, based on work by Adrian McEwen

 */
#include <TextFinder.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EEPROM.h>
// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "120.126.84.79";    // name address for Google (using DNS)

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient cardNum;
EthernetClient iD;
EthernetClient cardType;
EthernetClient cardBal;

long hits;
int x,a,b;
int v;
void setup() {
  int x = EEPROM.read(1);
  int a = EEPROM.read(2);
  int b = EEPROM.read(3);
   int z = EEPROM.read(4);
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:

  Serial.println("connecting...");

 
//      if (iD.connect("120.126.84.79", 80)) {
//    Serial.println("connected");
//    // Making HTTP request
//    iD.println("GET /RFID/id.txt ");
//    iD.println("Host: 120.126.84.79");}
//
//      if (cardBal.connect("120.126.84.79", 80)) {
//    Serial.println("connected");
//    // Making HTTP request
//    cardBal.println("GET /RFID/CardBalance.txt ");
//    cardBal.println("Host: 120.126.84.79");}
}



void loop() {
 
 
 int y = 1;


   
       if (cardNum.connect("120.126.84.79", 80)) {
    Serial.println("connected");
    // Making HTTP request
    cardNum.println("GET /RFID/CardNumber.txt ");
    cardNum.println("Host: 120.126.84.79");}
    if (cardNum.available()) {
    x++;EEPROM.write(1,x);
     char cn = cardNum.read();
     
    if(cn!='$'){
      
      
      int z = y-1;
      int Min = 9*z;
      int Max = 9*y;
      
      if(Min<x<Max){
        Serial.print(cn);
        y++;
      }
      
    }
    else if(cn=='$'){
      Serial.print("\n");
    }
    }else  {
    cardNum.stop();}
   int j =1;
    if (cardType.connect("120.126.84.79", 80)) {
    Serial.println("connected");
    // Making HTTP request
    cardType.println("GET /RFID/CardType.txt ");
    cardType.println("Host: 120.126.84.79");}
        if(cardType.available()){
      a++;EEPROM.write(2,a);
    char ct = cardType.read();
      if(ct!='$'){
      
  
int k = j-1;
           int Min = 6*k;
      int Max = 6*j;
      if(Min<a<Max){
        Serial.print(ct);
        j++;
      } }else if(ct=='$'){
      Serial.print("\n");
      
   }
        
}
}

    

  
      
      
  
 

  


