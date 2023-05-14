#include <stdio.h>
#include <bcm2835.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "164.41.98.15"
#define SERVER_PORT 10131

int vagasOcup = 0;

int ENDERECO_01 = 13, ENDERECO_02 = 06, ENDERECO_03 = 05;
int SENSOR_DE_VAGA = 20,
    VAGA[8],  
    lotado = 08;  //SINAL_DE_LOTADO_FECHADO
int SENSOR_DE_PASSAGEM_1 = 16, 
    SENSOR_DE_PASSAGEM_2 = 21;

void setPins(){
    bcm2835_gpio_fsel(SENSOR_DE_VAGA, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(SENSOR_DE_VAGA, BCM2835_GPIO_PUD_DOWN);
    bcm2835_gpio_fsel(SENSOR_DE_PASSAGEM_1, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(SENSOR_DE_PASSAGEM_1, BCM2835_GPIO_PUD_DOWN);
    bcm2835_gpio_fsel(SENSOR_DE_PASSAGEM_2, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(SENSOR_DE_PASSAGEM_2, BCM2835_GPIO_PUD_DOWN);

    bcm2835_gpio_fsel(ENDERECO_01, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(ENDERECO_02, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(ENDERECO_03, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(lotado, BCM2835_GPIO_FSEL_OUTP);
}

void andarLotado(int vagasOcup){
    if(vagasOcup == 7){
        bcm2835_gpio_write(lotado, HIGH);
        printf("Andar 2 lotado\n");
    }
    else{
        int disp = 8 -vagasOcup;
        bcm2835_gpio_write(lotado, LOW);
        printf("Quantidade de vagas livres no andar 2: %d\n", disp);
    }
}

void lerVagas(){
    printf("Iniciando leitura das vagas...\n");
    bcm2835_gpio_write(ENDERECO_01, LOW);
    //printf("Envia ao endereço 1 (porta 13) o valor 0\n");
    bcm2835_gpio_write(ENDERECO_02, LOW);
    //printf("Envia ao endereço 2 (porta 06) o valor 0\n");
    bcm2835_gpio_write(ENDERECO_03, LOW);
    //printf("Envia ao endereço 3 (porta 05) o valor 0\n");
    VAGA[0] = bcm2835_gpio_lev(SENSOR_DE_VAGA);
    //printf("Vaga 1 : %d\n", VAGA[0]);

    bcm2835_gpio_write(ENDERECO_01, HIGH);
    //printf("Envia ao endereço 1 (porta 13) o valor 1"\n);
    bcm2835_gpio_write(ENDERECO_02, LOW);
    //printf("Envia ao endereço 2 (porta 06) o valor 0\n");
    bcm2835_gpio_write(ENDERECO_03, LOW);
    //printf("Envia ao endereço 3 (porta 05) o valor 0\n");
    VAGA[1] = bcm2835_gpio_lev(SENSOR_DE_VAGA);
    //printf("Vaga 2 : %d\n", VAGA[1]);

    bcm2835_gpio_write(ENDERECO_01, LOW);
    //printf("Envia ao endereço 1 (porta 13) o valor 0\n");
    bcm2835_gpio_write(ENDERECO_02, HIGH);
    //printf("Envia ao endereço 2 (porta 06) o valor 1\n");
    bcm2835_gpio_write(ENDERECO_03, LOW);
    //printf("Envia ao endereço 3 (porta 05) o valor 0\n");
    VAGA[2] = bcm2835_gpio_lev(SENSOR_DE_VAGA);
    //printf("Vaga 3 : %d\n", VAGA[2]);

    bcm2835_gpio_write(ENDERECO_01, HIGH);
    //printf("Envia ao endereço 1 (porta 13) o valor 1\n");
    bcm2835_gpio_write(ENDERECO_02, HIGH);
    //printf("Envia ao endereço 2 (porta 06) o valor 1\n");
    bcm2835_gpio_write(ENDERECO_03, LOW);
    //printf("Envia ao endereço 3 (porta 05) o valor 0\n");
    VAGA[3] = bcm2835_gpio_lev(SENSOR_DE_VAGA);
    //printf("Vaga 4 : %d\n", VAGA[3]);

    bcm2835_gpio_write(ENDERECO_01, LOW);
    //printf("Envia ao endereço 1 (porta 13) o valor 0\n");
    bcm2835_gpio_write(ENDERECO_02, LOW);
    //printf("Envia ao endereço 2 (porta 06) o valor 0\n");
    bcm2835_gpio_write(ENDERECO_03, HIGH);
    //printf("Envia ao endereço 3 (porta 05) o valor 1\n");
    VAGA[4] = bcm2835_gpio_lev(SENSOR_DE_VAGA);
    //printf("Vaga 5 : %d\n", VAGA[4]);

    bcm2835_gpio_write(ENDERECO_01, HIGH);
    //printf("Envia ao endereço 1 (porta 13) o valor 1\n");
    bcm2835_gpio_write(ENDERECO_02, LOW);
    //printf("Envia ao endereço 2 (porta 06) o valor 0\n");
    bcm2835_gpio_write(ENDERECO_03, HIGH);
    //printf("Envia ao endereço 3 (porta 05) o valor 1\n");
    VAGA[5] = bcm2835_gpio_lev(SENSOR_DE_VAGA);
    //printf("Vaga 6 : %d\n", VAGA[5]);

    bcm2835_gpio_write(ENDERECO_01, LOW);
    //printf("Envia ao endereço 1 (porta 13) o valor 0\n");
    bcm2835_gpio_write(ENDERECO_02, HIGH);
    //printf("Envia ao endereço 2 (porta 06) o valor 1\n");
    bcm2835_gpio_write(ENDERECO_03, HIGH);
    //printf("Envia ao endereço 3 (porta 05) o valor 1\n");
    VAGA[6] = bcm2835_gpio_lev(SENSOR_DE_VAGA);
    //printf("Vaga 7 : %d\n", VAGA[6]);

    bcm2835_gpio_write(ENDERECO_01, HIGH);
    //printf("Envia ao endereço 1 (porta 13) o valor 1\n");
    bcm2835_gpio_write(ENDERECO_02, HIGH);
    //printf("Envia ao endereço 2 (porta 06) o valor 1\n");
    bcm2835_gpio_write(ENDERECO_03, HIGH);
    //printf("Envia ao endereço 3 (porta 05) o valor 1\n");
    VAGA[7] = bcm2835_gpio_lev(SENSOR_DE_VAGA);
    //printf("Vaga 8 : %d\n", VAGA[7]);

    vagasOcup = 0;
    for(int i = 0; i<8; i++){
        if (VAGA[i] == HIGH){
            vagasOcup++;
        }
        if(i==7){
            printf("%d\n", VAGA[i]);
        }
        else{
            printf("%d, ", VAGA[i]);
        }
    }
    printf("Quantidade de vagas ocupadas no primeiro andar: %d\n", vagasOcup);
    printf("------------------------\n");
    andarLotado(vagasOcup);

}

void menu(int comando){

    switch(comando){
        case 1:
            bcm2835_gpio_write(lotado, HIGH);
            printf("Andar 2 lotado\n");
        break;
        case 2:
            bcm2835_gpio_write(lotado, LOW);
            printf("Andar 2 liberado\n");

        break;
        case 3:
            lerVagas();
        break;
    }
}

int main(){
    if (!bcm2835_init())
	    return 1;
    setPins();
    pthread_t t_vagas;
    pthread_create(&t_vagasLidas, NULL, lerVagas, NULL);


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
    int valor = atoi(buffer);
    acao(valor);
    //printf("Mensagem recebida do servidor: %s\n", buffer);

  // pthread_join(t_vagasLidas,NULL);

    close(client_socket2);

    bcm2835_close();
    return 0;
}