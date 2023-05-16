#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <bcm2835.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>

int vagasOcup = 0;
// ConfiguraPins
int CancEntr = 10, //MOTOR_CANCELA_ENTRADA
    CancSai = 17; //MOTOR_CANCELA_SAIDA
// Sensores
int  EntAbr = 23,  //SENSOR_ABERTURA_CANCELA_ENTRADA
    EntFec = 24, //SENSOR_FECHAMENTO_CANCELA_ENTRADA
    SaiAbr = 25, //SENSOR_ABERTURA_CANCELA_SAIDA
    SaiFec = 12; //SENSOR_FECHAMENTO_CANCELA_SAIDA
// Endereços
int ENDERECO_01 = 22, ENDERECO_02 = 26, ENDERECO_03 = 19;

//Leds
int SENSOR_DE_VAGA = 18,
    VAGA[8],  
    lotado = 27;  //SINAL_DE_LOTADO_FECHADO

int vagasOcup2 = 0;

int ENDERECO_01_B = 13, ENDERECO_02_B = 6, ENDERECO_03_B = 5;
int SENSOR_DE_VAGA_2 = 20,
    VAGA2[8],  
    lotado2 = 8;  //SINAL_DE_LOTADO_FECHADO
int SENSOR_DE_PASSAGEM_1 = 16, 
    SENSOR_DE_PASSAGEM_2 = 21;

void setPins(){
    // Define sensores e configura entradas com Pull-Down
    bcm2835_gpio_fsel(EntAbr, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(EntAbr, BCM2835_GPIO_PUD_DOWN);
    bcm2835_gpio_fsel(EntFec, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(EntFec, BCM2835_GPIO_PUD_DOWN);
    bcm2835_gpio_fsel(SaiAbr, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(SaiAbr, BCM2835_GPIO_PUD_DOWN);
    bcm2835_gpio_fsel(SaiFec, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(SaiFec, BCM2835_GPIO_PUD_DOWN);
    bcm2835_gpio_fsel(SENSOR_DE_VAGA, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(SENSOR_DE_VAGA, BCM2835_GPIO_PUD_DOWN);

    // Pinos dos LEDs de saída ????
    bcm2835_gpio_fsel(ENDERECO_01, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(ENDERECO_02, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(ENDERECO_03, BCM2835_GPIO_FSEL_OUTP); 
    //LED 
    bcm2835_gpio_fsel(lotado, BCM2835_GPIO_FSEL_OUTP);

    // Motores das cancelas
    bcm2835_gpio_fsel(CancEntr, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(CancSai, BCM2835_GPIO_FSEL_OUTP);

    bcm2835_gpio_fsel(SENSOR_DE_VAGA_2, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(SENSOR_DE_VAGA_2, BCM2835_GPIO_PUD_DOWN);
    bcm2835_gpio_fsel(SENSOR_DE_PASSAGEM_1, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(SENSOR_DE_PASSAGEM_1, BCM2835_GPIO_PUD_DOWN);
    bcm2835_gpio_fsel(SENSOR_DE_PASSAGEM_2, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(SENSOR_DE_PASSAGEM_2, BCM2835_GPIO_PUD_DOWN);

    bcm2835_gpio_fsel(ENDERECO_01_B, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(ENDERECO_02_B, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(ENDERECO_03_B, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(lotado2, BCM2835_GPIO_FSEL_OUTP);
}

void *andarLotado (int vagasOcup){
    int disp = 8 - vagasOcup;
    //printf("Quantidade de vagas livres no andar 1: %d\n", disp);
    if(disp == 0){
        bcm2835_gpio_write(lotado, HIGH); //liga a luz de lotado
        printf("Andar 1 lotado\n");
    }
    else{
         bcm2835_gpio_write(lotado, LOW);
    }
}
void *andarLotado2(int vagasOcup2){
   int disp = 8 - vagasOcup2;
    //printf("Quantidade de vagas livres no andar 2: %d\n", disp);
    if(disp == 0){
        bcm2835_gpio_write(lotado2, HIGH);
        printf("Andar 2 lotado\n");
    }
    else{
        bcm2835_gpio_write(lotado2, LOW);
    }
}

void *abreCancelaEntrada (){
    while(1){

        if(bcm2835_gpio_lev(EntAbr)){
            bcm2835_gpio_write(CancEntr, HIGH);
            bcm2835_delay(800); // espera 500 ms
            printf("Cancela de entrada aberta\n");
        }
        if(bcm2835_gpio_lev(EntFec)){
            bcm2835_gpio_write(CancEntr, LOW);
            bcm2835_delay(800); // espera 500 ms         
            printf("Cancela de entrada Fechada\n");
        }
    }
}

void *lerVagas(){
    while(1){
    for (int address = 0; address <= 7; address++) {
            // Configura os bits de endereço
            bcm2835_gpio_write(ENDERECO_01, (address & 0x01)); // Bit 0 do endereço
            bcm2835_gpio_write(ENDERECO_02, (address & 0x02) >> 1); // Bit 1 do endereço
            bcm2835_gpio_write(ENDERECO_03, (address & 0x04) >> 2); // Bit 2 do endereço

            // Lê o valor do sensor
            VAGA[address] = bcm2835_gpio_lev(SENSOR_DE_VAGA);

            // Aguarda um tempo antes de passar para o próximo endereço
            bcm2835_delay(1000); // 1 segundo
        }
        vagasOcup = 0;
        for(int i = 0; i<8; i++){
            if (VAGA[i] == 1){
                vagasOcup++;
            }
        }
        andarLotado(vagasOcup);
    }
}
void *lerVagas2(){
    while(1){
        for (int address = 0; address <= 7; address++) {
            // Configura os bits de endereço
            bcm2835_gpio_write(ENDERECO_01_B, (address & 0x01)); // Bit 0 do endereço
            bcm2835_gpio_write(ENDERECO_02_B, (address & 0x02) >> 1); // Bit 1 do endereço
            bcm2835_gpio_write(ENDERECO_03_B, (address & 0x04) >> 2); // Bit 2 do endereço

            // Lê o valor do sensor
            VAGA2[address] = bcm2835_gpio_lev(SENSOR_DE_VAGA_2);

            // Aguarda um tempo antes de passar para o próximo endereço
            bcm2835_delay(1000); // 1 segundo
        }

        vagasOcup2 = 0;
        for(int i = 0; i<8; i++){
            if (VAGA2[i] == 1){
            vagasOcup2++;
            }
        }
        andarLotado2(vagasOcup2);
    }
}
int *imprimeVagas(){
    printf("Andar 1: ");
        for(int i = 0; i<8; i++){
            if(i==7){
                printf("%d\n", VAGA[i]);
            }
            else{
                printf("%d, ", VAGA[i]);
            }
        }
        printf("Quantidade de vagas ocupadas no primeiro andar: %d\n", vagasOcup);
        int disp = 8 - vagasOcup;
        printf("Quantidade de vagas livres no andar 1: %d\n", disp);
        printf("------------------------\n");
        return vagasOcup;
}

int *imprimeVagas2(){
    printf("Andar 2: ");
        for(int i = 0; i<8; i++){
            if(i==7){
                printf("%d\n", VAGA2[i]);
            }
            else{
                printf("%d, ", VAGA2[i]);
            }
        }
        printf("Quantidade de vagas ocupadas no segundo andar: %d\n", vagasOcup2);
        int disp = 8 - vagasOcup2;
        printf("Quantidade de vagas livres no andar 2: %d\n", disp);
        printf("------------------------\n");
        return vagasOcup2;
}

void *abreCancelaSaida (){
    while(1){
        if(bcm2835_gpio_lev(SaiAbr)){
            bcm2835_gpio_write(CancSai, HIGH);
            bcm2835_delay(800); // espera 500 ms
            printf("Cancela de saida aberta\n");
        }
        if(bcm2835_gpio_lev(SaiFec)){
        bcm2835_gpio_write(CancSai, LOW);
        bcm2835_delay(800); // espera 500 ms
        printf("Cancela de sai Fechada\n");
       }
    }
}
void acao(){
    int comando, vagas1, vagas2;
    printf("Digite o comando desejado\n");
    printf("1. Fechar estacionamento\n");
    printf("2. Abrir estacionamento\n");
    printf("3. Fechar andar 2\n");
    printf("4. Abrir andar 2\n");
    printf("5. Ler status das vagas\n");
    scanf("%d", &comando);

    switch(comando){
        case 1:
             bcm2835_gpio_write(lotado, HIGH); //liga a luz de lotado
             printf("Andar 1 Fechado\n");
        break;
        case 2:
            bcm2835_gpio_write(lotado, LOW); //liga a luz de lotado
            printf("Andar 1 Aberto\n"); 
        break;
        case 3:
            bcm2835_gpio_write(lotado2, HIGH);
            printf("Andar 2 lotado\n");
        break;
        case 4:
            bcm2835_gpio_write(lotado2, LOW);
            printf("Andar 2 aberto\n");
        break;
        case 5:
                              
            vagas1 = imprimeVagas(); 
            vagas2 = imprimeVagas2();
            int vagas_total = vagas1 + vagas2;
            printf("Total de vagas ocupadas: %d\n", vagas_total);

        break;
    }
}
int main(){
    if (!bcm2835_init()){
	    return 1;
    }
    setPins();

    pthread_t t_openEntrada,t_openSaida, t_lerVagas1, t_lerVagas2;     
    pthread_create(&t_openEntrada,NULL,abreCancelaEntrada,NULL);
    pthread_create(&t_lerVagas1, NULL,lerVagas,NULL);
    pthread_create(&t_lerVagas2, NULL,lerVagas2,NULL);
    pthread_create(&t_openSaida,NULL,abreCancelaEntrada,NULL);;

    while(1){
          acao();
    }
  
    bcm2835_close();
    return 0;   
}