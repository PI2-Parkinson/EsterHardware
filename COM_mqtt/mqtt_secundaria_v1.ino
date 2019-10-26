#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define LED1 D0
#define LED2 D1
#define LED3 D2
#define LED4 D3
#define LED5 D4
#define BTN1 D5
#define BTN2 D6
#define BTN3 D7
#define BTN4 D8
#define BTN5 A0

// Update these with values suitable for your network.

const char* ssid = "GVT-32F3";
const char* password = "2307541086";
const char* mqtt_server = "192.168.15.10";

WiFiClient espClient;
PubSubClient client(espClient);
String msg = "";
unsigned long timeout = 0, start_time = 0;
bool btn1 = false, btn2 = false, btn3 = false, btn4 = false, btn5 = false;
bool leds = false;

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
      client.subscribe("primaria");
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
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);
  pinMode(BTN4, INPUT_PULLUP);
  pinMode(BTN5, INPUT_PULLUP);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  digitalWrite(LED5, LOW);
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
  if (btn1 != digitalRead(BTN1))
  {
    send_state = true;
    msg = "SE1.BT1." + String(!digitalRead(BTN1)) + "\n\r";
    btn1 = digitalRead(BTN1);
    Serial.print(msg);
  }
  else if (btn2 != digitalRead(BTN2))
  {
    send_state = true;
    msg = "SE1.BT2." + String(!digitalRead(BTN2)) + "\n\r";
    btn2 = digitalRead(BTN2);
    Serial.print(msg);
  }
  else if (btn3 != digitalRead(BTN3))
  {
    send_state = true;
    msg = "SE1.BT3." + String(!digitalRead(BTN3)) + "\n\r";
    btn3 = digitalRead(BTN3);
    Serial.print(msg);
  }
  else if (btn4 != digitalRead(BTN4))
  {
    send_state = true;
    msg = "SE1.BT4." + String(!digitalRead(BTN4)) + "\n\r";
    btn4 = digitalRead(BTN4);
    Serial.print(msg);
  }
  
  if (send_state)
  {
    client.publish("secundaria", msg.c_str());
    delay(250);
  }
}
