#include <stdio.h> 
#include <string.h>   //strlen 
#include <stdlib.h> 
#include <errno.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros 
#include "cJSON.h"

#define TRUE   1 
#define FALSE  0
#define TAXA 0.15 

void interface(){
    int andar1 = 0, andar2 =0, andar3=0, total_carros=0; // Vagas ocupadas em cada andar e no estacionamento 
    int dispA, dispB, dispC;// Número de Vagas disponíveis em cada andar;
    float tarifa, entrada, saida ///A taxa por minuto deve ser R$ 0,15 (quize centavos por minuto);

    while(1){
        //int resp1 = Repostas quantas vagas estão ocupadas no andar 1
        if(andar1 != resp1 ){
            andar1 = resp1;
            printf("Vagas ocupadas no andar 1: %d\n", andar1);
            dispA = 8 - andar1;
            printf("Vagas disponíveis no andar 1: %d\n", dispA);
        }
        //int resp2 = Repostas quantas vagas estão dispoíveis no andar2
        if(andar2 != resp2 ){
            andar2 = resp2;
            printf("Vagas ocupadas no andar 2: %d\n", andar2);
            dispB = 8 - andar2;
            printf("Vagas disponíveis no andar 2: %d\n", dispB);
        }
        //int resp3 = Repostas quantas vagas estão dispoíveis no ansdar 3
        if(andar3 != resp3 ){
            andar3 = resp3;
            printf("Vagas ocupadas no andar 3: %d\n", andar3);
            dispC = 8 - andar3;
            printf("Vagas disponíveis no andar 3: %d\n", dispC);
        }
        total_carros = andar1 + andar2 + andar3;
        printf("Total de vagas ocupadas: %d\n", total_carros);
        //Valor a ser pago
        
    }
}
void menu(){
    int comando = 0;
    printf("Digite o número do procedimento desejado:\n ");
    printf("1. Fechar Estacionamento\n");
    printf("2. Bloquear segundo andar\n");
    printf("3. Bloquear terceiro andar\n");
    printf("4. Bloquear segundo e terceiro andar\n");
    scanf("%d", &comando);

    switch(comando){
        case 1:
            //Enviar comando para o andar 1 Fechar o estacionamento
            break;
        case 2:
            //Enviar comando para o andar 2 ligar a luz de lotado 
            break;
        case 3:
            //Enviar comando para o andar 3 ligar a luz de lotado 
            break;
        case 4:
            //Enviar comando para o andar 2 e 3 ligarem a luz de lotado 
            break;
    }
}