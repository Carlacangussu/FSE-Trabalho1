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
    
    if(vagasOcup == 7){
        bcm2835_gpio_write(lotado, HIGH); //liga a luz de lotado
        printf("Andar 1 lotado\n");
    }
        else{
        int disp = 8 - vagasOcup;
        bcm2835_gpio_write(lotado, LOW); // Desliga a luz de lotado
        printf("Quantidade de vagas livres no andar 1: %d\n", disp);
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
void *fechaCancelaEntrada(){
    while(1){
        if(bcm2835_gpio_lev(EntFec)){
        bcm2835_gpio_write(CancEntr, LOW);
        bcm2835_delay(800); // espera 500 ms         
        printf("Cancela de entrada Fechada\n");
        }
    }
}

int *lerVagas(){
    while(1){

        printf("Iniciando leitura das vagas...\n");
        bcm2835_gpio_write(ENDERECO_01, LOW);
        bcm2835_gpio_write(ENDERECO_02, LOW);
        bcm2835_gpio_write(ENDERECO_03, LOW);
        VAGA[0] = bcm2835_gpio_lev(SENSOR_DE_VAGA);

        bcm2835_gpio_write(ENDERECO_01, HIGH);
        bcm2835_gpio_write(ENDERECO_02, LOW);
        bcm2835_gpio_write(ENDERECO_03, LOW);
        VAGA[1] = bcm2835_gpio_lev(SENSOR_DE_VAGA);

        bcm2835_gpio_write(ENDERECO_01, LOW);
        bcm2835_gpio_write(ENDERECO_02, HIGH);
        bcm2835_gpio_write(ENDERECO_03, LOW);
        VAGA[2] = bcm2835_gpio_lev(SENSOR_DE_VAGA);

        bcm2835_gpio_write(ENDERECO_01, HIGH);
        bcm2835_gpio_write(ENDERECO_02, HIGH);
        bcm2835_gpio_write(ENDERECO_03, LOW);
        VAGA[3] = bcm2835_gpio_lev(SENSOR_DE_VAGA);

        bcm2835_gpio_write(ENDERECO_01, LOW);
        bcm2835_gpio_write(ENDERECO_02, LOW);
        bcm2835_gpio_write(ENDERECO_03, HIGH);
        VAGA[4] = bcm2835_gpio_lev(SENSOR_DE_VAGA);

        bcm2835_gpio_write(ENDERECO_01, HIGH);
        bcm2835_gpio_write(ENDERECO_02, LOW);
        bcm2835_gpio_write(ENDERECO_03, HIGH);
        VAGA[5] = bcm2835_gpio_lev(SENSOR_DE_VAGA);

        bcm2835_gpio_write(ENDERECO_01, LOW);
        bcm2835_gpio_write(ENDERECO_02, HIGH);
        bcm2835_gpio_write(ENDERECO_03, HIGH);
        VAGA[6] = bcm2835_gpio_lev(SENSOR_DE_VAGA);

        bcm2835_gpio_write(ENDERECO_01, HIGH);
        bcm2835_gpio_write(ENDERECO_02, HIGH);
        bcm2835_gpio_write(ENDERECO_03, HIGH);
        VAGA[7] = bcm2835_gpio_lev(SENSOR_DE_VAGA);

        vagasOcup = 0;
        printf("Andar 1: ");
        for(int i = 0; i<8; i++){
            if (VAGA[i] == 1){
                vagasOcup++;
            }
            if(i==7){
                printf("%d\n", VAGA[i]);
            }
            else{
                printf("%d, ", VAGA[i]);
            }
        }
        andarLotado(vagasOcup);
        return vagasOcup;
    }
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
void fechaCancelaSaida(){
    while(1){
       if(bcm2835_gpio_lev(SaiFec)){
        bcm2835_gpio_write(CancSai, LOW);
        bcm2835_delay(800); // espera 500 ms
        printf("Cancela de sai Fechada\n");
       }
    }
}
void acao(int comando){

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
            // registra data e hora da entrada;
            abreCancelaEntrada(); 
            bcm2835_delay(800); // espera 500 ms
            fechaCancelaEntrada();
            bcm2835_delay(800);                    
                
            int vagasO = lerVagas(); // imprimir vagas

            //calula valor a ser pago
            abreCancelaSaida();
            bcm2835_delay(800);
            fechaCancelaSaida();
            //return vagasO;
        break;
    }
}
int main(){
    if (!bcm2835_init()){
	    return 1;
    }
    setPins();

    pthread_t t_openEntrada, t_closeEntrada, t_openSaida, t_closeSaida, t_vagasLidas;
    
    pthread_create(&t_openEntrada,NULL,abreCancelaEntrada,NULL);
   // pthread_create(&t_closeEntrada,NULL,fechaCancelaEntrada,NULL);
  //  pthread_create(&t_vagasLidas, NULL, lerVagas, NULL);
    pthread_create(&t_openSaida, NULL,abreCancelaSaida,NULL);
  //  pthread_create(&t_closeSaida,NULL, fechaCancelaSaida,NULL);

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
    int valor = atoi(buffer);
    acao(valor);
    //printf("Mensagem recebida do servidor: %s\n", buffer); 

    pthread_join(t_openEntrada,NULL);
   // pthread_join(t_closeEntrada,NULL);
   // pthread_join(t_vagasLidas,NULL);
    pthread_join(t_openSaida,NULL);
   // pthread_join(t_closeSaida,NULL);

    close(client_socket);

    bcm2835_close();
    return 0;   
}