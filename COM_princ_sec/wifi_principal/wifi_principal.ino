#include <ESP8266WiFi.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

WiFiServer server(80);
IPAddress IP(192,168,4,10);
IPAddress mask = (255, 255, 255, 0);

String recebida = "teste_SIM_enviar_NVBOTAO";// mensagem - Novo botão foi apertado na esfera secundária? teste1_sim
String enviar_BOTAO = "enviar_BOTAO"; // mensagem - Qual botão foi apertado? teste1_botao1
String enviar_ACL = "enviar_ACL";
String enviar_APL = "enviar_APL";

void setup_com_principal_secundario() {
  Serial.begin(115200); 
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESTER_com_AP", "ESTER_com_comm");
  WiFi.softAPConfig(IP, IP, mask);
  server.begin();
  Serial.println();
  Serial.println("AP_1.ino");
  Serial.println("Server started.");
  Serial.print("IP: ");     Serial.println(WiFi.softAPIP());
  Serial.print("MAC:");     Serial.println(WiFi.softAPmacAddress());
}

void loop_com_principal_secundario() {
  WiFiClient client = server.available();
  if (!client) {return;}
  
  Serial.println("********************************");
  String answer11 = client.readStringUntil('\r');
  Serial.println ("Novo botao foi apertado na esfera secundária?");
  Serial.println (answer11);
    if (answer11 == "teste_SIM") {
    client.print(recebida + "\r");
      delay (3000);
      client.flush();
   
  Serial.println("********************************");
  String answer22 = client.readStringUntil('\r');
  Serial.println ("Qual botão foi apertado?");
  //Serial.println(answer22);
      if (answer22 == "QBOTAO") {
      client.print (enviar_BOTAO);
      delay (1000);
     
  String answer33 = client.readStringUntil('\r');
  Serial.println (answer33);
     }
    }
    
   Serial.println("********************************");
   String answer44 = client.readStringUntil('\r');
   Serial.println ("Acender leds?");
   Serial.println (answer44);
      if (answer44 == "ACL") {
      client.print(enviar_ACL);
      delay (1000);
     }

   Serial.println("********************************");
   String answer55 = client.readStringUntil('\r');
   Serial.println ("Apagar leds?");
   Serial.println (answer55);
      if (answer55 == "APL") {
      client.print(enviar_APL);
      delay (1000);
  }
      return;
   
  client.flush();
}

void setup()
{
  setup_com_principal_secundario();
}

void loop() {
  loop_com_principal_secundario();
}
