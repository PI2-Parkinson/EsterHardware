#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define BTN1 D5
#define BTN2 D6
#define BTN3 D7
#define BTN4 D8
#define BTN5 A0

// Update these with values suitable for your network.

const char* ssid = "Tatooine 2GHZ";
const char* password = "eriajh123";
const char* mqtt_server = "192.168.0.20";

WiFiClient espClient;
PubSubClient client(espClient);
int grau;
char recebido[300];
String msg = "";
int modo = 2;
  

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  memset(recebido, 0, sizeof(recebido));
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    recebido[i] = (char)payload[i];
  }
  Serial.println();
  String recebido2 = String(recebido);
      Serial.println ("recebido2: " + recebido2);

   if (recebido2 == "GR1")
  {
    grau = 1;
    Serial.println(grau);
  }

   else if (recebido2 == "GR2")
  {
    grau = 2;
    Serial.println(grau);
  }
     else if (recebido2 == "GR3")
  {
    grau = 3;
    Serial.println(grau);
  }
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("base");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

    
    if (!client.connected()) {
    reconnect();
    }

  bool send_state = false;
 
  if (modo == 1)
  {
    send_state = true;
    msg = "MT1\n\r";

    Serial.print(msg);
  }
  else if (modo == 2)
  {
    send_state = true;
    msg = "MT2\n\r";

    Serial.print(msg);
  }
  else if (modo == 3)
  {
    send_state = true;
    msg = "MT3\n\r";

    Serial.print(msg);
  }


  client.loop();
  
  if (send_state)
  {
    client.publish("principal", msg.c_str());
    delay(250);
  }

}


