#include <ESP8266WiFi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

char ssid[] = "ESTER_com_AP";           // SSID 
char pass[] = "ESTER_com_comm";         // SENHA
String transmissao = "teste_SIM"; 
String QBOTAO = "QBOTAO"; 
String ACL = "ACL";
String APL = "APL";

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

   client.print(transmissao + "\r"); 
   delay (500);
   Serial.println("********************************");
   String answer11 = client.readStringUntil('\r');
      if (answer11 == "teste_SIM_enviar_NVBOTAO") {
      Serial.println("Secundário recebeu - Algum botão novo foi apertado?");
      String botao_selecionado = "Botão_1"; 
      Serial.println (answer11);
      client.print(QBOTAO + "\r");
      client.flush();

    Serial.println("********************************");
    String answer22 = client.readStringUntil('\r');
        if (answer22 == "enviar_BOTAO") {
        Serial.println("Secundário recebeu - Qual botão foi pressionado:");
        Serial.println (answer22);
        client.print (botao_selecionado + "\r");//
        client.flush();}
        
     client.print(ACL);//manda ACL para principal
     delay(500);
     Serial.println("********************************");
     String answer44 = client.readStringUntil('\r');
        if (answer44 == "enviar_ACL") {
          Serial.println("Secundário recebeu - Acender leds?");
          Serial.println (answer44);
          client.flush();
          }

      client.print(APL);//manda APL para principal
      delay(500);
      Serial.println("********************************");
      String answer55 = client.readStringUntil('\r');
        if (answer55 == "enviar_APL") {
          Serial.println("Secundário recebeu - Apagar leds?");
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
