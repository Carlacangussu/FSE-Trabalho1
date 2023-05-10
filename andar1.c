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
// [ENTRADA] Sensor de presença p/Abrir {input} = EntAbr
// [ENTRADA] Sensor de passagem p/Fechar {input} = EntFec
// Motor cancela de entrada = CancEntr
// Sinal de lotado {output} = lotado
// Sensor de presença de cada vaga {input} = A[8]
// 8x Sinal de vaga livre - led verde {output} = ALivre[8]
// 8x Sinal de vaga ocupada - led vermelho {output} = AOcup[8]
// [SAÍDA] Sensor de presença p/ Abrir {input} = SaiAbr
// [SAÍDA] Sensor de passagem p/ Fechar {input} =SaiFec
// Motor cancela de saída = CancSai

int vagasOcup = 0, CancEntr = 0, CancSai = 0;
// Sensores
int A[8], EntAbr, EntFec, SaiAbr, SaiFec;
//Botoes
int BEntrada = 0, BRemove[8] =0;
//Leds
int ALivre[8] = 1, AOcup[8] = 0, lotado = 0,

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
        bcm2835_gpio_fsel(lotado, BCM2835_GPIO_FSEL_OUTP);
    }
    // Motores das cancelas
    bcm2835_gpio_fsel(CancEntr, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(CancSai, BCM2835_GPIO_FSEL_OUTP);
}

// Entrar
void cancelaEntrada (int EntAbr, int EntFec){
    if(EntAbr == 1){
        bcm2835_gpio_write(CancEntr, HIGH);
        EntAbr = 1;
        EntFec = 0;
    }
    else if(EntFec == 1){
        bcm2835_gpio_write(CancEntr, LOW);
        EntAbr = 0;
        EntFec = 1;
    }
}
// Ocupar vaga {Liga led Ocup e desliga livre}
void ocupaVaga (int A[i], int i){
    bcm2835_gpio_write(AOcup, HIGH);
    bcm2835_gpio_write (ALivre, LOW);
    A[i] = 1;
    vagasOcup++;
}
// Libera Vaga {Desliga led Ocup e liga livre}
void liberaVaga (int A[i], int i){
    bcm2835_gpio_write(AOcup, LOW);
    bcm2835_gpio_write (ALivre, HIGH);
    A[i] = 0;
    vagasOcup--;
}
// Sair
void cancelaEntrada (int SaiAbr, int SaiFec){
    if(SaiAbr == 1){
        bcm2835_gpio_write(CancSai, HIGH);
        SaiAbr = 1;
        SaiFec = 0;
    }
    else if(EntFec == 1){
        bcm2835_gpio_write(CancSai, LOW);
        SaiAbr = 0;
        SaiFec = 1;
    }
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