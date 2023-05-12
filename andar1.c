#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include "cJSON.h"
#include <bcm2835.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>

// Botão para simular a chegada de um carro = BEntrada
// 8x Botão para remover carro = BRemove[8]
// Sensor de presença de cada vaga {input} = A[8]
// 8x Sinal de vaga livre - led verde {output} = ALivre[8]
// 8x Sinal de vaga ocupada - led vermelho {output} = AOcup[8]

int vagasOcup = 0;
// ConfiguraPins
int CancEntr = 10, //MOTOR_CANCELA_ENTRADA
    CancSai = 17; //MOTOR_CANCELA_SAIDA
// Sensores
int  EntAbr = 23,  //SENSOR_ABERTURA_CANCELA_ENTRADA
    EntFec = 24, //SENSOR_FECHAMENTO_CANCELA_ENTRADA
    SaiAbr = 25, //SENSOR_ABERTURA_CANCELA_SAIDA
    SaiFec = 12; //SENSOR_FECHAMENTO_CANCELA_SAIDA
//Botoes
int BEntrada = 0, BRemove[8] =0;
//Leds
int ALivre[8] = 1, AOcup[8] = 0, 
    lotado = 27;  //SINAL_DE_LOTADO_FECHADO

void setPins(){
    // Define botões e configura entrada como Pull-down
    bcm2835_gpio_fsel(BEntrada, BCM2835_GPIO_FSEL_INPUT);
    bcm2835_gpio_set_pud(BEntrada, BCM2835_GPIO_PUD_DOWN);
    for(int i = 0; i < 8; i++){
        bcm2835_gpio_fsel(BRemove[i], BCM2835_GPIO_FSEL_INPUT);
        bcm2835_gpio_set_pud(BRemove[i], BCM2835_GPIO_PUD_DOWN);
    }
    // Define sensores e configura entradas com Pull-Down
    bcm2835_gpio_fsel(EntAbr, BCM2835_GPIO_FSEL_INPUT);
    bcm2835_gpio_set_pud(EntAbr, BCM2835_GPIO_PUD_DOWN);
    bcm2835_gpio_fsel(EntFec, BCM2835_GPIO_FSEL_INPUT);
    bcm2835_gpio_set_pud(EntFec, BCM2835_GPIO_PUD_DOWN);
    bcm2835_gpio_fsel(SaiAbr, BCM2835_GPIO_FSEL_INPUT);
    bcm2835_gpio_set_pud(SaiAbr, BCM2835_GPIO_PUD_DOWN);
    bcm2835_gpio_fsel(SaiFec, BCM2835_GPIO_FSEL_INPUT);
    bcm2835_gpio_set_pud(SaiFec, BCM2835_GPIO_PUD_DOWN);
    for( int i = 0; i < 8; i++){
        bcm2835_gpio_fsel(A[i], BCM2835_GPIO_FSEL_INPUT);
        bcm2835_gpio_set_pud(A[i], BCM2835_GPIO_PUD_DOWN);
    }
    // Pinos dos LEDs de saída
    for(int i = 0; i < 8; i++){
        bcm2835_gpio_fsel(ALivre[i], BCM2835_GPIO_FSEL_OUTP);
        bcm2835_gpio_fsel(AOcup[i], BCM2835_GPIO_FSEL_OUTP);
    }
    bcm2835_gpio_fsel(lotado, BCM2835_GPIO_FSEL_OUTP);

    // Motores das cancelas
    bcm2835_gpio_fsel(CancEntr, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(CancSai, BCM2835_GPIO_FSEL_OUTP);
}

void abreCancelaEntrada (){
    bcm2835_gpio_write(CancEntr, HIGH);
    bcm2835_delay(800); // espera 500 ms
    printf("Cancela Aberta\n");
}
void fechaCancelaEntrada(){
        bcm2835_gpio_write(CancEntr, LOW);
        bcm2835_delay(800); // espera 500 ms
         printf("Cancela Fechada\n");
}
// Ocupar vaga {Liga led Ocup e desliga livre}
void ocupaVaga (int A[i], int i){
    bcm2835_gpio_write(AOcup, HIGH);
    bcm2835_gpio_write (ALivre, LOW);
    vagasOcup++;
}
// Libera Vaga {Desliga led Ocup e liga livre}
void liberaVaga (int A[i], int i){
    bcm2835_gpio_write(AOcup, LOW);
    bcm2835_gpio_write (ALivre, HIGH);
    vagasOcup--;
}
void abreCancelaSaida (){
    bcm2835_gpio_write(CancSai, HIGH);
    bcm2835_delay(800); // espera 500 ms
    printf("Cancela Aberta\n");
}
void fechaCancelaSaida(){
    bcm2835_gpio_write(CancSai, LOW);
    bcm2835_delay(800); // espera 500 ms
    printf("Cancela Fechada\n");
}
// Lotado {Liga luz de lotado}
void andarLotado (int vagasOcup){
    if(vagasOcup == 7){
        bcm2835_gpio_write(lotado, HIGH);
    }
    else{
        bcm2835_gpio_write(lotado, LOW);
    }
}

int main(int argc, char **argv){
    if (!bcm2835_init())
	    return 1;

    if(argc < 2){
        printf("Precisa passar o arquivo a ser lido ao programa!\n");
        exit(0);
    }

    
}