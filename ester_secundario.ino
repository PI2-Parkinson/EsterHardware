#define rezet D5  // D5 is more ok than D0
#include <ESP8266WiFi.h>
/*
extern "C" {
  #include "user_interface.h"
}
*/
byte ledPin = 2;
char ssid[] = "Wemos_AP";           // SSID of your AP
char pass[] = "Wemos_comm";         // password of your AP
String teste = "88";

IPAddress server(192,168,4,10);     // IP address of the AP
WiFiClient client;


void setup() {

  Serial.begin(115200);
  delay(500);
  WiFi.disconnect();
  delay(500);
  WiFi.mode(WIFI_STA);
   delay(500);
  WiFi.begin(ssid, pass);           // connects to the WiFi AP
  Serial.println();
  Serial.println("Connection to the AP");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Connected");
  Serial.println("ST_1.ino");
  Serial.print("LocalIP:"); Serial.println(WiFi.localIP());
  Serial.println("MAC:" + WiFi.macAddress());
  Serial.print("Gateway:"); Serial.println(WiFi.gatewayIP());
  Serial.print("AP MAC:"); Serial.println(WiFi.BSSIDstr());

}

void loop() {
  client.connect(server, 80);

  Serial.println("********************************");

  client.print(teste + "\r");
  
  String answer = client.readStringUntil('\r');
  
  Serial.println("Secundário - Recebeu da principal: " + answer);
  
  client.flush();
 
  client.stop();

  delay(1000);
 
if (WiFi.status() != WL_CONNECTED) 
{
  Serial.println("disconect from wifi - reset");}

}
