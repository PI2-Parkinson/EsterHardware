#include <MPU6050_tockn.h>
#include <Wire.h>
//#include <WiFi.h>

MPU6050 mpu6050(Wire);

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define pi 3.14159265359

//BLE Client
#include "BLEDevice.h"
#include "BLEScan.h"

char* codigo_BLE;

// The remote service we wish to connect to.
static BLEUUID serviceUUID("c96d9bcc-f3b8-442e-b634-d546e4835f64");
// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID("44c6fe06-3860-4d90-903d-f665ec523e7a");

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
    delay(100);
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {
      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks

/************************************************************************
*         FUNÇÕES DE LEITURA E PROCESSAMENTO DOS DADOS
*************************************************************************/
float** ler_sensor( int quantidade){

    float** leitura_sensor;
    int i;
    //int amplitude = 3, offset = 70;

    leitura_sensor = (float**)malloc(3*sizeof(float*));
    
    leitura_sensor[0] = (float*) malloc(quantidade*sizeof(float));
    leitura_sensor[1] = (float*) malloc(quantidade*sizeof(float));
    leitura_sensor[2] = (float*) malloc(quantidade*sizeof(float));
    for( i = 0; i < 400; i++){
      mpu6050.update();
      delay(25);
    }
    //delay(14500);
    Serial.println("Comecar a medida");
    //delay(500);
    for(i = 0; i < quantidade; i++){
      mpu6050.update();
      leitura_sensor[0][i] = mpu6050.getAngleX();
      leitura_sensor[1][i] = mpu6050.getAngleY();
      leitura_sensor[2][i] = mpu6050.getAngleZ();

      Serial.print(i);
      Serial.print(" | angulo X : ");
      Serial.println(leitura_sensor[0][i]);
      
      delay(25); 
    }

    return leitura_sensor;

}


double* fft_emC(float* dados,int n){

    //double* tempo=new double[n];//armazena as 200 amostras igualmente espaçadas

    double tfr[n]; // armazena a parte real do sinal
    double tfi[n]; // armazena a parte imaginária do sinal
    double* tf_abs;

    tf_abs = (double*)malloc((n/2+1)*sizeof(double));

    for(int u=0;u<(n/2)+1;u++) {
        tfr[u]=0;
        tfi[u]=0;
        for(int x=0;x<n;x++){
            tfr[u]+=(double)dados[x]*cos(6.2831853*u*(double)x/(double)n);
            tfi[u]-=(double)dados[x]*sin(6.2831853*u* (double)x/(double)n);
        }
        tfr[u]/=(double)n;
        tfi[u]/=(double)n;

        tf_abs[u] = sqrt(pow(tfr[u],2)+pow(tfi[u],2));
        //printf("%d |   %.5lf  %.5lf\n",u,tfr[u],tfi[u]);
    }

    /*for(int x=0;x<(n/2+1);x++){
        tf_abs[x] = sqrt(pow(tfr[x],2)+pow(tfi[x],2));
        Serial.print(x);
        Serial.print(" | FFT : ");
        Serial.println(tf_abs[x]);
    }*/

    return tf_abs;
}

int grau_tremor(int modo, double** fft_dado, int tamanho){

    float alfa, beta;
    double amplitude[3] = {0,0,0};
    double amplitude_total = 0;
    int grau=0;

    switch(modo){
    case 1: //Descanso
        alfa = 0.588;
        beta = -3.02;
        break;
    case 2: //Postura
        alfa = 0.355;
        beta = -2.74;
        break;
    case 3: //Nariz
        alfa = 0.577;
        beta = -2.24;
        break;
    }

    for(int j = 0; j < 3; j++){
      for(int i = tamanho*0.1; i < tamanho; i++)
          if(amplitude[j]<fft_dado[j][i])
              amplitude[j] = fft_dado[j][i];
      amplitude[j] = 2*amplitude[j]; // em Graus
      amplitude[j] = 0.5*sqrt(2*(1-cos(amplitude[j]/180*pi)));
      Serial.print("amplitude real * : ");
      Serial.println(amplitude[j]);
    }

    amplitude_total = sqrt(pow(amplitude[0],2)+pow(amplitude[1],2)+pow(amplitude[2],2));
    amplitude_total = amplitude_total - 0.009;
    if(amplitude_total < 0)
      amplitude_total = 0.0001;
    grau = (int)((log10(amplitude_total)-beta)/alfa);

    Serial.print("amplitude real : ");
    Serial.println(amplitude_total);
    Serial.print("Grau real: ");
    Serial.print(((log10(amplitude_total)-beta)/alfa));
    
    if(grau < 0){
      grau = 0;
    }else if(grau > 4){
      grau = 4;
    }
    
    return grau;
}

int comunicacao_strap(int modo, int dado){
    int retorno;
    switch(modo){
    case 0: //Foi pedido leitura?
        retorno = leitura();
        break;
    case 1: //Qual modo?
        retorno = modo_de_medicao();
        break;
    case 2:
        enviar_grau(dado);
        break;
    default:
        Serial.print("Comando invalido\n");
        break;
    }

    return retorno;
}


/*************************************************************
 *                  FUNÇÕES DO WIFI
 *************************************************************/
 
int leitura(){ //0 - não será feito leitura e 1 sim
  char codigo_a_enviar[4] = "AET";
  char* codigo_recebido; 
  int retorno;
  
  do{
    codigo_recebido = receber_codigo(2);
  }while(strcmp(codigo_recebido,"IT")!=0);

  enviar_codigo("AET",3);

  return 1;  
}
int modo_de_medicao(){ // retorna o modo de operação
  char* codigo_recebido; 
  int retorno;
  
  do{
    codigo_recebido = receber_codigo(3);
  }while(codigo_recebido[0] != 'T');

  retorno = (int)codigo_recebido[2] - '0';

  return retorno;  
}

int enviar_grau(int grau){ //envia o grau de tremor
  char codigo_a_enviar[3] = "GX";

  codigo_a_enviar[1] = grau + '0';
  
  enviar_codigo(codigo_a_enviar,2);

  return 0;  
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

/***************************************************************/

void setup() {
  Serial.begin(115200);
  
  Wire.begin(21,22);
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  //BLE Client
  Serial.println("Starting BLE Client application...");
  BLEDevice::init("Base ESTER");

  delay(1000);
    
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
  int quant_dados = 400, grau, modo;
  float** dados_sensor;
  float*dados_sensor_crr;
  double* fft_dados_sensor[3];
  
  //COnectar a principal
  Serial.print("Conectar com o controlador principal... ");
  while(!connectToServer());
  do{
    enviar_codigo("B",1);
    delay(250);
  }while(strcmp(receber_codigo(1),"B") != 0);
  Serial.print("Conectado.\n");
  
  while(1){
      if(comunicacao_strap(0,0) == 1){ 
          dados_sensor = ler_sensor(quant_dados);
          //dados_sensor_crr = ajuste_media(dados_sensor, quant_dados);
          fft_dados_sensor[0] = fft_emC(dados_sensor[0], quant_dados);
          fft_dados_sensor[1] = fft_emC(dados_sensor[1], quant_dados);
          fft_dados_sensor[2] = fft_emC(dados_sensor[2], quant_dados);
          modo = comunicacao_strap(1,0); 
          
          grau = grau_tremor(modo,fft_dados_sensor,quant_dados/2+1);
          comunicacao_strap(2,grau);

          
      }
      delay(100);
  }
}
