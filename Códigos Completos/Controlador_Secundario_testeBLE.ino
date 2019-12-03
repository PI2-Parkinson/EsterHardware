#include <stdio.h>
#include <stdlib.h>

#define LEDVermelho 25
#define LEDLaranja 26
#define LEDAmarelo 14
#define LEDVerde 12
#define LEDAzul 13

#define ButtonVermelho 21
#define ButtonLaranja 19
#define ButtonAmarelo 18
#define ButtonVerde 05
#define ButtonAzul 17

//BLE Client
#include "BLEDevice.h"
#include "BLEScan.h"

char* codigo_BLE;

// The remote service we wish to connect to.
static BLEUUID serviceUUID("c96d9bcc-f3b8-442e-b634-d546e4835f64");
// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID("0fb8b636-c355-4269-9a7f-02011938cd1a");

static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    Serial.print("Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(" of data length ");
    Serial.println(length);
    Serial.print("data: ");
    Serial.println((char*)pData);
}

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
    Serial.print("onConnect");
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    Serial.println("onDisconnect");
  }
};

bool connectToServer() {
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());
    
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server.
    pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our service");


    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(charUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our characteristic");

    // Read the value of the characteristic.
    /*if(pRemoteCharacteristic->canRead()) {
      std::string value = pRemoteCharacteristic->readValue();
      Serial.print("The characteristic value was: ");
      Serial.println(value.c_str());
    }*/

    if(pRemoteCharacteristic->canNotify())
      pRemoteCharacteristic->registerForNotify(notifyCallback, false);

    connected = true;
    return true;
}
/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());
    delay(100);
    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {
      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks


int definir_botao(int escolha){
  int porta = 0;
  switch(escolha){
    case 1:
      porta = ButtonVermelho;
      break;
    case 2:
      porta = ButtonLaranja;
      break;
    case 3:
      porta = ButtonAmarelo;
      break;
    case 4:
      porta = ButtonVerde;
      break;
    case 5:
      porta = ButtonAzul;
      break; 
  }

 return porta;
}

int definir_LED(int escolha){
  int porta = 0;
  switch(escolha){
    case 1:
      porta = LEDVermelho;
      break;
    case 2:
      porta = LEDLaranja;
      break;
    case 3:
      porta = LEDAmarelo;
      break;
    case 4:
      porta = LEDVerde;
      break;
    case 5:
      porta = LEDAzul;
      break; 
  }

 return porta;
}

int comunicacao_sec(int opcao, int escolha){
    int retorno;
    switch(opcao){
    case 0:
        if(escolha == 0){
            retorno = botao_ou_led();
        }else if(escolha == 1){
            Serial.print("O jogo foi finalizado? 0 - nao e 1 - sim\n");
            ler_dado(&retorno);
        }else{
            Serial.print("Comando inválido\n");
        }
        break;
    case 1:
        retorno = enviar_botao_ap(escolha);
        break;
    case 2:
        retorno = receber_led();
        break;
    default:
        break;
    }

    return retorno;

}

int ler_botoes(){
    int i;
    int botao, apertou = 0;
    while(apertou == 0)
        for (i = 0; i < 5 ; i++){

            //LER O BOTAO
            apertou = not(digitalRead(definir_botao(i+1)));

            while(digitalRead(definir_botao(i + 1)) == 0);

            if(apertou == 1){
                botao = i+1;

                // ACENDE O LED
                acender_leds(botao,50);

                // ENVIA O DADO PARA A OUTRA ESFERA
                comunicacao_sec(1,botao);

                break;
            }
        }

    return botao;
}

void acender_leds(int LED, int tempo){
    int vetor_led[3] = {LED,tempo, 0};

    //printf("LED %d aceso\n", LED);
    digitalWrite(definir_LED(LED), HIGH);
    delay_ms(tempo);
    digitalWrite(definir_LED(LED),LOW);
}

void delay_ms(int tempo){
  delay(tempo);
}

void ler_dado(int* retorno){
  int dado_chegou = 0;
  int dado = 0;
  while(dado_chegou == 0 || dado < 48){
    if(Serial.available() > 0){
      dado_chegou = 1;
      dado = int(Serial.read());
      //Serial.println(dado);
    }
  }
  dado = dado -48;
  Serial.println(dado);
  retorno[0] = dado;
}

/**************FUNCOES COMUNICAÇÃO SECUNDÁRIA***********************/

//indica se um botão novo foi apertado 
int botao_ou_led(){

  char* cod_recebidow;
  int retorno;

    do{
      cod_recebidow = receber_codigo(2);
    }while(strcmp(cod_recebidow,"NV") != 0 && strcmp(cod_recebidow,"AL")!=0 && strcmp(cod_recebidow,"TL")!=0 && strcmp(cod_recebidow,"BL")!=0 && strcmp(cod_recebidow,"VL")!=0);
    
    
    if(strcmp(cod_recebidow,"NV") == 0)
      retorno = 0;
    else if (strcmp(cod_recebidow, "AL")== 0)
      retorno = 1;
    else if (strcmp(cod_recebidow,"TL") == 0)
      retorno = 2;
    else if (strcmp(cod_recebidow,"BL") == 0)
      retorno = 3;
    else
      retorno = 4;
  
  return retorno;
  }
  
int enviar_botao_ap(int botao){

  char* cod_recebidow ="QB";
  char codigo_a_enviar[3] = "BX";
  int retorno;
  
  cod_recebidow = receber_codigo(2);
  while(cod_recebidow[0] != 'Q'){
    enviar_codigo("XX",2);
    cod_recebidow = receber_codigo(2);
  }

  codigo_a_enviar[1] = botao + '0';
  
  enviar_codigo(codigo_a_enviar,2);
        
  return 0;
  
  }
int receber_led(){

  char* cod_recebidow;
  char codigo_enviar[3] = "QL";

  cod_recebidow = receber_codigo(2);

  while(cod_recebidow[0] != 'L'){
     enviar_codigo("RR",2);
     cod_recebidow = receber_codigo(2);
    }
  enviar_codigo("QL",2);
  
  int led = (int)cod_recebidow[1] - '0';
     
  return led;
  }

int acendertodos(){

  int tempo=0;
  char* cod_recebido;
  cod_recebido = receber_codigo(5);
  
  while(cod_recebido[0] != 'T'){
  cod_recebido = receber_codigo(5);
  }
  tempo = (cod_recebido[2] - '0') * 100 + (cod_recebido[3] - '0') * 10 + (cod_recebido[4] - '0') *1;
  return tempo; 
}

//acender quando conectar no app
int acenderalgunsleds(){

  int tempo=0;
  char* cod_recebido;
  cod_recebido = receber_codigo(5);
  
  while(cod_recebido[0] != 'B'){
  cod_recebido = receber_codigo(5);
  }
  tempo = (cod_recebido[2] - '0') * 100 + (cod_recebido[3] - '0') * 10 + (cod_recebido[4] - '0') *1;
  return tempo; 
}
  
//acender leds conectando sec
int acendersec(){

  int tempo=0;
  char* cod_recebido;
  cod_recebido = receber_codigo(5);
  
  while(cod_recebido[0] != 'V'){
  cod_recebido = receber_codigo(5);
  }
  tempo = (cod_recebido[2] - '0') * 100 + (cod_recebido[3] - '0') * 10 + (cod_recebido[4] - '0') *1;
  return tempo; 
}

//FUNÇÃO PARA ENVIAR CÓDIGO CHAR* PARA O CELULAR VIA BLE
void enviar_codigo(char* vetor, int tamanho){

  while(!pRemoteCharacteristic->canWrite());
  pRemoteCharacteristic->writeValue(vetor,tamanho+1);

  delay(5);
  Serial.print("Enviou pelo BLE: '");
  Serial.print(vetor);
  Serial.print("'\n");
}

//FUNÇÃO PARA RECEBER CÓDIGO CHAR* PARA O CELULAR VIA BLE
char* receber_codigo(int tamanho){
  std::string value = "0000";
  
  char* codigo_char = (char*)malloc((tamanho+1)*sizeof(char));
  
  while(strcmp(value.c_str(),"0000") == 0 || strcmp(value.c_str(),"") == 0){
    while(!pRemoteCharacteristic->canRead());
    value = pRemoteCharacteristic->readValue(); 
  }
  
  strcpy(codigo_char,value.c_str());
  enviar_codigo(codigo_char, tamanho);
  codigo_char[tamanho] = '\0';
  
 while(strcmp(value.c_str(),codigo_char) == 0){
    value = pRemoteCharacteristic->readValue(); 
  }

  //enviar_codigo("XX", 2);
  
  delay(5);
  Serial.print("Recebeu pelo BLE: '");
  Serial.print(codigo_char);
  Serial.print("'\n");
  
 return codigo_char;
}

/**************FUNCAO PRINCIPAL***********************/
void setup() {
  Serial.begin(115200); 
  for (int contador = 1; contador <= 5; contador++)
    pinMode(definir_LED(contador), OUTPUT);
  for (int contador = 1; contador <= 5; contador++)
    pinMode(definir_botao(contador), INPUT_PULLUP);
    
  Serial.println("Starting BLE Client application...");
  BLEDevice::init("Secundario ESTER");

  //delay(1000);
    
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
  codigo_BLE = (char*)malloc(5*sizeof(char));
  strcpy(codigo_BLE,"0000");
  Serial.println("Finalizado setup");
}

void loop() {
    int estado_atual = 0;
    int proximo_estado = 0;
    int led_ou_botao = 0; //0 - LED  1 - Botao
    int botao;
    int tempo;

  //COnectar a principal
  Serial.print("Conectar com o controlador principal... ");
  while(!connectToServer());
  do{
    enviar_codigo("S",1);
    delay(250);
  }while(strcmp(receber_codigo(1),"S") != 0);
  Serial.print("Conectado.\n");
  
  while(1){
      switch(estado_atual){
      case 0:
          led_ou_botao = comunicacao_sec(0,0);
          proximo_estado = led_ou_botao+1;
          break;
      case 1:
          botao = ler_botoes();
          //comunicacao_sec(1,botao);
          proximo_estado = 0;
          break;
      case 2:
          acender_leds(comunicacao_sec(2,0),333);
          proximo_estado = 0;
          break;
      case 3:
          tempo = acendertodos();
          digitalWrite(definir_LED(1), HIGH);
          digitalWrite(definir_LED(2), HIGH);
          digitalWrite(definir_LED(3), HIGH);
          digitalWrite(definir_LED(4), HIGH);
          digitalWrite(definir_LED(5), HIGH);
          delay(tempo);
          digitalWrite(definir_LED(1), LOW);
          digitalWrite(definir_LED(2), LOW);
          digitalWrite(definir_LED(3), LOW);
          digitalWrite(definir_LED(4), LOW);
          digitalWrite(definir_LED(5), LOW);
          proximo_estado = 0;
          break;
      case 4:
          tempo = acenderalgunsleds();
          digitalWrite(definir_LED(1), HIGH);
          digitalWrite(definir_LED(3), HIGH);
          digitalWrite(definir_LED(5), HIGH);
          delay(tempo);
          digitalWrite(definir_LED(1), LOW);
          digitalWrite(definir_LED(3), LOW);
          digitalWrite(definir_LED(5), LOW);
          digitalWrite(definir_LED(2), HIGH);
          digitalWrite(definir_LED(4), HIGH);
          delay(tempo);
          digitalWrite(definir_LED(2), LOW);
          digitalWrite(definir_LED(4), LOW);   
          proximo_estado = 0;      
          break;
      case 5:
          tempo = acendersec();
          digitalWrite(definir_LED(1), HIGH);
          delay(100);
          digitalWrite(definir_LED(1), LOW);
          digitalWrite(definir_LED(2), HIGH);
          delay(100);
          digitalWrite(definir_LED(2), LOW);
          digitalWrite(definir_LED(3), HIGH);
          delay(100);
          digitalWrite(definir_LED(3), LOW);
          digitalWrite(definir_LED(4), HIGH);
          delay(100);
          digitalWrite(definir_LED(4), LOW);
          digitalWrite(definir_LED(5), HIGH);
          delay(100);
          digitalWrite(definir_LED(5), LOW);
          digitalWrite(definir_LED(4), HIGH);
          delay(100);
          digitalWrite(definir_LED(4), LOW);
          digitalWrite(definir_LED(3), HIGH);
          delay(100);
          digitalWrite(definir_LED(3), LOW);
          digitalWrite(definir_LED(2), HIGH);
          delay(100);
          digitalWrite(definir_LED(2), LOW);
          digitalWrite(definir_LED(1), HIGH);
          delay(100);
          digitalWrite(definir_LED(1), LOW);
          proximo_estado = 0;
          break;
      default:
          break;
      }
      estado_atual = proximo_estado;
      //delay(250);
  }
}
