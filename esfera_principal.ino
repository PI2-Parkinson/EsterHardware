#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

int sequencia_padrao[6] = {1,2,3,4,5,0};

int comunicacao(int modo_de_op, int escolha, int* vetor_de_dados){
    int retorno;
    int i;
    switch(modo_de_op){
    case 0: // modo principal
        switch(escolha){
        case 0:
           Serial.print("Qual modo? ");
            ler_dado(&retorno);
            break;
        case 1:
           Serial.print("O jogo foi finalizado? 0 nao e 1 sim ");
            ler_dado(&retorno);
            break;
        default:
           Serial.print("Comando invalido!\n");
            break;
        }
        break;
    case 1: // exercício 1
        switch(escolha){
        case 1:
           Serial.print("O botao de para foi apertado? 0- nao 1- sim\n");
            ler_dado(&retorno);
            break;
        case 2:
           Serial.print("Ele errou, deseja continuar? 0- nao 1- sim \n");
            ler_dado(&retorno);
            break;
        case 3:
           Serial.print("Qual nivel? ");
            ler_dado(&retorno);
            break;
        case 4:
           Serial.print("Nivel: ");
           Serial.println(vetor_de_dados[0]);
            break;
        case 5:
            i = 0;
           Serial.print("Toca sequencia: ");
            while(vetor_de_dados[i]!=0){
               Serial.print(vetor_de_dados[i]);
                i++;
            }
           Serial.print("\n");
            break;
        default:
           Serial.print("Comando invalido!\n");
            break;
        }
        break;
    case 2: // exercício 2
        switch(escolha){
        case 1:
           Serial.print("O botao de para foi apertado? 0- nao 1- sim\n");
            ler_dado(&retorno);
            break;
        case 2:
           Serial.print("Ele errou, deseja continuar? 0- nao 1- sim \n");
            ler_dado(&retorno);
            break;
        case 3:
           Serial.print("Qual nivel? ");
            ler_dado(&retorno);
            break;
        case 4:
           Serial.print("Nivel: ");
           Serial.println(vetor_de_dados[0]);
            break;
        case 5:
            i = 0;
           Serial.print("Mostra sequencia: ");
            while(vetor_de_dados[i]!=0){
               Serial.print(vetor_de_dados[i]);
                i++;
            }
           Serial.print("\n");
            break;
        default:
           Serial.print("Comando invalido!\n");
            break;
        }
        break;
    case 3: // exercício 3
        switch(escolha){
        case 0:
           Serial.print("Finalizou o jogo? 0 - nao e 1 - sim");
            ler_dado(&retorno);
            break;
        default:
            break;
        }
        break;
    case 4: // pulseira
        switch(escolha){
        case 0:
           Serial.print("O dado foi enviado para o software? 0 - nao e 1 - sim ");
            ler_dado(&retorno);
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





/*****************************************/

void loop() {
  int estado_atual = 5;
  int proximo_estado = 0;
  int modo = 0;
   Serial.print("Vai iniciar o while\n");
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
