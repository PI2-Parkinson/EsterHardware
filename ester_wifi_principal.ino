#include <ESP8266WiFi.h>

WiFiServer server(80);
IPAddress IP(192,168,4,15);
IPAddress mask = (255, 255, 255, 0);

byte ledPin = 2;
int test = 77;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("Wemos_AP", "Wemos_comm");
  WiFi.softAPConfig(IP, IP, mask);
  server.begin();
  pinMode(ledPin, OUTPUT);
  Serial.println();
  //Serial.println("AP_1.ino");
  Serial.println("Server started.");
  Serial.print("IP: ");     Serial.println(WiFi.softAPIP());
  Serial.print("MAC:");     Serial.println(WiFi.softAPmacAddress());
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {return;}
  digitalWrite(ledPin, LOW);
  String request = client.readStringUntil('\r');//recebe mensagem do servidor
  //Serial.println("********************************");
  Serial.println("Teste_principal: " + request);
  client.flush();
  //Serial.print("Byte sent to the station: ");
// Serial.println(client.println(request + "ca" + "\r"));
  Serial.println(client.println("Hi, friend " + (String)test));//envia mensagem pro servidor
  digitalWrite(ledPin, HIGH);
}
