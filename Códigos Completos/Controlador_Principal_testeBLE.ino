#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Bibliotecas do BLE
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

//Biblioteca do Wifi#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Bibliotecas do BLE
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

//Biblioteca do Wifi
//#include <ESP8266WiFi.h>
//#include <WiFi.h>

//ENTRADAS E SAIDAS
//LEDs e botÃµes
#define LEDVermelho 13
#define LEDLaranja 12
#define LEDAmarelo 14
#define LEDVerde 26
#define LEDAzul 25

#define ButtonVermelho 17
#define ButtonLaranja 05
#define ButtonAmarelo 18
#define ButtonVerde 19
#define ButtonAzul 21

//BLE
#define SERVICE_UUID        "c96d9bcc-f3b8-442e-b634-d546e4835f64"
#define CHARACTERISTIC_UUID "807b8bad-a892-4ff7-b8bc-83a644742f9b"
#define CHARACTERISTIC_BS_UUID "44c6fe06-3860-4d90-903d-f665ec523e7a"
#define CHARACTERISTIC_SC_UUID "0fb8b636-c355-4269-9a7f-02011938cd1a"
#define NOTIFICATION_UUID     0x2A08

BLEServer* pServer = NULL;
BLEAdvertising* pAdvertising = NULL;
BLEService *pService = NULL;
BLECharacteristic* pCharacteristic = NULL;
BLECharacteristic* pCharacteristic_bs = NULL;
BLECharacteristic* pCharacteristic_sc = NULL;

bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

char* codigo_BLE;
char* codigo_BLE_bs;
char* codigo_BLE_sc;

int sequencia_padrao[6] = {1, 2, 3, 4, 5, 0};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      strcpy(codigo_BLE, value.c_str());
      //Serial.println("Salvou o dado");
    }
};

class MyCallbacks_bs: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic_bs) {
      std::string value = pCharacteristic_bs->getValue();

      strcpy(codigo_BLE_bs, value.c_str());
    }
};

class MyCallbacks_sc: public BLECharacteristicCallbacks {
      void onWrite(BLECharacteristic *pCharacteristic_sc) {
      std::string value = pCharacteristic_sc->getValue();

      strcpy(codigo_BLE_sc, value.c_str());
      //Serial.println(codigo_BLE_sc);
    }
};

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      //BLEDevice::startAdvertising();
      delay(1000);
      pServer->startAdvertising();
      //Serial.println("entrou nessa condição: onConnect");
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};




/*******************************************************************
                        FUNÃ‡Ã•ES PRINCIPAIS
                      DO CONTROLADOR PRINCIPAL
 ******************************************************************/
int comunicacao(int modo_de_op, int escolha, int* vetor_de_dados) {
  int retorno;
  int i;
  switch (modo_de_op) {
    case 0: // modo principal
      switch (escolha) {
        case 0:
          retorno = definir_modo_op();
          break;
        case 1:
          retorno = botao_parar_apertado();
          break;
        default:
          Serial.print("Comando invalido!\n");
          break;
      }
      break;
    case 1: // exercÃ­cio 1
      switch (escolha) {
        case 1:
          retorno = botao_parar_apertado();
          break;
        case 2:
          retorno = errou_sequencia();
          break;
        case 3:
          retorno = qual_nivel(1);
          break;
        case 4:
          retorno = enviar_nivel(vetor_de_dados[0], 1);
          break;
        case 5:
          i = 0;
          while (vetor_de_dados[i] != 0) {
            Serial.println(vetor_de_dados[i]);
            i++;
          }
          Serial.println(vetor_de_dados[i]);
          enviar_sequencia(i, vetor_de_dados);
          retorno = finalizou_sequencia();
          break;
        default:
          Serial.print("Comando invalido!\n");
          break;
      }
      break;
    case 2: // exercÃ­cio 2
      switch (escolha) {
        case 1:
          retorno = botao_parar_apertado();
          break;
        case 2:
          retorno = errou_sequencia();
          break;
        case 3:
          retorno = qual_nivel(2);
          break;
        case 4:
          retorno = enviar_nivel(vetor_de_dados[0], 2);
          break;
        case 5:
          i = 0;
          while (vetor_de_dados[i] != 0) {
            i++;
          }
          enviar_sequencia(i, vetor_de_dados);
          for (int j = 0 ; j < i ; j++)
            acender_leds(vetor_de_dados[j], 333);
          retorno = finalizou_sequencia();
          break;
        default:
          Serial.print("Comando invalido!\n");
          break;
      }
      break;
    case 3: // exercÃ­cio 3
      switch (escolha) {
        case 0:
          retorno = exercicio3_finalizado();
          break;
        default:
          break;
      }
      break;
    case 4: // pulseira
      switch (escolha) {
        case 0:
          retorno = receber_modo_grau();
          retorno = bs_receber_grau_tremor(retorno);
          //Receber da base o grau, no caso aqui Ã© 3
          retorno = enviar_grau_tremor(retorno); //ADICIONAR FUNÃ‡ÃƒO QUE RECEBE O GRAU NO LUGAR DO 3
          break;
        default:
          Serial.print("Comando invalido!\n");
          break;
      }
      break;
    case 5: // Leitura dos botÃµes da esf secundaria
      switch (escolha) {
        case 0:
          retorno = sec_ler_botoes();
          break;
        case 1:
          retorno = sec_botao_ap();
          break;
        default:
          Serial.print("Comando invalido!\n");
          break;
      }
      break;
    case 6: // acender leds da esfera secundaria
      switch (escolha) {
        case 1:
          sec_led_acender();
          retorno = sec_led_on(vetor_de_dados[0]);
          //delay_ms(vetor_de_dados[1]);
          break;
        default:
          Serial.print("Comando invalido!\n");
          break;
      }
      break;
    default:
      break;
  }
  return retorno;
}

/********* EXERCÃ�CIOS ************/
void exercicio1() {
  //INICIO
  int semente_rand = time(NULL);
  int count, nivel;
  int i, j = 0;
  const int lower = 1, upper = 5;
  int sequencia[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int deseja_continuar = 1;

  //DEMONSTRAÃ‡ÃƒO
  comunicacao(1, 5, sequencia_padrao);

  for (i = 0; i < 5; i++) {
    acender_leds(i + 1, 333);
  }

  //RECEBE NÃ�VEL
  nivel = comunicacao(1, 3, sequencia_padrao);

  for (count = nivel; count < 32 && deseja_continuar == 1; count++)
  {
    //GERA SEQUÃŠNCIA ALEATÃ“RIA
    srand(semente_rand);
    if (count == nivel)
      for (j = 0; j < count; j++)
        sequencia[j] = printRandoms(lower, upper, count);
    else
      sequencia[count - 1] = printRandoms(lower, upper, count);

    //MANDA SEQUÃŠNCIA
    comunicacao(1, 5, sequencia);

    //LER BOTÃ•ES E ACENDE LEDS
    if (ler_botoes(count, sequencia) == 1) {

      //ERROU, DESEJA CONTINUAR?
      deseja_continuar = comunicacao(1, 2, sequencia_padrao);
      count = 0;
      nivel = 1;
      for (i = 0; i < 32; i++)
        sequencia[i] = 0;
    } else {

      //ACERTOU, DESEJA CONTINUAR?
      deseja_continuar = 1 - comunicacao(1, 1, sequencia_padrao);
    }
  }

  //MANDA NÃ�VEL
  nivel = count;
  comunicacao(1, 4, &nivel);

}
void exercicio2() {
  //INICIO
  int semente_rand = time(NULL);
  int count, nivel;
  int i, j = 0;
  const int lower = 1, upper = 5;
  int sequencia[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int deseja_continuar = 1;

  //DEMONSTRAÃ‡ÃƒO
  comunicacao(2, 5, sequencia_padrao);

  //ACENDE LEDS DA SEQUENCIA
  /*for (i = 0; i < 5; i++){
      acender_leds(i+1,333);
    }*/

  //RECEBE NÃ�VEL
  nivel = comunicacao(2, 3, sequencia_padrao);

  for (count = nivel; count < 32 && deseja_continuar == 1; count++)
  {

    //GERA SEQUÃŠNCIA ALEATÃ“RIA
    srand(semente_rand);
    if (count == nivel)
      for (j = 0; j < count; j++)
        sequencia[j] = printRandoms(lower, upper, count);
    else
      sequencia[count - 1] = printRandoms(lower, upper, count);

    //MANDA SEQUÃŠNCIA
    comunicacao(2, 5, sequencia);

    //ACENDE LEDS DA SEQUENCIA
    /*i = 0;
      while (sequencia[i] != 0){
        acender_leds(sequencia[i],333);
        i++;
      }*/

    //LER BOTÃ•ES E ACENDE LEDS
    if (ler_botoes(count, sequencia) == 1) {

      //ERROU, DESEJA CONTINUAR?
      deseja_continuar = comunicacao(2, 2, sequencia_padrao);
      count = 0;
      nivel = 1;
      for (i = 0; i < 32; i++)
        sequencia[i] = 0;
    } else {

      //ACERTOU, DESEJA CONTINUAR?
      deseja_continuar = 1 - comunicacao(2, 1, sequencia_padrao);
    }
  }

  //MANDA NÃ�VEL
  nivel = count;
  comunicacao(2, 4, &nivel);

}
void exercicio3() {
  while (comunicacao(3, 0, sequencia_padrao) != 0);
}
void pulseira() {
  while (comunicacao(4, 0, sequencia_padrao) != 0);
}


/***** BOTÃ•ES E LEDS ************/
int ler_botoes(int nivel, int* sequencia) {
  Serial.print("Digite a seq de botoes\n");
  int botao, botao_sec;
  int apertou = 1;
  int errou = 0;
  int i, cont = 0;
  int porta = 0;
  unsigned long tempo_inicial = 0;
  unsigned long tempo_atual = millis();

  // LER CADA BOTAO E VERIFICA SE ERROU
  tempo_inicial = tempo_atual;
  while (cont < nivel && (tempo_atual - tempo_inicial) < (nivel*1000+2000)) {
    //FALAR QUE É PARA LER BOTOES DA SECUNDARIA
    if(apertou == 1)
      comunicacao(5,0, sequencia_padrao);

    // VERIFICAR TODOS OS CINCO BOTOES
    for (i = 0; i < 5 ; i++) {

      apertou = not(digitalRead(definir_botao(i + 1)));

      while(digitalRead(definir_botao(i + 1)) == 0);
      
      if (apertou == 1) {
        //APERTOU BOTAO
        botao = i + 1;
                
        //ACENDE O LED
        acender_leds(botao, 50);
        //delay_ms(200);

        //VERIFICA SE O BOTAO DA ESF SEC FOI APERTADO
        botao_sec = comunicacao(5,1,sequencia_padrao);
        //botao_sec = botao;

        //VERIFICA SE ESTA CORRETO
        if (sequencia[cont] == botao && sequencia[cont] == botao_sec && errou == 0)
          errou = 0;
        else
          errou = 1;
        cont++;

        break;
      }
    }
  }

  // QUANTO TEMPO DEMOROU PARA LER CADA BOTAO

  return errou;
}
void acender_leds(int LED, int tempo) {
  int vetor_led[3] = {LED, tempo, 0};

  //printf("LED %d aceso\n", LED);
  if (tempo == 333)
    comunicacao(6, 1, vetor_led);
  digitalWrite(definir_LED(LED), HIGH);
  delay_ms(tempo-10);
  digitalWrite(definir_LED(LED), LOW);
  delay_ms(10);
}

int printRandoms(int lower, int upper, int count) {
  int i, num = 0;
  for (i = 0; i < count; i++) {
    num = (rand() % (upper - lower + 1)) + lower;
  }
  return num;
}
void delay_ms(int tempo) {
  delay(tempo);
}

int definir_botao(int escolha) {
  int porta = 0;
  switch (escolha) {
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
int definir_LED(int escolha) {
  int porta = 0;
  switch (escolha) {
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

/*******************************************************************/


/*******************************************************************
                 FUNÃ‡Ã•ES DA COMUNICAÃ‡ÃƒO ENTRE
              CONTROLADOR PRINCIPAL E SMARTPHONE
 ******************************************************************/
//DEFINIR QUAL MODO - 1, 2, 3 - exercÃ­cios 1, 2, 3; 4 - strap
int definir_modo_op() {

  String recebido_ble;
  int modo = 0;
  char* codigo_recebido = "M0";

  do {
    //Serial.println("Vai enviar QM");
    enviar_codigo("QM");
    //Serial.println("Vai receber do bluetooth");
    codigo_recebido = receber_codigo(2);
  } while (codigo_recebido[0] != 'M');

  modo = (int)codigo_recebido[1] - 48;

  return modo;

}

//O BOTÃƒO DE PARAR NO MEIO DO JOGO FOI APERTADO?
int botao_parar_apertado( ) { // 0 NÃ£o apertou | 1 Apertou

  int apertou = 0;
  char* codigo_recebido = "PX";

  do {
    enviar_codigo("BP");
    codigo_recebido = receber_codigo(2);
  } while (codigo_recebido[0] != 'P');

  apertou = (int)codigo_recebido[1] - 48;

  return apertou; //0 NÃ£o apertou | 1 Apertou
}

//O USUÃ�RIO ERROU A SEQUENCIA, ELE QUER CONTINUAR?
int errou_sequencia( ) { //0 NÃ£o quer continuar | 1 Continuar nivel 0
  String recebido_ble;
  int quer_continuar = 0;
  char* codigo_recebido = "C0";

  do {
    enviar_codigo("ES");
    codigo_recebido = receber_codigo(2);
  } while (codigo_recebido[0] != 'C');

  quer_continuar = (int)codigo_recebido[1] - 48;

  return quer_continuar; //0 NÃ£o quer continuar | 1 Continuar nivel 0
}

//QUAL FOI O NÃ�VEL SALVO NO APLICATIVO?
int qual_nivel(int exercicio) {

  int nivel = 0;
  char* codigo_recebido = "N0XX";

  if (exercicio == 1)
    do {
      enviar_codigo("N1");
      codigo_recebido = receber_codigo(4);
    } while (codigo_recebido[0] != 'N' || codigo_recebido[1] != '1');
  else
    do {
      enviar_codigo("N2");
      codigo_recebido = receber_codigo(4);
    } while (codigo_recebido[0] != 'N' || codigo_recebido[1] != '2');

  nivel = 10 * ((int)codigo_recebido[2] - 48) + 1 * ((int)codigo_recebido[3] - 48);

  return nivel;
}

//JOGO FINALIZADO, ENVIAR O NÃ�VEL PARA O APLICATIVO
int enviar_nivel(int nivel, int exercicio) {

  char* codigo_recebido;
  char codigo_a_enviar[5] = "V0XX";

  if (exercicio == 1)
    codigo_a_enviar[1] = '1';
  else
    codigo_a_enviar[1] = '2';
  do {
    codigo_a_enviar[2] = nivel / 10 + '0';
    codigo_a_enviar[3] = nivel % 10 + '0';
    enviar_codigo(codigo_a_enviar);
    codigo_recebido = receber_codigo(2);
  } while (strcmp(codigo_recebido, "SN") != 0);

  return 0;
}

//ENVIAR SEQUENCIA A SER APRESENTADA
int enviar_sequencia(int quantidade, int* vetor_de_dados) {

  char* codigo_recebido;
  char sequencia[36] = "SQXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXF";

  for (int i = 0 ; i < quantidade ; i++)
    sequencia[i + 2] = vetor_de_dados[i] + '0';
  sequencia[quantidade + 2] = 'F';
  sequencia[quantidade + 3] = '\0';
  do {
    enviar_codigo(sequencia);
    codigo_recebido = receber_codigo(2);
  } while (strcmp(codigo_recebido, "SR") != 0);


  return 0;
}

//AVISA QUE FINALIZOU A SEQUENCIA
int finalizou_sequencia() {
  char* codigo_recebido;

  do {
    codigo_recebido = receber_codigo(2);
  } while (strcmp(codigo_recebido, "SF") != 0);

  return 0;
}

//SABER SE O EXERCÃ�CIO 3 FOI FINALIZADO
int exercicio3_finalizado( ) {

  char* codigo_recebido;

  do {
    enviar_codigo("F3");
    codigo_recebido = receber_codigo(2);
  } while (strcmp(codigo_recebido, "F1") != 0);

  return 1;
}

//RECEBER O MODO DE MEDIDA DO DO GRAU DE TREMOR
int receber_modo_grau() {

  int grau = 0;
  char* codigo_recebido = "MTX";
  do {
    enviar_codigo("MT");
    codigo_recebido = receber_codigo(3);
  } while (codigo_recebido[0] != 'M' || codigo_recebido[1] != 'T');

  do {
    codigo_recebido = receber_codigo(2);
  } while (strcmp(codigo_recebido, "IT") != 0);

  grau = (int)codigo_recebido[2] - 48;

  return grau;
}

//ENVIAR O GRAU DE TREMOR PARA O CELULAR
int enviar_grau_tremor(int grau_tremor) {

  char codigo_a_enviar[4] = "GTX";
  char* codigo_recebido;

  codigo_a_enviar[2] = grau_tremor + '0';
  do {
    enviar_codigo(codigo_a_enviar);
    codigo_recebido = receber_codigo(3);
  } while (strcmp(codigo_recebido, "GTR") != 0);

  return 0;
}

//FUNÃ‡ÃƒO PARA ENVIAR CÃ“DIGO CHAR* PARA O CELULAR VIA BLE
void enviar_codigo(char* vetor) {

  pCharacteristic->setValue(vetor);
  Serial.print("Enviou :'");
  Serial.print(vetor);
  Serial.print("'\n");
}

//FUNÃ‡ÃƒO PARA RECEBER CÃ“DIGO CHAR* PARA O CELULAR VIA BLE
char* receber_codigo(int tamanho) {

  char* codigo_char = (char*)malloc((tamanho + 1) * sizeof(char));

  while (strcmp(codigo_BLE, "0000") == 0)
    delay(100);

  for (int i = 0 ; i < tamanho; i++)
    codigo_char[i] = codigo_BLE[i];
  codigo_char[tamanho] = '\0';

  strcpy(codigo_BLE, "0000");

  Serial.print("Recebeu pelo BLE :'");
  Serial.print(codigo_char);
  Serial.print("'\n");

  return codigo_char;
}

/*******************************************************************/

/*******************************************************************
                    FUNÃ‡Ã•ES DA COMUNICAÃ‡ÃƒO ENTRE
                    CONTROLADOR PRINCIPAL E BASE
 ******************************************************************/

int bs_receber_grau_tremor(int modo_de_medida) {
  char codigo_a_enviar[4] = "TTX";
  char* codigo_recebido;
  int grau = 5;

  do {
    wifi_enviar_bs("IT", 2);
    codigo_recebido = wifi_receber_bs(3);
  } while (strcmp(codigo_recebido, "AET") != 0);

  codigo_a_enviar[2] = modo_de_medida + '0';

  do {
    wifi_enviar_bs(codigo_a_enviar, 3);
    codigo_recebido = wifi_receber_bs(2);
  } while (codigo_recebido[0] != 'G');

  grau = (int)codigo_recebido[1] - '0';

  return grau;
}

char* wifi_receber_bs(int tamanho ) {
  
  char* codigo_char = (char*)malloc((tamanho + 1) * sizeof(char));

  while (strcmp(codigo_BLE_bs, "0000") == 0)
    delay(5);
  /*Serial.print("1.1 Recebou da base via wifi:'");
  Serial.print(codigo_BLE_bs);
  Serial.print("'");*/
  strcpy(codigo_char,codigo_BLE_bs);
  codigo_char[tamanho] = '\0';

  strcpy(codigo_BLE_bs, "0000");

  Serial.print("Recebou da base via wifi:'");
  Serial.print(codigo_char);
  Serial.print("'\n");

  return codigo_char;
}
int wifi_enviar_bs(char* string, int tamanho) {

  /*while (strcmp(wifi_receber_sc(2), "XX") != 0)
    delay(100);*/
  pCharacteristic_bs->setValue(string);
  while (strcmp(wifi_receber_bs(tamanho), string) != 0);

  pCharacteristic_bs->setValue("0000");
  Serial.print("Enviou para a base via wifi:'");
  Serial.print(string);
  Serial.print("'\n");

  return 0;
}

/******************************************************************/
/*******************************************************************
                        FUNÃ‡Ã•ES DA COMUNICAÃ‡ÃƒO ENTRE
                    CONTROLADOR PRINCIPAL E SECUNDARIO
 ******************************************************************/
//Novo botÃ£o 0-nÃ£o apertado 1-apertado
int sec_ler_botoes() {

  wifi_enviar_sc("NV", 2);

  return 0;
}

//qual botÃ£o 1 a 5
int sec_botao_ap() {

  int botao = 0;
  char* cod_recebidow;

  do {
    //enviar_secundario("QB");
    wifi_enviar_sc("QB", 2);
    cod_recebidow = wifi_receber_sc(2);
  } while (cod_recebidow[0] != 'B');

  botao = (int)cod_recebidow[1] - 48; // virou inteiro

  return botao;
}

//aviso que led vai acender - 0 nÃ£o vai 1 vai
int sec_led_acender() {

  wifi_enviar_sc("AL", 2);

  return 0;
}

//avisa qual led vai acender
int sec_led_on(int led) {

  char* cod_recebidow;
  char codigo_enviar[3] = "LX";

  Serial.println("entrou no sec_led_on"); 
  do {
    codigo_enviar[1] = led + '0';
    //enviar_secundario(codigo_enviar);
    wifi_enviar_sc(codigo_enviar, 2);
    cod_recebidow = wifi_receber_sc(2);
  } while (strcmp(cod_recebidow, "QL") != 0);


  return 0;
}

int acender_todos_leds(int tempo){
  char enviar[6] = "TLXXX";
  enviar[2] = (int)tempo/100 + '0';
  enviar[3] = (int)(tempo%100)/10 + '0';
  enviar[4] = (int)(tempo%100)%10 + '0';
  wifi_enviar_sc("TL", 2);
  wifi_enviar_sc(enviar, 5);

  return 0;
}

int acender_alguns_leds(int tempo){
  char enviar[] = "BLXXX";
  enviar[2] = (int)tempo/100 + '0';
  enviar[3] = (int)(tempo%100)/10 + '0';
  enviar[4] = (int)(tempo%100)%10 + '0';
  wifi_enviar_sc("BL", 2);
  wifi_enviar_sc(enviar, 5);

  return 0;
}


int acender_seq_leds(int tempo){
  char enviar[] = "VLXXX";
  enviar[2] = (int)tempo/100 + '0';
  enviar[3] = (int)(tempo%100)/10 + '0';
  enviar[4] = (int)(tempo%100)%10 + '0';
  wifi_enviar_sc("VL", 2);
  wifi_enviar_sc(enviar, 5);

  return 0;
}

char* wifi_receber_sc(int tamanho ) {
  
  char* codigo_char = (char*)malloc((tamanho + 1) * sizeof(char));

  while (strcmp(codigo_BLE_sc, "0000") == 0)
    delay(5);
  /*Serial.print("1.1 Recebou da base via wifi:'");
  Serial.print(codigo_BLE_bs);
  Serial.print("'");*/
  strcpy(codigo_char,codigo_BLE_sc);
  codigo_char[tamanho] = '\0';

  strcpy(codigo_BLE_sc, "0000");

  Serial.print("Recebou do secundario via wifi:'");
  Serial.print(codigo_char);
  Serial.print("'\n");

  return codigo_char;
}
int wifi_enviar_sc(char* string, int tamanho) {

  /*while (strcmp(wifi_receber_sc(2), "XX") != 0)
    delay(100);*/
  pCharacteristic_sc->setValue(string);
  while (strcmp(wifi_receber_sc(tamanho), string) != 0);

  pCharacteristic_sc->setValue("0000");
  Serial.print("Enviou para o secundario via wifi:'");
  Serial.print(string);
  Serial.print("'\n");

  return 0;
}

/******************************************************************/
void setup() {

  //Definindo LEDS e BotÃµes
  Serial.begin(115200);
  for (int contador = 1; contador <= 5; contador++)
    pinMode(definir_LED(contador), OUTPUT);
  for (int contador = 1; contador <= 5; contador++)
    pinMode(definir_botao(contador), INPUT_PULLUP);

  //Definindo BLE
  BLEDevice::init("ESTER"); //Nome do BLE
  pServer = BLEDevice::createServer(); //Criando Servidor
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID); //Criando serviÃ§o
  
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

  //pCharacteristic->addDescriptor(new BLE2902());
  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->setValue("C");
  pCharacteristic->addDescriptor(new BLE2902());
  codigo_BLE = (char*)malloc(34 * sizeof(char));
  strcpy(codigo_BLE, "0000");
        
  pCharacteristic_bs = pService->createCharacteristic(
                         CHARACTERISTIC_BS_UUID,
                         BLECharacteristic::PROPERTY_READ   |
                         BLECharacteristic::PROPERTY_WRITE  |
                         BLECharacteristic::PROPERTY_NOTIFY |
                         BLECharacteristic::PROPERTY_INDICATE
                       );

  pCharacteristic_bs->setCallbacks(new MyCallbacks_bs());
  pCharacteristic_bs->setValue("B");
  pCharacteristic_bs->addDescriptor(new BLE2902());
  codigo_BLE_bs = (char*)malloc(6 * sizeof(char));
  strcpy(codigo_BLE_bs, "0000");

  pCharacteristic_sc = pService->createCharacteristic(
                         CHARACTERISTIC_SC_UUID,
                         BLECharacteristic::PROPERTY_READ   |
                         BLECharacteristic::PROPERTY_WRITE  |
                         BLECharacteristic::PROPERTY_NOTIFY |
                         BLECharacteristic::PROPERTY_INDICATE
                       );

  pCharacteristic_sc->setCallbacks(new MyCallbacks_sc());
  pCharacteristic_sc->setValue("S");
  pCharacteristic_sc->addDescriptor(new BLE2902());
  codigo_BLE_sc = (char*)malloc(6 * sizeof(char));
  strcpy(codigo_BLE_sc, "0000");
  
  pService->start();
  pAdvertising = pServer->getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  pAdvertising->start();

}

void loop() {
  int estado_atual = 5;
  int proximo_estado = 0;
  int modo = 0;
  int conectado = 0;
  int sequencia_inv[6] = {5, 4, 3, 2, 1, 0};

  Serial.println("Conectando aos dispositivos via BLE");
  
  Serial.print("Conectar com o secundário\n");
  while (strcmp(wifi_receber_sc(1), "S") != 0) {
    wifi_enviar_sc("S",1);
    delay(250);
  }
  Serial.print("secundária conectada\n");
    
  delay(250);
  acender_seq_leds(100);
  delay(10);
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
  
  Serial.print("Conectar com a base\n");
  while (strcmp(wifi_receber_bs(1), "B") != 0) {
    wifi_enviar_bs("B",1);
    delay(250);
  }
  Serial.print("base conectada\n");
  
  delay(250);
  acender_todos_leds(250);
  delay(10);
  digitalWrite(definir_LED(1), HIGH);
  digitalWrite(definir_LED(2), HIGH);
  digitalWrite(definir_LED(3), HIGH);
  digitalWrite(definir_LED(4), HIGH);
  digitalWrite(definir_LED(5), HIGH);
  delay(250);
  digitalWrite(definir_LED(1), LOW);
  digitalWrite(definir_LED(2), LOW);
  digitalWrite(definir_LED(3), LOW);
  digitalWrite(definir_LED(4), LOW);
  digitalWrite(definir_LED(5), LOW);
  
  Serial.print("Conectar com o smartphone\n");
  while (strcmp(receber_codigo(1), "C") != 0) {
    enviar_codigo("C");
    delay(250);
  }
  Serial.print("Conectado com o smartphone\n");

  delay(250);
  acender_alguns_leds(250);
  delay(10);
  digitalWrite(definir_LED(1), HIGH);
  digitalWrite(definir_LED(3), HIGH);
  digitalWrite(definir_LED(5), HIGH);
  delay(250);
  digitalWrite(definir_LED(1), LOW);
  digitalWrite(definir_LED(3), LOW);
  digitalWrite(definir_LED(5), LOW);
  digitalWrite(definir_LED(2), HIGH);
  digitalWrite(definir_LED(4), HIGH);
  delay(250);
  digitalWrite(definir_LED(2), LOW);
  digitalWrite(definir_LED(4), LOW);

  while (/*comunicacao(0, 1, sequencia_padrao) == 0*/1) {
    switch (estado_atual) {
      case 0:
        modo = comunicacao(0, 0, sequencia_padrao);
        proximo_estado = modo;
        //stand_by();
        break;
      case 1:
        exercicio1();
        proximo_estado = 0;
        break;
      case 2:
        exercicio2();
        proximo_estado = 0;
        break;
      case 3:
        exercicio3();
        proximo_estado = 0;
        break;
      case 4:
        pulseira();
        proximo_estado = 0;
        break;
      case 5:
        //stand_by();
        proximo_estado = 0;
        break;
    }
    estado_atual = proximo_estado;
    //delay(1000);
  }


}

//#include <ESP8266WiFi.h>
//#include <WiFi.h>

//ENTRADAS E SAIDAS
//LEDs e botÃµes
#define LEDVermelho 13
#define LEDLaranja 12
#define LEDAmarelo 14
#define LEDVerde 26
#define LEDAzul 25

#define ButtonVermelho 17
#define ButtonLaranja 05
#define ButtonAmarelo 18
#define ButtonVerde 19
#define ButtonAzul 21

//BLE
#define SERVICE_UUID        "c96d9bcc-f3b8-442e-b634-d546e4835f64"
#define CHARACTERISTIC_UUID "807b8bad-a892-4ff7-b8bc-83a644742f9b"
#define CHARACTERISTIC_BS_UUID "44c6fe06-3860-4d90-903d-f665ec523e7a"
#define CHARACTERISTIC_SC_UUID "0fb8b636-c355-4269-9a7f-02011938cd1a"
#define NOTIFICATION_UUID     0x2A08

BLEServer* pServer = NULL;
BLEAdvertising* pAdvertising = NULL;
BLEService *pService = NULL;
BLECharacteristic* pCharacteristic = NULL;
BLECharacteristic* pCharacteristic_bs = NULL;
BLECharacteristic* pCharacteristic_sc = NULL;

bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

char* codigo_BLE;
char* codigo_BLE_bs;
char* codigo_BLE_sc;

int sequencia_padrao[6] = {1, 2, 3, 4, 5, 0};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      strcpy(codigo_BLE, value.c_str());
      //Serial.println("Salvou o dado");
    }
};

class MyCallbacks_bs: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic_bs) {
      std::string value = pCharacteristic_bs->getValue();

      strcpy(codigo_BLE_bs, value.c_str());
    }
};

class MyCallbacks_sc: public BLECharacteristicCallbacks {
      void onWrite(BLECharacteristic *pCharacteristic_sc) {
      std::string value = pCharacteristic_sc->getValue();

      strcpy(codigo_BLE_sc, value.c_str());
      //Serial.println(codigo_BLE_sc);
    }
};

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      //BLEDevice::startAdvertising();
      delay(1000);
      pServer->startAdvertising();
      //Serial.println("entrou nessa condição: onConnect");
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};




/*******************************************************************
                        FUNÃ‡Ã•ES PRINCIPAIS
                      DO CONTROLADOR PRINCIPAL
 ******************************************************************/
int comunicacao(int modo_de_op, int escolha, int* vetor_de_dados) {
  int retorno;
  int i;
  switch (modo_de_op) {
    case 0: // modo principal
      switch (escolha) {
        case 0:
          retorno = definir_modo_op();
          break;
        case 1:
          retorno = botao_parar_apertado();
          break;
        default:
          Serial.print("Comando invalido!\n");
          break;
      }
      break;
    case 1: // exercÃ­cio 1
      switch (escolha) {
        case 1:
          retorno = botao_parar_apertado();
          break;
        case 2:
          retorno = errou_sequencia();
          break;
        case 3:
          retorno = qual_nivel(1);
          break;
        case 4:
          retorno = enviar_nivel(vetor_de_dados[0], 1);
          break;
        case 5:
          i = 0;
          while (vetor_de_dados[i] != 0) {
            Serial.println(vetor_de_dados[i]);
            i++;
          }
          Serial.println(vetor_de_dados[i]);
          enviar_sequencia(i, vetor_de_dados);
          retorno = finalizou_sequencia();
          break;
        default:
          Serial.print("Comando invalido!\n");
          break;
      }
      break;
    case 2: // exercÃ­cio 2
      switch (escolha) {
        case 1:
          retorno = botao_parar_apertado();
          break;
        case 2:
          retorno = errou_sequencia();
          break;
        case 3:
          retorno = qual_nivel(2);
          break;
        case 4:
          retorno = enviar_nivel(vetor_de_dados[0], 2);
          break;
        case 5:
          i = 0;
          while (vetor_de_dados[i] != 0) {
            i++;
          }
          enviar_sequencia(i, vetor_de_dados);
          for (int j = 0 ; j < i ; j++)
            acender_leds(vetor_de_dados[j], 333);
          retorno = finalizou_sequencia();
          break;
        default:
          Serial.print("Comando invalido!\n");
          break;
      }
      break;
    case 3: // exercÃ­cio 3
      switch (escolha) {
        case 0:
          retorno = exercicio3_finalizado();
          break;
        default:
          break;
      }
      break;
    case 4: // pulseira
      switch (escolha) {
        case 0:
          retorno = receber_modo_grau();
          retorno = bs_receber_grau_tremor(retorno);
          //Receber da base o grau, no caso aqui Ã© 3
          retorno = enviar_grau_tremor(retorno); //ADICIONAR FUNÃ‡ÃƒO QUE RECEBE O GRAU NO LUGAR DO 3
          break;
        default:
          Serial.print("Comando invalido!\n");
          break;
      }
      break;
    case 5: // Leitura dos botÃµes da esf secundaria
      switch (escolha) {
        case 0:
          retorno = sec_ler_botoes();
          break;
        case 1:
          retorno = sec_botao_ap();
          break;
        default:
          Serial.print("Comando invalido!\n");
          break;
      }
      break;
    case 6: // acender leds da esfera secundaria
      switch (escolha) {
        case 1:
          sec_led_acender();
          retorno = sec_led_on(vetor_de_dados[0]);
          //delay_ms(vetor_de_dados[1]);
          break;
        default:
          Serial.print("Comando invalido!\n");
          break;
      }
      break;
    default:
      break;
  }
  return retorno;
}

/********* EXERCÃ�CIOS ************/
void exercicio1() {
  //INICIO
  int semente_rand = time(NULL);
  int count, nivel;
  int i, j = 0;
  const int lower = 1, upper = 5;
  int sequencia[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int deseja_continuar = 1;

  //DEMONSTRAÃ‡ÃƒO
  comunicacao(1, 5, sequencia_padrao);

  for (i = 0; i < 5; i++) {
    acender_leds(i + 1, 333);
  }

  //RECEBE NÃ�VEL
  nivel = comunicacao(1, 3, sequencia_padrao);

  for (count = nivel; count < 32 && deseja_continuar == 1; count++)
  {
    //GERA SEQUÃŠNCIA ALEATÃ“RIA
    srand(semente_rand);
    if (count == nivel)
      for (j = 0; j < count; j++)
        sequencia[j] = printRandoms(lower, upper, count);
    else
      sequencia[count - 1] = printRandoms(lower, upper, count);

    //MANDA SEQUÃŠNCIA
    comunicacao(1, 5, sequencia);

    //LER BOTÃ•ES E ACENDE LEDS
    if (ler_botoes(count, sequencia) == 1) {

      //ERROU, DESEJA CONTINUAR?
      deseja_continuar = comunicacao(1, 2, sequencia_padrao);
      count = 0;
      nivel = 1;
      for (i = 0; i < 32; i++)
        sequencia[i] = 0;
    } else {

      //ACERTOU, DESEJA CONTINUAR?
      deseja_continuar = 1 - comunicacao(1, 1, sequencia_padrao);
    }
  }

  //MANDA NÃ�VEL
  nivel = count;
  comunicacao(1, 4, &nivel);

}
void exercicio2() {
  //INICIO
  int semente_rand = time(NULL);
  int count, nivel;
  int i, j = 0;
  const int lower = 1, upper = 5;
  int sequencia[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int deseja_continuar = 1;

  //DEMONSTRAÃ‡ÃƒO
  comunicacao(2, 5, sequencia_padrao);

  //ACENDE LEDS DA SEQUENCIA
  /*for (i = 0; i < 5; i++){
      acender_leds(i+1,333);
    }*/

  //RECEBE NÃ�VEL
  nivel = comunicacao(2, 3, sequencia_padrao);

  for (count = nivel; count < 32 && deseja_continuar == 1; count++)
  {

    //GERA SEQUÃŠNCIA ALEATÃ“RIA
    srand(semente_rand);
    if (count == nivel)
      for (j = 0; j < count; j++)
        sequencia[j] = printRandoms(lower, upper, count);
    else
      sequencia[count - 1] = printRandoms(lower, upper, count);

    //MANDA SEQUÃŠNCIA
    comunicacao(2, 5, sequencia);

    //ACENDE LEDS DA SEQUENCIA
    /*i = 0;
      while (sequencia[i] != 0){
        acender_leds(sequencia[i],333);
        i++;
      }*/

    //LER BOTÃ•ES E ACENDE LEDS
    if (ler_botoes(count, sequencia) == 1) {

      //ERROU, DESEJA CONTINUAR?
      deseja_continuar = comunicacao(2, 2, sequencia_padrao);
      count = 0;
      nivel = 1;
      for (i = 0; i < 32; i++)
        sequencia[i] = 0;
    } else {

      //ACERTOU, DESEJA CONTINUAR?
      deseja_continuar = 1 - comunicacao(2, 1, sequencia_padrao);
    }
  }

  //MANDA NÃ�VEL
  nivel = count;
  comunicacao(2, 4, &nivel);

}
void exercicio3() {
  while (comunicacao(3, 0, sequencia_padrao) != 0);
}
void pulseira() {
  while (comunicacao(4, 0, sequencia_padrao) != 0);
}


/***** BOTÃ•ES E LEDS ************/
int ler_botoes(int nivel, int* sequencia) {
  Serial.print("Digite a seq de botoes\n");
  int botao, botao_sec;
  int apertou = 1;
  int errou = 0;
  int i, cont = 0;
  int porta = 0;
  unsigned long tempo_inicial = 0;
  unsigned long tempo_atual = millis();

  // LER CADA BOTAO E VERIFICA SE ERROU
  tempo_inicial = tempo_atual;
  while (cont < nivel && (tempo_atual - tempo_inicial) < (nivel*1000+2000)) {
    //FALAR QUE É PARA LER BOTOES DA SECUNDARIA
    if(apertou == 1)
      comunicacao(5,0, sequencia_padrao);

    // VERIFICAR TODOS OS CINCO BOTOES
    for (i = 0; i < 5 ; i++) {

      apertou = not(digitalRead(definir_botao(i + 1)));

      while(digitalRead(definir_botao(i + 1)) == 0);
      
      if (apertou == 1) {
        //APERTOU BOTAO
        botao = i + 1;
                
        //ACENDE O LED
        acender_leds(botao, 50);
        //delay_ms(200);

        //VERIFICA SE O BOTAO DA ESF SEC FOI APERTADO
        botao_sec = comunicacao(5,1,sequencia_padrao);
        //botao_sec = botao;

        //VERIFICA SE ESTA CORRETO
        if (sequencia[cont] == botao && sequencia[cont] == botao_sec && errou == 0)
          errou = 0;
        else
          errou = 1;
        cont++;

        break;
      }
    }
  }

  // QUANTO TEMPO DEMOROU PARA LER CADA BOTAO

  return errou;
}
void acender_leds(int LED, int tempo) {
  int vetor_led[3] = {LED, tempo, 0};

  //printf("LED %d aceso\n", LED);
  if (tempo == 333)
    comunicacao(6, 1, vetor_led);
  digitalWrite(definir_LED(LED), HIGH);
  delay_ms(tempo-10);
  digitalWrite(definir_LED(LED), LOW);
  delay_ms(10);
}

int printRandoms(int lower, int upper, int count) {
  int i, num = 0;
  for (i = 0; i < count; i++) {
    num = (rand() % (upper - lower + 1)) + lower;
  }
  return num;
}
void delay_ms(int tempo) {
  delay(tempo);
}

int definir_botao(int escolha) {
  int porta = 0;
  switch (escolha) {
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
int definir_LED(int escolha) {
  int porta = 0;
  switch (escolha) {
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

/*******************************************************************/


/*******************************************************************
                 FUNÃ‡Ã•ES DA COMUNICAÃ‡ÃƒO ENTRE
              CONTROLADOR PRINCIPAL E SMARTPHONE
 ******************************************************************/
//DEFINIR QUAL MODO - 1, 2, 3 - exercÃ­cios 1, 2, 3; 4 - strap
int definir_modo_op() {

  String recebido_ble;
  int modo = 0;
  char* codigo_recebido = "M0";

  do {
    //Serial.println("Vai enviar QM");
    enviar_codigo("QM");
    //Serial.println("Vai receber do bluetooth");
    codigo_recebido = receber_codigo(2);
  } while (codigo_recebido[0] != 'M');

  modo = (int)codigo_recebido[1] - 48;

  return modo;

}

//O BOTÃƒO DE PARAR NO MEIO DO JOGO FOI APERTADO?
int botao_parar_apertado( ) { // 0 NÃ£o apertou | 1 Apertou

  int apertou = 0;
  char* codigo_recebido = "PX";

  do {
    enviar_codigo("BP");
    codigo_recebido = receber_codigo(2);
  } while (codigo_recebido[0] != 'P');

  apertou = (int)codigo_recebido[1] - 48;

  return apertou; //0 NÃ£o apertou | 1 Apertou
}

//O USUÃ�RIO ERROU A SEQUENCIA, ELE QUER CONTINUAR?
int errou_sequencia( ) { //0 NÃ£o quer continuar | 1 Continuar nivel 0
  String recebido_ble;
  int quer_continuar = 0;
  char* codigo_recebido = "C0";

  do {
    enviar_codigo("ES");
    codigo_recebido = receber_codigo(2);
  } while (codigo_recebido[0] != 'C');

  quer_continuar = (int)codigo_recebido[1] - 48;

  return quer_continuar; //0 NÃ£o quer continuar | 1 Continuar nivel 0
}

//QUAL FOI O NÃ�VEL SALVO NO APLICATIVO?
int qual_nivel(int exercicio) {

  int nivel = 0;
  char* codigo_recebido = "N0XX";

  if (exercicio == 1)
    do {
      enviar_codigo("N1");
      codigo_recebido = receber_codigo(4);
    } while (codigo_recebido[0] != 'N' || codigo_recebido[1] != '1');
  else
    do {
      enviar_codigo("N2");
      codigo_recebido = receber_codigo(4);
    } while (codigo_recebido[0] != 'N' || codigo_recebido[1] != '2');

  nivel = 10 * ((int)codigo_recebido[2] - 48) + 1 * ((int)codigo_recebido[3] - 48);

  return nivel;
}

//JOGO FINALIZADO, ENVIAR O NÃ�VEL PARA O APLICATIVO
int enviar_nivel(int nivel, int exercicio) {

  char* codigo_recebido;
  char codigo_a_enviar[5] = "V0XX";

  if (exercicio == 1)
    codigo_a_enviar[1] = '1';
  else
    codigo_a_enviar[1] = '2';
  do {
    codigo_a_enviar[2] = nivel / 10 + '0';
    codigo_a_enviar[3] = nivel % 10 + '0';
    enviar_codigo(codigo_a_enviar);
    codigo_recebido = receber_codigo(2);
  } while (strcmp(codigo_recebido, "SN") != 0);

  return 0;
}

//ENVIAR SEQUENCIA A SER APRESENTADA
int enviar_sequencia(int quantidade, int* vetor_de_dados) {

  char* codigo_recebido;
  char sequencia[36] = "SQXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXF";

  for (int i = 0 ; i < quantidade ; i++)
    sequencia[i + 2] = vetor_de_dados[i] + '0';
  sequencia[quantidade + 2] = 'F';
  sequencia[quantidade + 3] = '\0';
  do {
    enviar_codigo(sequencia);
    codigo_recebido = receber_codigo(2);
  } while (strcmp(codigo_recebido, "SR") != 0);


  return 0;
}

//AVISA QUE FINALIZOU A SEQUENCIA
int finalizou_sequencia() {
  char* codigo_recebido;

  do {
    codigo_recebido = receber_codigo(2);
  } while (strcmp(codigo_recebido, "SF") != 0);

  return 0;
}

//SABER SE O EXERCÃ�CIO 3 FOI FINALIZADO
int exercicio3_finalizado( ) {

  char* codigo_recebido;

  do {
    enviar_codigo("F3");
    codigo_recebido = receber_codigo(2);
  } while (strcmp(codigo_recebido, "F1") != 0);

  return 1;
}

//RECEBER O MODO DE MEDIDA DO DO GRAU DE TREMOR
int receber_modo_grau() {

  int grau = 0;
  char* codigo_recebido = "MTX";
  do {
    enviar_codigo("MT");
    codigo_recebido = receber_codigo(3);
  } while (codigo_recebido[0] != 'M' || codigo_recebido[1] != 'T');

  do {
    codigo_recebido = receber_codigo(2);
  } while (strcmp(codigo_recebido, "IT") != 0);

  grau = (int)codigo_recebido[2] - 48;

  return grau;
}

//ENVIAR O GRAU DE TREMOR PARA O CELULAR
int enviar_grau_tremor(int grau_tremor) {

  char codigo_a_enviar[4] = "GTX";
  char* codigo_recebido;

  codigo_a_enviar[2] = grau_tremor + '0';
  do {
    enviar_codigo(codigo_a_enviar);
    codigo_recebido = receber_codigo(3);
  } while (strcmp(codigo_recebido, "GTR") != 0);

  return 0;
}

//FUNÃ‡ÃƒO PARA ENVIAR CÃ“DIGO CHAR* PARA O CELULAR VIA BLE
void enviar_codigo(char* vetor) {

  pCharacteristic->setValue(vetor);
  Serial.print("Enviou :'");
  Serial.print(vetor);
  Serial.print("'\n");
}

//FUNÃ‡ÃƒO PARA RECEBER CÃ“DIGO CHAR* PARA O CELULAR VIA BLE
char* receber_codigo(int tamanho) {

  char* codigo_char = (char*)malloc((tamanho + 1) * sizeof(char));

  while (strcmp(codigo_BLE, "0000") == 0)
    delay(100);

  for (int i = 0 ; i < tamanho; i++)
    codigo_char[i] = codigo_BLE[i];
  codigo_char[tamanho] = '\0';

  strcpy(codigo_BLE, "0000");

  Serial.print("Recebeu pelo BLE :'");
  Serial.print(codigo_char);
  Serial.print("'\n");

  return codigo_char;
}

/*******************************************************************/

/*******************************************************************
                    FUNÃ‡Ã•ES DA COMUNICAÃ‡ÃƒO ENTRE
                    CONTROLADOR PRINCIPAL E BASE
 ******************************************************************/

int bs_receber_grau_tremor(int modo_de_medida) {
  char codigo_a_enviar[4] = "TTX";
  char* codigo_recebido;
  int grau = 5;

  do {
    wifi_enviar_bs("IT", 2);
    codigo_recebido = wifi_receber_bs(3);
  } while (strcmp(codigo_recebido, "AET") != 0);

  codigo_a_enviar[2] = modo_de_medida + '0';

  do {
    wifi_enviar_bs(codigo_a_enviar, 3);
    codigo_recebido = wifi_receber_bs(2);
  } while (codigo_recebido[0] != 'G');

  grau = (int)codigo_recebido[1] - '0';

  return grau;
}

char* wifi_receber_bs(int tamanho ) {
  
  char* codigo_char = (char*)malloc((tamanho + 1) * sizeof(char));

  while (strcmp(codigo_BLE_bs, "0000") == 0)
    delay(5);
  /*Serial.print("1.1 Recebou da base via wifi:'");
  Serial.print(codigo_BLE_bs);
  Serial.print("'");*/
  strcpy(codigo_char,codigo_BLE_bs);
  codigo_char[tamanho] = '\0';

  strcpy(codigo_BLE_bs, "0000");

  Serial.print("Recebou da base via wifi:'");
  Serial.print(codigo_char);
  Serial.print("'\n");

  return codigo_char;
}
int wifi_enviar_bs(char* string, int tamanho) {

  /*while (strcmp(wifi_receber_sc(2), "XX") != 0)
    delay(100);*/
  pCharacteristic_bs->setValue(string);
  while (strcmp(wifi_receber_bs(tamanho), string) != 0);

  pCharacteristic_bs->setValue("0000");
  Serial.print("Enviou para a base via wifi:'");
  Serial.print(string);
  Serial.print("'\n");

  return 0;
}

/******************************************************************/
/*******************************************************************
                        FUNÃ‡Ã•ES DA COMUNICAÃ‡ÃƒO ENTRE
                    CONTROLADOR PRINCIPAL E SECUNDARIO
 ******************************************************************/
//Novo botÃ£o 0-nÃ£o apertado 1-apertado
int sec_ler_botoes() {

  wifi_enviar_sc("NV", 2);

  return 0;
}

//qual botÃ£o 1 a 5
int sec_botao_ap() {

  int botao = 0;
  char* cod_recebidow;

  do {
    //enviar_secundario("QB");
    wifi_enviar_sc("QB", 2);
    cod_recebidow = wifi_receber_sc(2);
  } while (cod_recebidow[0] != 'B');

  botao = (int)cod_recebidow[1] - 48; // virou inteiro

  return botao;
}

//aviso que led vai acender - 0 nÃ£o vai 1 vai
int sec_led_acender() {

  wifi_enviar_sc("AL", 2);

  return 0;
}

//avisa qual led vai acender
int sec_led_on(int led) {

  char* cod_recebidow;
  char codigo_enviar[3] = "LX";

  Serial.println("entrou no sec_led_on"); 
  do {
    codigo_enviar[1] = led + '0';
    //enviar_secundario(codigo_enviar);
    wifi_enviar_sc(codigo_enviar, 2);
    cod_recebidow = wifi_receber_sc(2);
  } while (strcmp(cod_recebidow, "QL") != 0);


  return 0;
}

int acender_todos_leds(int tempo){
  char enviar[6] = "TLXXX";
  enviar[2] = (int)tempo/100 + '0';
  enviar[3] = (int)(tempo%100)/10 + '0';
  enviar[4] = (int)(tempo%100)%10 + '0';
  wifi_enviar_sc("TL", 2);
  wifi_enviar_sc(enviar, 5);

  return 0;
}

int acender_alguns_leds(int tempo){
  char enviar[] = "BLXXX";
  enviar[2] = (int)tempo/100 + '0';
  enviar[3] = (int)(tempo%100)/10 + '0';
  enviar[4] = (int)(tempo%100)%10 + '0';
  wifi_enviar_sc("BL", 2);
  wifi_enviar_sc(enviar, 5);

  return 0;
}


int acender_alguns_leds(int tempo){
  char enviar[] = "VLXXX";
  enviar[2] = (int)tempo/100 + '0';
  enviar[3] = (int)(tempo%100)/10 + '0';
  enviar[4] = (int)(tempo%100)%10 + '0';
  wifi_enviar_sc("VL", 2);
  wifi_enviar_sc(enviar, 5);

  return 0;
}

char* wifi_receber_sc(int tamanho ) {
  
  char* codigo_char = (char*)malloc((tamanho + 1) * sizeof(char));

  while (strcmp(codigo_BLE_sc, "0000") == 0)
    delay(5);
  /*Serial.print("1.1 Recebou da base via wifi:'");
  Serial.print(codigo_BLE_bs);
  Serial.print("'");*/
  strcpy(codigo_char,codigo_BLE_sc);
  codigo_char[tamanho] = '\0';

  strcpy(codigo_BLE_sc, "0000");

  Serial.print("Recebou do secundario via wifi:'");
  Serial.print(codigo_char);
  Serial.print("'\n");

  return codigo_char;
}
int wifi_enviar_sc(char* string, int tamanho) {

  /*while (strcmp(wifi_receber_sc(2), "XX") != 0)
    delay(100);*/
  pCharacteristic_sc->setValue(string);
  while (strcmp(wifi_receber_sc(tamanho), string) != 0);

  pCharacteristic_sc->setValue("0000");
  Serial.print("Enviou para o secundario via wifi:'");
  Serial.print(string);
  Serial.print("'\n");

  return 0;
}

/******************************************************************/
void setup() {

  //Definindo LEDS e BotÃµes
  Serial.begin(115200);
  for (int contador = 1; contador <= 5; contador++)
    pinMode(definir_LED(contador), OUTPUT);
  for (int contador = 1; contador <= 5; contador++)
    pinMode(definir_botao(contador), INPUT_PULLUP);

  //Definindo BLE
  BLEDevice::init("ESTER"); //Nome do BLE
  pServer = BLEDevice::createServer(); //Criando Servidor
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID); //Criando serviÃ§o
  
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

  //pCharacteristic->addDescriptor(new BLE2902());
  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->setValue("C");
  pCharacteristic->addDescriptor(new BLE2902());
  codigo_BLE = (char*)malloc(34 * sizeof(char));
  strcpy(codigo_BLE, "0000");
        
  pCharacteristic_bs = pService->createCharacteristic(
                         CHARACTERISTIC_BS_UUID,
                         BLECharacteristic::PROPERTY_READ   |
                         BLECharacteristic::PROPERTY_WRITE  |
                         BLECharacteristic::PROPERTY_NOTIFY |
                         BLECharacteristic::PROPERTY_INDICATE
                       );

  pCharacteristic_bs->setCallbacks(new MyCallbacks_bs());
  pCharacteristic_bs->setValue("B");
  pCharacteristic_bs->addDescriptor(new BLE2902());
  codigo_BLE_bs = (char*)malloc(6 * sizeof(char));
  strcpy(codigo_BLE_bs, "0000");

  pCharacteristic_sc = pService->createCharacteristic(
                         CHARACTERISTIC_SC_UUID,
                         BLECharacteristic::PROPERTY_READ   |
                         BLECharacteristic::PROPERTY_WRITE  |
                         BLECharacteristic::PROPERTY_NOTIFY |
                         BLECharacteristic::PROPERTY_INDICATE
                       );

  pCharacteristic_sc->setCallbacks(new MyCallbacks_sc());
  pCharacteristic_sc->setValue("S");
  pCharacteristic_sc->addDescriptor(new BLE2902());
  codigo_BLE_sc = (char*)malloc(6 * sizeof(char));
  strcpy(codigo_BLE_sc, "0000");
  
  pService->start();
  pAdvertising = pServer->getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  pAdvertising->start();

}

void loop() {
  int estado_atual = 5;
  int proximo_estado = 0;
  int modo = 0;
  int conectado = 0;
  int sequencia_inv[6] = {5, 4, 3, 2, 1, 0};

  Serial.println("Conectando aos dispositivos via BLE");
  
  Serial.print("Conectar com o secundário\n");
  while (strcmp(wifi_receber_sc(1), "S") != 0) {
    wifi_enviar_sc("S",1);
    delay(250);
  }
  Serial.print("secundária conectada\n");
    
  delay(250);
  acender_seq_leds(100);
  delay(10);
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
  
  Serial.print("Conectar com a base\n");
  while (strcmp(wifi_receber_bs(1), "B") != 0) {
    wifi_enviar_bs("B",1);
    delay(250);
  }
  Serial.print("base conectada\n");
  
  delay(250);
  acender_todos_leds(250);
  delay(10);
  digitalWrite(definir_LED(1), HIGH);
  digitalWrite(definir_LED(2), HIGH);
  digitalWrite(definir_LED(3), HIGH);
  digitalWrite(definir_LED(4), HIGH);
  digitalWrite(definir_LED(5), HIGH);
  delay(250);
  digitalWrite(definir_LED(1), LOW);
  digitalWrite(definir_LED(2), LOW);
  digitalWrite(definir_LED(3), LOW);
  digitalWrite(definir_LED(4), LOW);
  digitalWrite(definir_LED(5), LOW);
  
  Serial.print("Conectar com o smartphone\n");
  while (strcmp(receber_codigo(1), "C") != 0) {
    enviar_codigo("C");
    delay(250);
  }
  Serial.print("Conectado com o smartphone\n");

  delay(250);
  acender_alguns_leds(250);
  delay(10);
  digitalWrite(definir_LED(1), HIGH);
  digitalWrite(definir_LED(3), HIGH);
  digitalWrite(definir_LED(5), HIGH);
  delay(250);
  digitalWrite(definir_LED(1), LOW);
  digitalWrite(definir_LED(3), LOW);
  digitalWrite(definir_LED(5), LOW);
  digitalWrite(definir_LED(2), HIGH);
  digitalWrite(definir_LED(4), HIGH);
  delay(250);
  digitalWrite(definir_LED(2), LOW);
  digitalWrite(definir_LED(4), LOW);

  while (/*comunicacao(0, 1, sequencia_padrao) == 0*/1) {
    switch (estado_atual) {
      case 0:
        modo = comunicacao(0, 0, sequencia_padrao);
        proximo_estado = modo;
        //stand_by();
        break;
      case 1:
        exercicio1();
        proximo_estado = 0;
        break;
      case 2:
        exercicio2();
        proximo_estado = 0;
        break;
      case 3:
        exercicio3();
        proximo_estado = 0;
        break;
      case 4:
        pulseira();
        proximo_estado = 0;
        break;
      case 5:
        //stand_by();
        proximo_estado = 0;
        break;
    }
    estado_atual = proximo_estado;
    //delay(1000);
  }


}
