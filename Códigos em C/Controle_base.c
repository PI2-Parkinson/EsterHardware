#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define pi 3.14159265359

int* ler_sensor( int quantidade){

    int* leitura_sensor;
    int i;
    int amplitude = 3, offset = 70;

    leitura_sensor = malloc(quantidade*sizeof(int));
    for(i = 0; i < quantidade; i++){
        leitura_sensor[i] = amplitude*(sin(2*pi*1*((float)i/10))+ 0.2*cos(2*pi*4*((float)i/10)))+offset;
        //printf("Dado %d\n", leitura_sensor[i]);
    }

    return leitura_sensor;

}

int* ajuste_media(int* vetor_crr, int quant){

    int* vetor = vetor_crr;
    float media = (float) vetor[0];

    for(int i = 1; i <quant; i++){
        //printf("Dado %d\n", vetor[i]);
        media = (media + (float)vetor[i])/2;
    }

    for(int i = 0; i < quant ; i++)
        vetor[i] = (int)(vetor[i] - media);

    return vetor;

}

double* fft_emC(int* dados,int n){

    //double* tempo=new double[n];//armazena as 200 amostras igualmente espaçadas

    double tfr[n]; // armazena a parte real do sinal
    double tfi[n]; // armazena a parte imaginária do sinal
    double* tf_abs;

    tf_abs = malloc(n*sizeof(double));

    for(int u=1;u<(n/2)+1;u++) {
        tfr[u]=0;
        tfi[u]=0;
        for(int x=0;x<n;x++){
            tfr[u]+=dados[x]*cos(6.2831853*u*(double)x/(double)n);
            tfi[u]-=dados[x]*sin(6.2831853*u* (double)x/(double)n);
        }
        tfr[u]/=(double)n;
        tfi[u]/=(double)n;

        //printf("%d |   %.5lf  %.5lf\n",u,tfr[u],tfi[u]);
    }

    for(int x=0;x<(n/2+1);x++){
        tf_abs[x] = sqrt(pow(tfr[x],2)+pow(tfi[x],2));
        //printf("%d |   %.2lf\n",x,tf_abs[x]);
    }

    return tf_abs;
}

int grau_tremor(int modo, double* fft_dado, int tamanho){

    float alfa, beta;
    double amplitude = 0;
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

    for(int i = 5; i < tamanho; i++)
        if(amplitude<fft_dado[i])
            amplitude = fft_dado[i];
    amplitude = 2*amplitude; // em Graus
    printf("Amplitude : %.4lf grau\n",amplitude);
    amplitude = 1*sqrt(2*(1-cos(amplitude/180*pi)));
    printf("Amplitude : %.4lf cm\n",amplitude);

    grau = (int)((log10(amplitude)-beta)/alfa);

    return grau;
}

int comunicacao_strap(int modo, int dado){
    int retorno;
    switch(modo){
    case 0: //Foi pedido leitura?
        printf("Sera feito leitura? 0 - nao e 1 - sim ");
        scanf("%d", &retorno);
        break;
    case 1: //Qual modo?
        printf("Qual o modo? 1, 2 ou 3? ");
        scanf("%d", &retorno);
        break;
    case 2:
        printf("Grau : %d\n", dado);
        break;
    default:
        printf("Comando invalido\n");
        break;
    }

    return retorno;
}

int main(){

    int quant_dados = 1000, grau, modo;
    int* dados_sensor, dados_sensor_crr;
    double* fft_dados_sensor;
    //FILE* f_dados = fopen("dados_fft_sensor_v4.txt","w+");

    while (1){
        if(comunicacao_strap(0,0) == 1){
            dados_sensor = ler_sensor(quant_dados);
            dados_sensor_crr = ajuste_media(dados_sensor, quant_dados);
            fft_dados_sensor = fft_emC(dados_sensor_crr, quant_dados);
            modo = comunicacao_strap(1,0);
            grau = grau_tremor(modo,fft_dados_sensor,quant_dados/2+1);
            comunicacao_strap(2,grau);
        }

    }


    //for(int i = 0; i < quant_dados/2+1; i++)
        //fprintf(f_dados,"%d, %.4lf\n",i,fft_dados_sensor[i]);

    //fclose(f_dados);

    //printf("terminou a execucao, Grau: %d\n",grau);

    return 0;

}
