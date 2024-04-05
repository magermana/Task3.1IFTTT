#include <WiFiNINA.h>
#include "secrets.h"

#include <BH1750.h>
#include <Wire.h>


//please enter your sensitive data in the Secret tab
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
String state = "test";
WiFiClient client;
BH1750 lightMeter; 

//https://maker.ifttt.com/trigger/high_light/with/key/bez5X0LrjseKF-dCUW2Qqi

char   HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME   = "/trigger/high_light/with/key/bez5X0LrjseKF-dCUW2Qqi"; // change your EVENT-NAME and YOUR-KEY
//String Json = "{"test_value": "i_like_cheese"}";
float former_average;
float average;
void setup() {
  // initialize WiFi connection
  WiFi.begin(ssid, pass);

  Serial.begin(9600);
  while (!Serial);
  Wire.begin();
  lightMeter.begin();

  former_average = lightMeter.readLightLevel();
  average = lightMeter.readLightLevel();
  

  // connect to web server on port 80:

}

void loop() {
  
  delay(500);
  former_average = average;
  average = 0;
  for(int i = 0; i < 10; i++)
  {
    average = average + lightMeter.readLightLevel();
    delay(50);
  }
  average = average/10;
  Serial.print("Average Light last 10 measurments: ");
  Serial.println(average);
  bool condition1 = ((average > 100) && (former_average <= 100));
  bool condition2 = ((average <= 100) && (former_average > 100));
  String query_append;
  if(condition1) query_append = String("BRIGHT");
  else if(condition2) query_append = String("DARK");
  String queryString = String("?value1=") + String(average) + String("&value2=") + String(query_append);


  if(condition1 || condition2)
  {

  
  
    if (client.connect(HOST_NAME, 80)) {
    // if connected:
    Serial.println("Connected to server");
  }
  else {// if not connected:
    Serial.println("connection failed");
  }
  

    // make a HTTP request:
    // send HTTP header
  client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
  client.println("Host: " + String(HOST_NAME));
  client.println("Connection: close");
  client.println(); // end HTTP header


    while (client.connected()) {
      if (client.available()) {
        // read an incoming byte from the server and print it to serial monitor:
        char c = client.read();
        Serial.print(c);
      }
    }

    // the server's disconnected, stop the client:
    client.stop();
    Serial.println();
    Serial.println("disconnected");
  }
}

