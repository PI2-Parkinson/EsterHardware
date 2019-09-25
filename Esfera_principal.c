#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int sequencia_padrao[6] = {1,2,3,4,5,0};

int comunicacao(int modo_de_op, int escolha, int* vetor_de_dados){
    int retorno;
    int i;
    switch(modo_de_op){
    case 0: // modo principal
        switch(escolha){
        case 0:
            printf("Qual modo? ");
            scanf("%d", &retorno);
            break;
        case 1:
            printf("O jogo foi finalizado? 0 nao e 1 sim ");
            scanf("%d", &retorno);
            break;
        default:
            printf("Comando invalido!\n");
            break;
        }
        break;
    case 1: // exercício 1
        switch(escolha){
        case 1:
            printf("O botao de para foi apertado? 0- nao 1- sim\n");
            scanf("%d",&retorno);
            break;
        case 2:
            printf("Ele errou, deseja continuar? 0- nao 1- sim \n");
            scanf("%d",&retorno);
            break;
        case 3:
            printf("Qual nivel? ");
            scanf("%d", &retorno);
            break;
        case 4:
            printf("Nivel: %d\n",vetor_de_dados[0]);
            break;
        case 5:
            i = 0;
            printf("Toca sequencia: ");
            while(vetor_de_dados[i]!=0){
                printf("%d ",vetor_de_dados[i]);
                i++;
            }
            printf("\n");
            break;
        default:
            printf("Comando invalido!\n");
            break;
        }
        break;
    case 2: // exercício 2
        switch(escolha){
        case 1:
            printf("O botao de para foi apertado? 0- nao 1- sim\n");
            scanf("%d",&retorno);
            break;
        case 2:
            printf("Ele errou, deseja continuar? 0- nao 1- sim \n");
            scanf("%d",&retorno);
            break;
        case 3:
            printf("Qual nivel? ");
            scanf("%d", &retorno);
            break;
        case 4:
            printf("Nivel: %d\n",vetor_de_dados[0]);
            break;
        case 5:
            i = 0;
            printf("Mostra sequencia: ");
            while(vetor_de_dados[i]!=0){
                printf("%d ",vetor_de_dados[i]);
                i++;
            }
            printf("\n");
            break;
        default:
            printf("Comando invalido!\n");
            break;
        }
        break;
    case 3: // exercício 3
        switch(escolha){
        case 0:
            printf("Finalizou o jogo? 0 - nao e 1 - sim");
            scanf("%d",&retorno);
            break;
        default:
            break;
        }
        break;
    case 4: // pulseira
        switch(escolha){
        case 0:
            printf("O dado foi enviado para o software? 0 - nao e 1 - sim ");
            scanf("%d",&retorno);
            break;
        default:
            printf("Comando invalido!\n");
            break;
        }
        break;
    case 5: // Leitura dos botões da esf secundaria
        switch(escolha){
        case 1:
            retorno = 0;
            while(retorno == 0){
                printf("Novo botao foi apertado na esf sec? 0 nao e 1 sim ");
                scanf("%d",&retorno);
                if(retorno == 1){
                    printf("Qual botao da esf sec foi apertado? ");
                    scanf("%d",&retorno);
                }

            }
            break;
        default:
            printf("Comando invalido!\n");
            break;
        }
        break;
    case 6: // acender leds da esfera secundaria
        switch(escolha){
        case 1:
            printf("LED esf sec %d aceso\n",vetor_de_dados[0]);
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

        for (count = nivel; count <= 32 && deseja_continuar == 1; count++)
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

        for (count = nivel; count <= 32 && deseja_continuar == 1; count++)
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

            //ACENDE LEDS DA SEQUENCIA
            i = 0;
            while (sequencia[i] != 0){
                acender_leds(sequencia[i],333);
                i++;
            }

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
        comunicacao(1,4,&nivel);

}
void exercicio3(){
    while(comunicacao(3,0,sequencia_padrao) == 0);
}
void pulseira(){

}


int ler_botoes(int nivel, int* sequencia){
    printf("Digite a seq de botoes\n");
    int botao, botao_sec, apertou = 0;
    int errou = 0;
    int i, cont = 0;

    // LER CADA BOTAO E VERIFICA SE ERROU
    while(cont < nivel){

        // VERIFICAR TODOS OS CINCO BOTOES
        for (i = 0; i < 5 ; i++){
            printf("O botao %d foi apertado? 0 nao e 1 sim\n",i+1);
            scanf("%d", &apertou);
            if(apertou == 1){
                botao = i+1;

                //ACENDE O LED
                acender_leds(botao,50);

                //VERIFICA SE O BOTAO DA ESF SEC FOI APERTADO
                botao_sec = comunicacao(5,1,sequencia_padrao);

                //VERIFICA SE ESTA CORRETO
                if(sequencia[cont] == botao && sequencia[cont] == botao_sec && errou == 0)
                    errou = 0;
                else
                    errou = 1;
                cont ++;
            }
        }
    }

    // QUANTO TEMPO DEMOROU PARA LER CADA BOTAO

    return errou;
}
void acender_leds(int LED, int tempo){
    int vetor_led[3] = {LED,tempo, 0};

    printf("LED %d aceso\n", LED);
    delay_ms(tempo);

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

}


int main(void){

	int estado_atual = 5;
	int proximo_estado = 0;
	int modo = 0;
    printf("Vai iniciar o while\n");
	while(comunicacao(0,1,sequencia_padrao)==0){
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
				//pulseira();
				while(comunicacao(4,0,sequencia_padrao) == 0);
				proximo_estado = 0;
				break;
			case 5:
				//stand_by();
				proximo_estado = 0;
				break;
		}
		estado_atual = proximo_estado;
	}

	return 0;
}



