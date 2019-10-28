#include <ESP8266WiFi.h>
#include <PubSubClient.h>


const char* ssid = "nome";
const char* password = "senha";
const char* mqtt_server = "ip";

WiFiClient espClient;
PubSubClient client(espClient);
String msg = "";
String MT = "MT4";
unsigned long timeout = 0, start_time = 0;

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

void callback(char* topic, byte* payload, unsigned int length) {
  String line;
/*
  //obtem a string do payload recebido
  for (int i = 0; i < length; i++)
  {
    char c = (char)payload[i];
    line += c;
  }
  if (line.length() > 0)
  {
    if (line.indexOf("SE") >= 0)
    {
      int SE = line.substring(line.indexOf("SE") + 2).toInt();
      int LD = line.substring(line.indexOf("LD") + 2).toInt();
      int TM = line.substring(line.indexOf("LD") + 4).toInt();
      Serial.println("Semi esfera: " + String(SE));
      Serial.println("Led: " + String(LD));
      Serial.println("Tempo: " + String(TM));
      timeout = TM;
      start_time = millis();
      leds = true;
      if (LD == 1)
      {
        digitalWrite(LED1, HIGH);
      }
      else if (LD == 2)
      {
        digitalWrite(LED2, HIGH);
      }
      else if (LD == 3)
      {
        digitalWrite(LED3, HIGH);
      }
      else if (LD == 4)
      {
        digitalWrite(LED4, HIGH);
      }
      else if (LD == 5)
      {
        digitalWrite(LED5, HIGH);
      }
    }
  } */
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

  bool send_state = false;
  
  if (MT == "MT1")
  {
    send_state = true;
    msg = MT + "\n\r";
    Serial.println(msg);
  }
  else if (MT == "MT2")
  {
    send_state = true;
    msg = MT + "\n\r";
    Serial.println(msg);
  }
  else if (MT == "MT3")
  {
    send_state = true;
    msg = MT + "\n\r";
    Serial.println(msg);
  }
  else if (MT == "MT4")
  {
    
    msg = MT + "\n\r";
    send_state = true;
    
    Serial.println(msg);
  }
  else 
  {
    Serial.println(msg);
    Serial.println("Errou ao receber modo de tremor");
  }
  
  if (send_state)
  {
    Serial.println("principal enviou: " + msg);
    client.publish("principal: ", msg.c_str());
    delay(1000);
  }
}
