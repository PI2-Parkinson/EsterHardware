#include <ESP8266WiFi.h>

WiFiServer server(80);
IPAddress IP(192,168,4,10);
IPAddress mask = (255, 255, 255, 0);

String IT = "sim"; //mensagem - iniciar tremor 
String AETR = "AETR"; //mensagem - aviso de envio de tremor recebido
String modo_1 = "TT1", modo_2 = "TT2", modo_3 = "TT3";
int inicia = 1, modo = 1, grau;

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

int loop_com_principal_base (int inicia, int modo)
{
  WiFiClient client = server.available();
  if (!client) {return 0;}

  if (inicia == 1) {

  Serial.println ("Pedido para iniciar tremor");
  client.print(IT + "\r");  
  delay (1000);
  client.flush();
   
  Serial.println("********************************");

    String answer1 = client.readStringUntil('\r');

    Serial.println ("Resposta1: "+ answer1);

     if (answer1 == "AET") {
      Serial.println ("Principla recebeu que base esta pronta");
     // client.print(receb + "\r");
     //delay (3000);
     //client.flush();
   //String answer2 = client.readStringUntil('\r');
  //Serial.println("Resposta2: " + answer2);

     if (modo == 1) {
         Serial.println("Principal envia envia modo 1");
         client.flush();
         client.print(modo_1 + "\r");
         delay (1000);
     }
      else if (modo == 2) { 
         Serial.println("Principal envia envia modo 2");
         client.flush();
         client.print(modo_2 + "\r");
         delay (1000);
      }
      else if (modo == 3) 
         Serial.println("Principal envia envia modo 3");
         client.flush();
         client.print(modo_3 + "\r");
         delay (1000);  
       }
       else Serial.println("Falha ao enviar modo");

  

  }
   else {return 0;}
    delay (1000);
    String grau_str = client.readStringUntil('\r');

    Serial.println ("grau recebido : "+ grau_str);

     if (grau_str == "grau_1") 
          {
            grau = 1;
             Serial.println("Grau recebido da base = 1");
             return grau;
          }
          else if (grau_str = "grau_2")
          {
             grau = 2;
             Serial.println("Grau recebido da base = 2");
             return grau;
          }
          else if (grau_str = "grau_3")
          {
             grau = 3;
             Serial.println("Grau recebido da base = 3");
             return grau;   
          }
          else if (grau_str = "grau_4")
          {
             grau = 4;
             Serial.println("Grau recebido da base = 4");
             return grau;   
          }
          else 
          {
             Serial.println("falha ao receber grau");
             return 0;
          }
}
void setup()
{
  setup_com_principal_base ();
}

void loop() {
  grau = loop_com_principal_base (inicia, modo);
  
}
