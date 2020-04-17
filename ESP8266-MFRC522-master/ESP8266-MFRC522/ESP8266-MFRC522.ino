/*
Many thanks to nikxha from the ESP8266 forum
*/

#include <ESP8266WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
#include <pins_arduino.h>
      #include <ESP8266WiFiMulti.h>
      #include <ESP8266HTTPClient.h>
/* wiring the MFRC522 to ESP8266 (ESP-12)
RST     = GPIO5
SDA(SS) = GPIO4 
MOSI    = GPIO13
MISO    = GPIO12
SCK     = GPIO14
GND     = GND
3.3V    = 3.3V
*/

#define RST_PIN	SCL  // RST-PIN für RC522 - RFID - SPI - Modul GPIO5 
#define SS_PIN	SS  // SDA-PIN für RC522 - RFID - SPI - Modul GPIO4 
int speaker = D2;
const char *ssid =	"dlink-77D0";	    // change according to your Network - cannot be longer than 32 characters!
const char *pass =	"gtkfw32117";	// change according to your Network
String cardnumber="";
MFRC522 mfrc522(SS_PIN, RST_PIN);	// Create MFRC522 instance
ESP8266WiFiMulti WiFiMulti;
void setup() {
  Serial.begin(9600);    // Initialize serial communications
  delay(250);

  
  SPI.begin();	         // Init SPI bus
  mfrc522.PCD_Init();    // Init MFRC522
  WiFiMulti.addAP(ssid,pass);
  WiFi.begin(ssid, pass);
  
  
  if (WiFi.status() == WL_CONNECTED) {

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
    delay(50);
    return;
  }
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();

}

// Helper routine to dump a byte array as hex values to Serial
void dump_byte_array(byte *buffer, byte bufferSize) {
HTTPClient http;
    cardnumber =String(buffer[0], HEX)+String(buffer[1], HEX)+String(buffer[2], HEX)+String(buffer[3], HEX)+String(buffer[4], HEX);
    http.begin("http://192.168.0.160/html/RFID/add_data.php?cardnumber="+cardnumber);
    Serial.println(cardnumber);
  http.end();
}
