
//
// Read Yahoo Weather API XML
// 03.09.2012
// http://arduino-praxis.ch


#include <SPI.h>
#include <Ethernet.h>
#include <TextFinder.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xAD };
byte ip[] = { 192, 168, 1, 102 };
byte gateway[] = { 192, 168, 1, 1 };
byte subnet[] = { 255, 255, 255, 0 };

// Server Yahoo
IPAddress server(87,248,122,181);

EthernetClient client;
TextFinder  finder( client );  

char place[50];
char hum[30];


void setup()
{
  // Start Ehternet
  Ethernet.begin(mac, ip);
  // Start Serial Port
  Serial.begin(9600);
  Serial.println("Setup...");
}


void loop()
{
  if (client.connect(server, 80))
  {
    // Call Wetter-API
    // w: ID from your City
    // http://weather.yahooapis.com/forecastrss?w=12893459&u=c
    ///
    Serial.println("Connect to Yahoo Weather...");
    client.println("GET /forecastrss?w=12893459&u=c HTTP/1.0");
    client.println("HOST:weather.yahooapis.com\n\n");
    client.println();
    Serial.println("Connected...");
  } 
  else
  {
    Serial.println(" connection failed");
  } 


  if (client.connected())
  {
    
    // Humidity
   if ( (finder.getString("<yweather:atmosphere humidity=\"", "\"",hum,4)!=0) )
   {
     Serial.print("Humidity:  ");
     Serial.println(hum);
   } 
   else
   {
     Serial.print("No Humidity Data");
   }
    
    
    // Place/City
    if ( (finder.getString("<title>Conditions for ", " ",place,50)!=0) )
    {
      Serial.print("City:  ");
      Serial.println(place);
    }
    
    
    // Temperature
    if(finder.find("temp=") )
    {
     int temperature = finder.getValue();
     Serial.print("Temp C:  ");
     Serial.println(temperature);
   }
   else
   {
     Serial.print("No Temperature Data");
   }
   
         
  // END XML
  }
  else
  {
    Serial.println("Disconnected"); 
  }

  client.stop();
  client.flush();
  delay(60000); 
}

