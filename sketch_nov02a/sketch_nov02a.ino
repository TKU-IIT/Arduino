#include <Wire.h>
#include <LCD5110_Basic.h>
#include <ArduinoWiFi.h>
/*
  開啟瀏覽器，輸入此網址：
  http://<IP>/arduino/webserver/
  或者
  http://<hostname>.local/arduino/webserver/

  相關說明請參閱：
  http://labs.arduino.org/WebServer
*/
LCD5110 myGLCD(4,3,2,1,0);
extern uint8_t SmallFont[];
void setup() {
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  Wifi.begin();
  myGLCD.print("WebServer Serv", LEFT, 0);
  myGLCD.print("er is up", LEFT, 12);  // 輸出訊息到（網頁的）WiFi控制台
}

void loop() {
  while (Wifi.available()) {
    process(Wifi);
  }
  delay(50);
}
void process(WifiData client) {
  // 讀取命令
  String command = client.readStringUntil('/');

  if (command == "webserver") {
    WebServer(client);
  }
}
void WebServer(WifiData client) {

}
