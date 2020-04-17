#include <SPI.h>
#include <Ethernet.h>


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server(192,168,0,178);     

IPAddress ip(192, 168, 0, 177); 
EthernetClient client;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac, ip);
  }
  delay(1000);
  Serial.println("connecting...");
}
void loop()
{
  if (client.connect(server, 80)) {
    Serial.println("connected");
    client.println("GET /RFID/add_data.php?CardNumber=10110100&CardBalance=0&CardType=1 HTTP/1.1");
    client.println("Host: www.google.com");
    client.println("Connection: close");
    client.println();
  }
  else {
    Serial.println("connection failed");
  }
    client.stop();
}
