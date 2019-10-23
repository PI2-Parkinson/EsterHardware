#include <ESP8266WiFi.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

WiFiServer server(80);
IPAddress IP(192,168,4,10);
IPAddress mask = (255, 255, 255, 0);

String sec_ler_botoes = "sec_ler_botoes";// mensagem - Novo botão foi apertado na esfera secundária? teste1_sim
String sec_botao_ap = "sec_botao_ap"; // mensagem - Qual botão foi apertado? teste1_botao1
String sec_led_acender = "sec_led_acender";
String sec_led_on = "sec_led_on";

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
    if (answer11 == "botao_ou_led") {
    client.print(sec_ler_botoes + "\r");
      delay (3000);
      client.flush();
   
  Serial.println("********************************");
  String answer22 = client.readStringUntil('\r');
  Serial.println ("Qual botão foi apertado?");
  //Serial.println(answer22);
      if (answer22 == "enviar_botao_ap") {
      client.print (sec_botao_ap);
      delay (1000);
     
  String answer33 = client.readStringUntil('\r');
  Serial.println ("Botão apertado: " + answer33);
     }
    }
    
   Serial.println("********************************");
   String answer44 = client.readStringUntil('\r');
   Serial.println ("Acender leds?");
   Serial.println (answer44);
      if (answer44 == "enviar_led_ac") {
      client.print(sec_led_acender);
      delay (1000);
     }

   Serial.println("********************************");
   String answer55 = client.readStringUntil('\r');
   Serial.println ("Acender qual led?");
   Serial.println (answer55);
      if (answer55 == "receber_led") {
      client.print(sec_led_on);
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
