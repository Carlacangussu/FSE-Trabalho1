#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <bcm2835.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 10131

int vagasOcup = 0;

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

int main(){
    if (!bcm2835_init()){
	    return 1;
    }
    setPins();

    pthread_t t_openEntrada, t_openSaida,, t_vagasLidas;
    
    pthread_create(&t_openEntrada,NULL,abreCancelaEntrada,NULL);
    pthread_create(&t_vagasLidas, NULL, lerVagas, NULL);
    pthread_create(&t_openSaida, NULL,abreCancelaSaida,NULL);


    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        printf("Erro ao criar o socket\n");
        return -1;
    }
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_address.sin_port = htons(SERVER_PORT);

    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        printf("Erro ao conectar ao servidor\n");
        close(client_socket);
        return -1;
    }

    char buffer[1024];
    int buffer_size = recv(client_socket, buffer, 1024, 0);
    if (buffer_size == -1) {
        printf("Erro ao receber a mensagem do servidor\n");
        close(client_socket);
        return -1;
    }
    buffer[buffer_size] = '\0';
    //receber o comando e chamar a função responsável pela ação
    int comando = atoi(buffer);
    int vagas1;
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
            vagas1 = imprimeVagas(); 
            //envia a msg de volta
             send(client_socket, vagas1, strlen(vagas1), 0);
        break;
    }

    close(client_socket);

    bcm2835_close();
    return 0;   
}