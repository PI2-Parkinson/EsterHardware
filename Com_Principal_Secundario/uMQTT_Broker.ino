/*
   uMQTTBroker demo for Arduino

   Minimal Demo: the program simply starts a broker and waits for any client to connect.
*/

#include <ESP8266WiFi.h>
#include "uMQTTBroker.h"
int counter = 0;

uMQTTBroker myBroker;

/*
   Your WiFi config here
*/
char ssid[] = "EsTer";      // your network SSID (name)
char pass[] = "12345678"; // your network password

/*
   WiFi init stuff
*/
void startWiFiClient()
{
  Serial.println("Connecting to " + (String)ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: " + WiFi.localIP().toString());
}

void startWiFiAP()
{
  WiFi.softAP(ssid, pass);
  Serial.println("AP started");
  Serial.println("IP address: " + WiFi.softAPIP().toString());
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  // Connect to a WiFi network
  //startWiFiClient();

  // Or start the ESP as AP
  startWiFiAP();

  // Start the broker
  Serial.println("Starting MQTT broker");
  myBroker.init();
  myBroker.subscribe("#");
}

void loop()
{
  //  myBroker.publish("broker/counter", (String)counter++);
  ////  myBroker.init();
  //  // do anything here
  //  delay(1000);
  yield();
}
