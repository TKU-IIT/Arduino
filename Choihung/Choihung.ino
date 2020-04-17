/**
 * BasicHTTPClient.ino
 *
 *  Created on: 24.05.2015
 *
 */
#define NOTE_    0
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <pins_arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>
#define RST_PIN  SCL  // RST-PIN für RC522 - RFID - SPI - Modul GPIO5 
#define SS_PIN  SS  // SDA-PIN für RC522 - RFID - SPI - Modul GPIO4 
#define SPK D2;
String cardnumber="";
byte nuidPICC[4];
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance
#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

void setup() {

    USE_SERIAL.begin(115200);
    USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFiMulti.addAP("dlink-77D0", "gtkfw32117");
  delay(250);

  
  SPI.begin();           // Init SPI bus
  mfrc522.PCD_Init();    // Init MFRC522
}
int melody[] = {
NOTE_G6, NOTE_A6,NOTE_AS6,NOTE_B6
};
int noteDurations[] = {
  9,3,3,3};
  void play(int *melody, int *noteDurations, int num){
  for(int note = 0; note < num; note++){
    int noteDuration = 300 / noteDurations[note];
    tone(D2, melody[note], noteDuration);

    delay(noteDuration);
  }
}
void loop() {

  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {

    delay(50);
    return;
  }
  // Select one of the cards
  
  if ( ! mfrc522.PICC_ReadCardSerial()) {


    USE_SERIAL.print("error");
  } 
 
    if (mfrc522.uid.uidByte[0] != nuidPICC[0] || 
    mfrc522.uid.uidByte[1] != nuidPICC[1] || 
    mfrc522.uid.uidByte[2] != nuidPICC[2] || 
    mfrc522.uid.uidByte[3] != nuidPICC[3] ){
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  play(melody, noteDurations, sizeof(melody) / sizeof(int));
      for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = mfrc522.uid.uidByte[i];
    }
    // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        
    http.begin("http://192.168.0.160/html/RFID/add_data.php?SENSORED_VALUE=1&cardnumber="+cardnumber);

        USE_SERIAL.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.println(payload);
            }
            delay(200);
  http.begin("http://192.168.0.160/html/RFID/add_data.php?SENSORED_VALUE=0");
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }}

        return;
    }
}
void dump_byte_array(byte *buffer, byte bufferSize) {
    cardnumber =String(buffer[0], HEX)+String(buffer[1], HEX)+String(buffer[2], HEX)+String(buffer[3], HEX)+String(buffer[4], HEX);

    Serial.println(cardnumber);

}

