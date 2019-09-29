#include <stdio.h>
#include <stdlib.h>

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

void setup() {
  Serial.begin(115200); 
  for (int contador = 1; contador <= 5; contador++)
    pinMode(definir_LED(contador), OUTPUT);
  for (int contador = 1; contador <= 5; contador++)
    pinMode(definir_botao(contador), INPUT_PULLUP);
    

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

int comunicacao_sec(int opcao, int escolha){
    int retorno;
    switch(opcao){
    case 0:
        if(escolha == 0){
            Serial.print("Ler botao (0) ou acender LED (1)\n");
            ler_dado(&retorno);
        }else if(escolha == 1){
            Serial.print("O jogo foi finalizado? 0 - nao e 1 - sim\n");
            ler_dado(&retorno);
        }else{
            Serial.print("Comando inválido\n");
        }
        break;
    case 1:
        Serial.print("Enviar botao ");
        Serial.println(escolha);
        break;
    case 2:
        Serial.print("Qual led vai acender? ");
        ler_dado(&retorno);
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



/**************FUNCAO PRINCIPAL***********************/
void loop() {
    int estado_atual = 0;
    int proximo_estado = 0;
    int led_ou_botao = 0; //0 - LED  1 - Botao
    int botao;
    
    while(comunicacao_sec(0,1)== 0){
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
        default:
            break;
        }
        estado_atual = proximo_estado;
        //delay(1000);
    }
}
