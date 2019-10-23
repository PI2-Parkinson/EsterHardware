#include <ESP8266WiFi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

char ssid[] = "ESTER_com_AP";           // SSID 
char pass[] = "ESTER_com_comm";         // SENHA

String botao_ou_led = "botao_ou_led"; 
String enviar_botao_ap = "enviar_botao_ap"; 
String enviar_led_ac = "enviar_led_ac";
String receber_led = "receber_led";

IPAddress server(192,168,4,10);     // IP address of the AP
WiFiClient client;

void setup_com_secundario_principal() {

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

void loop_com_secundario_principal() {
  client.connect(server, 80);
  client.flush();

   client.print(botao_ou_led + "\r"); 
   delay (500);
   Serial.println("********************************");
   String answer11 = client.readStringUntil('\r');
      if (answer11 == "sec_ler_botoes") {
      Serial.println("Secundário recebeu - Algum botão novo foi apertado?");
      String botao_selecionado = "botao_1"; 
      Serial.println (answer11);
      client.print(enviar_botao_ap + "\r");
      client.flush();

    Serial.println("********************************");
    String answer22 = client.readStringUntil('\r');
        if (answer22 == "sec_botao_ap") {
        Serial.println("Secundário recebeu - Qual botão foi pressionado:");
        Serial.println (answer22);
        client.print (botao_selecionado + "\r");//
        client.flush();}
        
     client.print(enviar_led_ac);//manda ACL para principal
     delay(500);
     Serial.println("********************************");
     String answer44 = client.readStringUntil('\r');
        if (answer44 == "sec_led_acender") {
          Serial.println("Secundário recebeu - Acender leds?");
          Serial.println (answer44);
          client.flush();
          }

      client.print(receber_led);//manda APL para principal
      delay(500);
      Serial.println("********************************");
      String answer55 = client.readStringUntil('\r');
        if (answer55 == "sec_led_on") {
          Serial.println("Secundário recebeu - Acender qual led?");
          Serial.println (answer55);
          client.flush();
          }
        }
        client.stop();
        return;
        delay(1000);
            } 
   
void setup()
{
  setup_com_secundario_principal();
}

void loop() 
{
  loop_com_secundario_principal();
}
