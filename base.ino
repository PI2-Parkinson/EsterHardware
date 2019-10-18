#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define pi 3.14159265359

int** ler_sensor( int quantidade){

    int** leitura_sensor;
    int i;
    //int amplitude = 3, offset = 70;

    leitura_sensor = (int**)malloc(3*sizeof(int*));
    
    leitura_sensor[0] = (int*) malloc(quantidade*sizeof(int));
    leitura_sensor[1] = (int*) malloc(quantidade*sizeof(int));
    leitura_sensor[2] = (int*) malloc(quantidade*sizeof(int));
    
    for(i = 0; i < quantidade; i++){
      mpu6050.update();
      leitura_sensor[0][i] = mpu6050.getAngleX();
      leitura_sensor[1][i] = mpu6050.getAngleY();
      leitura_sensor[2][i] = mpu6050.getAngleZ();

      Serial.print(i);
      Serial.print(" | angulo X : ");
      Serial.println(leitura_sensor[0][i]);
      
      delay(100); 
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

    tf_abs = (double*)malloc((n/2+1)*sizeof(double));

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
        Serial.print(x);
        Serial.print(" | FFT : ");
        Serial.println(tf_abs[x]);
    }

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
      for(int i = 5; i < tamanho; i++)
          if(amplitude[j]<fft_dado[j][i])
              amplitude[j] = fft_dado[j][i];
      amplitude[j] = 2*amplitude[j]; // em Graus
      amplitude[j] = 1*sqrt(2*(1-cos(amplitude[j]/180*pi)));
    }

    amplitude_total = sqrt(pow(amplitude[0],2)+pow(amplitude[1],2)+pow(amplitude[2],2));
    grau = (int)((log10(amplitude_total)-beta)/alfa); // Grau vai de 0 a 4
    
    if(grau > 4){
      grau = 4;
    }else if (grau<0){
      grau = 0;
    }
    
    return grau;
}

int comunicacao_strap(int modo, int dado){
    int retorno;
    switch(modo){
    case 0: //Foi pedido leitura?
        Serial.print("Sera feito leitura? 0 - nao e 1 - sim ");
        ler_dado(&retorno);
        break;
    case 1: //Qual modo?
        Serial.print("Qual o modo? 1, 2 ou 3? ");
        ler_dado(&retorno);
        break;
    case 2:
        Serial.print("Grau : ");
        Serial.println(dado);
        break;
    default:
        Serial.print("Comando invalido\n");
        break;
    }

    return retorno;
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



void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}

void loop() {
  int quant_dados = 100, grau, modo;
  int** dados_sensor;
  int*dados_sensor_crr;
  double* fft_dados_sensor[3];
  //FILE* f_dados = fopen("dados_fft_sensor_v4.txt","w+");

  while (1){
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

    }

}
