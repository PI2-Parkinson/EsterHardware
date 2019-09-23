#include <stdio.h>
#include <stdlib.h>

int comunicacao_sec(int opcao, int escolha){
    int retorno;
    switch(opcao){
    case 0:
        break;
    case 1:
        printf("Enviar botao %d\n", escolha);
        break;
    case 2:
        printf("Qual led vai acender? ");
        scanf("%d",&retorno);
        break;
    default:
        break;
    }

}

int ler_botoes(){
    int botao
    for (i = 0; i < 5 ; i++){
        printf("O botao %d foi apertado? 0 nao e 1 sim\n");
        scanf("%d", &apertou);
        if(apertou == 1){
            botao = i+1;

            // ACENDE O LED
            acender_leds(botao,50);

            // ENVIA O DADO PARA A OUTRA ESFERA
            comunicacao(1,botao);
        }
    }
    printf("Qual botao apertou? ");
    scanf("%d",&botao);
    acender_leds(botao, 50);
    return botao;
}
int acender_leds(int led, int tempo){
    printf("LED %d aceso", led);
    delay_ms(tempo);
}

void delay_ms(int tempo){

}



int main(){

    int estado_atual, proximo_estado = 0;
    int led_ou_botao; //0 - LED  1 - Botao
    int botao;
    while(comunicacao_sec(0,1)== 0){
        switch(estado_atual){
        case 0:
            led_ou_botao = comunicacao_sec(0,0);
            proximo_estado = led_ou_botao+1;
            break;
        case 1:
            botao = ler_botoes();
            comunicacao_sec(1,botao);
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
    }

    return 0;
}
