#include <ESP8266WiFi.h>

WiFiServer server(80);
IPAddress IP(192,168,4,10);
IPAddress mask = (255, 255, 255, 0);

String receb = "ITR"; //mensagem - iniciar tremor recebida
String AETR = "AETR"; //mensagem - aviso de envio de tremor recebido

void setup_com_principal_base ()
{
    Serial.begin(115200); 
  WiFi.mode(WIFI_AP);
  WiFi.softAP("Ester_AP", "Ester_comm");
  WiFi.softAPConfig(IP, IP, mask);
  server.begin();
 
  Serial.println();
  Serial.println("AP_1.ino");
  Serial.println("Server started.");
  Serial.print("IP: ");     Serial.println(WiFi.softAPIP());
  Serial.print("MAC:");     Serial.println(WiFi.softAPmacAddress());
}

void loop_com_principal_base ()
{
  WiFiClient client = server.available();
  if (!client) {return;}
   
  Serial.println("********************************");

    String answer1 = client.readStringUntil('\r');

    Serial.println ("answer1: "+ answer1);

     if (answer1 == "ITT") {
      Serial.println ("Pedido para iniciar tremor");
      client.print(receb + "\r");
   
      delay (3000);
   client.flush();
   String answer2 = client.readStringUntil('\r');
  Serial.println("answer2: " + answer2);

  if (answer2 == "AETT") {
    Serial.println ("Recebeu da base aviso de envio de tremor");
    client.print (AETR);
    delay (1000);
    String answer3 = client.readStringUntil('\r');
    Serial.println ("tremor recebido: " + answer3);
     }
     
   else {
    Serial.println("Nao recebeu mais mensagens");
    return;
    }     
}
   else return;
   
  client.flush();
}

void setup()
{
  setup_com_principal_base ();
}

void loop() {
  loop_com_principal_base ();
}
