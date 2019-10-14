#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
//bool deviceConnected = false;
//bool oldDeviceConnected = false;

char* codigo_BLE;

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();
      
      strcpy(codigo_BLE,value.c_str());
      //Serial.println("Salvou o dado");
    }
};


void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  BLEDevice::init("ESTER"); //Nome do BLE
  pServer = BLEDevice::createServer(); //Criando Servo
  BLEService *pService = pServer->createService(SERVICE_UUID); //Criando serviço
  pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                        BLECharacteristic::PROPERTY_READ   |
                                        BLECharacteristic::PROPERTY_WRITE  
                                       );
                                       
  //pCharacteristic->addDescriptor(new BLE2902());
  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->setValue("C");
  pService->start();
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
  
  codigo_BLE = (char*)malloc(34*sizeof(char));
  strcpy(codigo_BLE,"0000");

  /*****************************CODIGO MAIN ************************************/

  int modo = 0;
  char* cod_recebido;
  int seq_aleatoria[11] = {1,4,3,5,2,2,1,5,1,4,2};
  Serial.println("Vai receber C");
  
  cod_recebido = receber_codigo(1);
  Serial.print("Código recebido : '");
  Serial.print(cod_recebido);
  Serial.print("'\n");
  
  while(1){
    Serial.println("Qual função?");
    Serial.println("1 - definir_modo_op");
    Serial.println("2 - botao_parar_apertado");
    Serial.println("3 - qual_nivel");
    Serial.println("4 - enviar_nivel");
    Serial.println("5 - enviar_sequencia");
    Serial.println("6 - finalizou_sequencia");
    Serial.println("7 - exercicio3_finalizado");
    Serial.println("8 - enviar_grau_tremor");
    Serial.println("9 - errou_sequencia");
    while(Serial.available() == 0);
    modo = Serial.read() - '0';
    switch(modo){
      case 1:
        Serial.println(definir_modo_op());
        break;
      case 2:
        Serial.println(botao_parar_apertado());
        break;
      case 3:
        Serial.println(qual_nivel(2));
        break;
      case 4:
        Serial.println(enviar_nivel(7,2));
        break;
      case 5:
        Serial.println(enviar_sequencia(11,seq_aleatoria));
        Serial.println(finalizou_sequencia());
        break;
      case 6:
        Serial.println(finalizou_sequencia());
        break;
      case 7:
        Serial.println(exercicio3_finalizado());
        break;
      case 8:
        Serial.println(enviar_grau_tremor(3));
        break;
      case 9:
        Serial.println(errou_sequencia());
        break;
      default:
        Serial.println("Serial invalido!");
        break;
    }
    delay(500);
  }
}

/****************************FUNCOES**************************************/

//DEFINIR QUAL MODO - 1, 2, 3 - exercícios 1, 2, 3; 4 - strap
int definir_modo_op(){
  
  String recebido_ble;
  int modo = 0;
  char* codigo_recebido = "M0";

  do{
    Serial.println("Vai enviar QM");
    enviar_codigo("QM");
    Serial.println("Vai receber do bluetooth");
    codigo_recebido = receber_codigo(2);
  }while(codigo_recebido[0] != 'M');
  
  modo = (int)codigo_recebido[1] - 48;

  return modo;
  
}

//O BOTÃO DE PARAR NO MEIO DO JOGO FOI APERTADO?
int botao_parar_apertado( ){// 0 Não apertou | 1 Apertou

  int apertou = 0;
  char* codigo_recebido = "PX";

  do{
    enviar_codigo("BP");
    codigo_recebido = receber_codigo(2);
  }while(codigo_recebido[0] != 'P');
  
  apertou = (int)codigo_recebido[1] - 48;

  return apertou; //0 Não apertou | 1 Apertou
}

//O USUÁRIO ERROU A SEQUENCIA, ELE QUER CONTINUAR?
int errou_sequencia( ){//0 Não quer continuar | 1 Continuar nivel 0
  String recebido_ble;
  int quer_continuar = 0;
  char* codigo_recebido = "C0";

  do{
    enviar_codigo("ES");
    codigo_recebido = receber_codigo(2);
  }while(codigo_recebido[0] != 'C');
  
  quer_continuar = (int)codigo_recebido[1] - 48;

  return quer_continuar; //0 Não quer continuar | 1 Continuar nivel 0
}

//QUAL FOI O NÍVEL SALVO NO APLICATIVO?
int qual_nivel(int exercicio){

  int nivel = 0;
  char* codigo_recebido = "N0XX";

  if(exercicio == 1)
    do{
      enviar_codigo("N1");
      codigo_recebido = receber_codigo(4);
    }while(codigo_recebido[0] != 'N'&& codigo_recebido[1] != '1');
  else
    do{
      enviar_codigo("N2");
      codigo_recebido = receber_codigo(4);
    }while(codigo_recebido[0] != 'N'&& codigo_recebido[1] != '2');
  
  nivel = 10*((int)codigo_recebido[2] - 48)+1*((int)codigo_recebido[3] - 48);

  return nivel;
}

//JOGO FINALIZADO, ENVIAR O NÍVEL PARA O APLICATIVO
int enviar_nivel(int nivel, int exercicio){
  
  char* codigo_recebido; 
  char codigo_a_enviar[5] = "V0XX";

  if(exercicio == 1)
    codigo_a_enviar[1] = '1';
  else
    codigo_a_enviar[1] = '2';
  do{
    codigo_a_enviar[2] = nivel/10 + '0';
    codigo_a_enviar[3] = nivel%10 + '0';
    enviar_codigo(codigo_a_enviar);
    codigo_recebido = receber_codigo(2);
  }while(strcmp(codigo_recebido,"SN") != 0);
  
  return 0;
}

//ENVIAR SEQUENCIA A SER APRESENTADA
int enviar_sequencia(int quantidade, int* vetor_de_dados){
  
  char* codigo_recebido;
  char sequencia[36] = "SQXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXF";
  
  for(int i = 0 ; i < quantidade ; i++)
    sequencia[i+2] = vetor_de_dados[i] + '0';
  sequencia[quantidade+2] = 'F'; 
  sequencia[quantidade+3] = '\0'; 
  do{
    enviar_codigo(sequencia);
    codigo_recebido = receber_codigo(2);
  }while(strcmp(codigo_recebido,"SR") != 0);

  
  return 0;
}

//AVISA QUE FINALIZOU A SEQUENCIA
int finalizou_sequencia(){
  char* codigo_recebido;
  
  do{
    codigo_recebido = receber_codigo(2);
  }while(strcmp(codigo_recebido,"SF") != 0);
  
  return 0;
}

//SABER SE O EXERCÍCIO 3 FOI FINALIZADO
int exercicio3_finalizado( ){

  char* codigo_recebido;
  
  do{
    enviar_codigo("F3");
    codigo_recebido = receber_codigo(2);
  }while(strcmp(codigo_recebido,"F1") != 0);

  return 1;
}

//ENVIAR O GRAU DE TREMOR PARA O CELULAR
int enviar_grau_tremor(int grau_tremor){

  char codigo_a_enviar[4] = "GTX";
  char* codigo_recebido;
  
  codigo_a_enviar[2] = grau_tremor + '0';
  do{
    enviar_codigo(codigo_a_enviar);
    codigo_recebido = receber_codigo(3);
  }while(strcmp(codigo_recebido,"GTR") != 0);

  return 0;
}

//FUNÇÃO PARA ENVIAR CÓDIGO CHAR* PARA O CELULAR VIA BLE
void enviar_codigo(char* vetor){

  pCharacteristic->setValue(vetor);
  Serial.print("Enviou :'");
  Serial.print(vetor);
  Serial.print("'\n");
}

//FUNÇÃO PARA RECEBER CÓDIGO CHAR* PARA O CELULAR VIA BLE
char* receber_codigo(int tamanho){
  
  char* codigo_char = (char*)malloc((tamanho+1)*sizeof(char));

  while(strcmp(codigo_BLE,"0000") == 0)
    delay(100);
    
  for (int i = 0 ; i < tamanho; i++)
    codigo_char[i] = codigo_BLE[i];
  codigo_char[tamanho] = '\0';

  strcpy(codigo_BLE,"0000");
  
  Serial.print("Recebeu pelo BLE :'");
  Serial.print(codigo_char);
  Serial.print("'\n");
  
 return codigo_char;
}


void loop() {

  
}

