#include <stdio.h>
#include <bcm2835.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 10131

int vagasOcup2 = 0;

int ENDERECO_01_B = 13, ENDERECO_02_B = 6, ENDERECO_03_B = 5;
int SENSOR_DE_VAGA_2 = 20,
    VAGA2[8],  
    lotado2 = 8;  //SINAL_DE_LOTADO_FECHADO
int SENSOR_DE_PASSAGEM_1 = 16, 
    SENSOR_DE_PASSAGEM_2 = 21;

void setPins(){
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


int main(){
    if (!bcm2835_init())
	    return 1;
    setPins();
    pthread_t t_vagas;
    pthread_create(&t_vagasLidas, NULL, lerVagas2, NULL);


    int client_socket2 = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket2 == -1) {
        printf("Erro ao criar o socket\n");
        return -1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_address.sin_port = htons(SERVER_PORT);

    if (connect(client_socket2, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        printf("Erro ao conectar ao servidor\n");
        close(client_socket2);
        return -1;
    }
    
    char buffer[1024];
    int buffer_size = recv(client_socket2, buffer, 1024, 0);
    if (buffer_size == -1) {
        printf("Erro ao receber a mensagem do servidor\n");
        close(client_socket2);
        return -1;
    }
    buffer[buffer_size] = '\0';
    int comando = atoi(buffer);
    int vagas2;
    switch(comando){
        case 1:
            bcm2835_gpio_write(lotado2, HIGH);
            printf("Andar 2 lotado\n");
        break;
        case 2:
            bcm2835_gpio_write(lotado2, LOW);
            printf("Andar 2 liberado\n");

        break;
        case 3:
            vagas2 = imprimeVagas2();
            send(client_socket2, vagas1, strlen(vagas1), 0);
        break;
    }
    
    close(client_socket2);

    bcm2835_close();
    return 0;
}