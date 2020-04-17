#include <ArduinoWiFi.h>
/*
  開啟瀏覽器，輸入此網址：
  http://<IP>/arduino/webserver/
  或者
  http://<hostname>.local/arduino/webserver/
 
  相關說明請參閱：
  http://labs.arduino.org/WebServer
*/
 
void setup() {
  Wifi.begin();
  Wifi.println("WebServer Server is up");  // 輸出訊息到（網頁的）WiFi控制台
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
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Refresh: 5");  // 每5秒自動更新（重新整理）網頁
  client.println();
  client.println("<html>");
  client.println("<head> <meta charset=\"utf-8\"><title>UNO WIFI物聯網</title> </head>");
  client.print("<body>");
 
  for (int analogChannel = 0; analogChannel < 4; analogChannel++) {
    int sensorReading = analogRead(analogChannel);
    client.print("類比輸入 ");
    client.print(analogChannel);
    client.print(" 的值為 ");
    client.print(sensorReading);
    client.print("<br/>");
  }
 
  client.print("</body>");
  client.println("</html>");
  client.print(DELIMITER); // 中斷與用戶的連線
}
