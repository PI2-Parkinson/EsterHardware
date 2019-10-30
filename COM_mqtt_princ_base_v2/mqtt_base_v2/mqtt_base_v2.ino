#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "EsTer";
const char* password = "EsTer_password";
const char* mqtt_server = "10.3.141.1";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
String msg = "";
char recebido[300];
int value = 0;
int modo;
int grau = 3;

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
  if (recebido2 == "MT1")
  {
    modo = 1;
    Serial.println(modo);
  }

  else if (recebido2 == "MT2")
  {
    modo = 2;
    Serial.println(modo);
  }
  else if (recebido2 == "MT3")
  {
    modo = 3;
    Serial.println(modo);
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

      client.subscribe("principal");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay (1000);

  if (grau == 1)
  {
    msg = "GR1\n\r";
    client.publish("base", msg.c_str());
    delay(250);
  }
  else if (grau == 2)
  {
    msg = "GR2\n\r";
    client.publish("base", msg.c_str());
    delay(250);
  }
  else if (grau == 3)
  {
    msg = "GR3\n\r";
    client.publish("base", msg.c_str());
    delay(250);
  }
}



