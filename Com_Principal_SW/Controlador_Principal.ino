#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Bibliotecas do BLE
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

//ENTRADAS E SAIDAS
//LEDs e botões
#define LEDVermelho 13
#define LEDLaranja 12
#define LEDAmarelo 14
#define LEDVerde 02
#define LEDAzul 04

#define ButtonVermelho 25
#define ButtonLaranja 33
#define ButtonAmarelo 32
#define ButtonVerde 27
#define ButtonAzul 26

//BLE
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;

char* codigo_BLE;

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();
      
      strcpy(codigo_BLE,value.c_str());
      //Serial.println("Salvou o dado");
    }
};



void setup() {

  //Definindo LEDS e Botões
  Serial.begin(115200); 
  for (int contador = 1; contador <= 5; contador++)
    pinMode(definir_LED(contador), OUTPUT);
  for (int contador = 1; contador <= 5; contador++)
    pinMode(definir_botao(contador), INPUT_PULLUP);

  //Definindo BLE
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
  codigo_BLE = (char*)malloc(34*sizeof(char));
  strcpy(codigo_BLE,"0000");
  
}

int sequencia_padrao[6] = {1,2,3,4,5,0};

/*******************************************************************
 *                      FUNÇÕES PRINCIPAIS 
 *                    DO CONTROLADOR PRINCIPAL 
 ******************************************************************/
int comunicacao(int modo_de_op, int escolha, int* vetor_de_dados){
    int retorno;
    int i;
    switch(modo_de_op){
    case 0: // modo principal
        switch(escolha){
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
    case 1: // exercício 1
        switch(escolha){
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
            retorno = enviar_nivel(vetor_de_dados[0],1);
            break;
        case 5:
            i = 0;
            while(vetor_de_dados[i]!=0){
                Serial.println(vetor_de_dados[i]);
                i++;
            }
            Serial.println(vetor_de_dados[i]);
            enviar_sequencia(i,vetor_de_dados);
            retorno = finalizou_sequencia();
            break;
        default:
           Serial.print("Comando invalido!\n");
            break;
        }
        break;
    case 2: // exercício 2
        switch(escolha){
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
            retorno = enviar_nivel(vetor_de_dados[0],2);
            break;
        case 5:
            i = 0;
            while(vetor_de_dados[i]!=0){
                i++;
            }
            enviar_sequencia(i,vetor_de_dados);
            for(int j = 0 ; i < i ; j++)
              acender_leds(vetor_de_dados[j],333);
            retorno = finalizou_sequencia();
            break;
        default:
           Serial.print("Comando invalido!\n");
            break;
        }
        break;
    case 3: // exercício 3
        switch(escolha){
        case 0:
            retorno = exercicio3_finalizado();
            break;
        default:
            break;
        }
        break;
    case 4: // pulseira
        switch(escolha){
        case 0:
            retorno = enviar_grau_tremor(3); //ADICIONAR FUNÇÃO QUE RECEBE O GRAU NO LUGAR DO 3
            break;
        default:
           Serial.print("Comando invalido!\n");
            break;
        }
        break;
    case 5: // Leitura dos botões da esf secundaria
        switch(escolha){
        case 1:
            retorno = 0;
            while(retorno == 0){
               Serial.print("Novo botao foi apertado na esf sec? 0 nao e 1 sim ");
                ler_dado(&retorno);
                if(retorno == 1){
                   Serial.print("Qual botao da esf sec foi apertado? ");
                    ler_dado(&retorno);
                }

            }
            break;
        default:
           Serial.print("Comando invalido!\n");
            break;
        }
        break;
    case 6: // acender leds da esfera secundaria
        switch(escolha){
        case 1:
           Serial.print("LED esf sec ");
           Serial.print(vetor_de_dados[0]);
           Serial.print("aceso\n");
            delay_ms(vetor_de_dados[1]);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    return retorno;
}

/********* EXERCÍCIOS ************/

void exercicio1(){
        //INICIO
        int semente_rand = time(NULL);
        int count, nivel;
        int i, j = 0;
        const int lower = 1, upper = 5;
        int sequencia[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        int deseja_continuar = 1;

        //DEMONSTRAÇÃO
        comunicacao(1,5,sequencia_padrao);

        for (i = 0; i < 5; i++){
            acender_leds(i+1,333);
        }

        //RECEBE NÍVEL
        nivel = comunicacao(1,3,sequencia_padrao);

        for (count = nivel; count < 32 && deseja_continuar == 1; count++)
        {
            //GERA SEQUÊNCIA ALEATÓRIA
            srand(semente_rand);
            if (count == nivel)
                for (j = 0; j < count; j++)
                    sequencia[j] = printRandoms(lower, upper, count);
            else
                sequencia[count - 1] = printRandoms(lower, upper, count);

            //MANDA SEQUÊNCIA
            comunicacao(1,5,sequencia);

            //LER BOTÕES E ACENDE LEDS
            if(ler_botoes(count,sequencia) == 1){

                //ERROU, DESEJA CONTINUAR?
                deseja_continuar = comunicacao(1,2,sequencia_padrao);
                count = 0;
                nivel = 1;
                for(i=0;i<32;i++)
                    sequencia[i] = 0;
            }else{

                //ACERTOU, DESEJA CONTINUAR?
                deseja_continuar = 1-comunicacao(1,1,sequencia_padrao);
            }
        }

        //MANDA NÍVEL
        nivel = count;
        comunicacao(1,4,&nivel);

}
void exercicio2(){
        //INICIO
        int semente_rand = time(NULL);
        int count, nivel;
        int i, j = 0;
        const int lower = 1, upper = 5;
        int sequencia[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        int deseja_continuar = 1;

        //DEMONSTRAÇÃO
        comunicacao(2,5,sequencia_padrao);

        //ACENDE LEDS DA SEQUENCIA
        for (i = 0; i < 5; i++){
            acender_leds(i+1,333);
        }

        //RECEBE NÍVEL
        nivel = comunicacao(2,3,sequencia_padrao);

        for (count = nivel; count < 32 && deseja_continuar == 1; count++)
        {

            //GERA SEQUÊNCIA ALEATÓRIA
            srand(semente_rand);
            if (count == nivel)
                for (j = 0; j < count; j++)
                    sequencia[j] = printRandoms(lower, upper, count);
            else
                sequencia[count - 1] = printRandoms(lower, upper, count);

            //MANDA SEQUÊNCIA
            comunicacao(2,5,sequencia);

            //ACENDE LEDS DA SEQUENCIA
            i = 0;
            while (sequencia[i] != 0){
                acender_leds(sequencia[i],333);
                i++;
            }

            //LER BOTÕES E ACENDE LEDS
            if(ler_botoes(count,sequencia) == 1){

                //ERROU, DESEJA CONTINUAR?
                deseja_continuar = comunicacao(2,2,sequencia_padrao);
                count = 0;
                nivel = 1;
                for(i=0;i<32;i++)
                    sequencia[i] = 0;
            }else{

                //ACERTOU, DESEJA CONTINUAR?
                deseja_continuar = 1-comunicacao(2,1,sequencia_padrao);
            }
        }

        //MANDA NÍVEL
        nivel = count;
        comunicacao(2,4,&nivel);

}
void exercicio3(){
    while(comunicacao(3,0,sequencia_padrao) == 0);
}
void pulseira(){
    while(comunicacao(4,0,sequencia_padrao) == 0);
}


/***** BOTÕES E LEDS ************/
int ler_botoes(int nivel, int* sequencia){
   Serial.print("Digite a seq de botoes\n");
    int botao, botao_sec, apertou = 0;
    int errou = 0;
    int i, cont = 0;
    int porta = 0;

    // LER CADA BOTAO E VERIFICA SE ERROU
    while(cont < nivel){

        // VERIFICAR TODOS OS CINCO BOTOES
        for (i = 0; i < 5 ; i++){
            //printf("O botao %d foi apertado? 0 nao e 1 sim\n",i+1);
            //ler_dado(&apertou);
            
            apertou = not(digitalRead(definir_botao(i+1)));
            
            if(apertou == 1){
                delay_ms(200);
                botao = i+1;

                //ACENDE O LED
                acender_leds(botao,50);

                //VERIFICA SE O BOTAO DA ESF SEC FOI APERTADO
                //botao_sec = comunicacao(5,1,sequencia_padrao);
                botao_sec = botao;

                //VERIFICA SE ESTA CORRETO
                if(sequencia[cont] == botao && sequencia[cont] == botao_sec && errou == 0)
                    errou = 0;
                else
                    errou = 1;
                cont++;
            }
        }
    }

    // QUANTO TEMPO DEMOROU PARA LER CADA BOTAO

    return errou;
}
void acender_leds(int LED, int tempo){
    int vetor_led[3] = {LED,tempo, 0};

    //printf("LED %d aceso\n", LED);
    digitalWrite(definir_LED(LED), HIGH);
    delay_ms(tempo);
    digitalWrite(definir_LED(LED),LOW);

    comunicacao(6, 1, vetor_led);
}


int printRandoms(int lower, int upper, int count){
    int i, num = 0;
    for (i = 0; i < count; i++) {
        num = (rand() % (upper - lower + 1)) + lower;
    }
    return num;
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
/*******************************************************************/


/*******************************************************************
 *                      FUNÇÕES DA COMUNICAÇÃO ENTRE
 *                  CONTROLADOR PRINCIPAL E SMARTPHONE 
 ******************************************************************/
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


/*******************************************************************/

void loop() {
  int estado_atual = 5;
  int proximo_estado = 0;
  int modo = 0;
  Serial.print("Conectar com o bluetooth\n");
  while(strcmp(receber_codigo(1),"C") != 0){
    enviar_codigo("C");
    delay(250);
  }
  Serial.print("bluetooth conectado\n");
    
  while(comunicacao(0,1,sequencia_padrao) == 0){
    switch(estado_atual){
      case 0:
        modo = comunicacao(0,0,sequencia_padrao);
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
