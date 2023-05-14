#include <stdio.h> 
#include <string.h> 
#include <stdlib.h>  
#include <sys/socket.h> 
#include <netinet/in.h> 
//#include <pthread.h>
#include <arpa/inet.h>
#include <unistd.h>

//#include <semaphore.h>
//#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros 

#define SERVER_IP "164.41.98.15"
#define SERVER_PORT 10131

#define TRUE   1 
#define FALSE  0
#define TAXA 0.15 

void interface(){
    int andar1 = 0, andar2 =0, total_carros=0; // Vagas ocupadas em cada andar e no estacionamento 
    int dispA, dispB;// Número de Vagas disponíveis em cada andar;
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
        total_carros = andar1 + andar2;
        printf("Total de vagas ocupadas: %d\n", total_carros);
        //Valor a ser pago
        
    }
}
int menu(){
    int comando
    printf("Digite o número do procedimento desejado:\n ");
    printf("0. Sair\n");
    printf("1. Fechar Estacionamento\n");
    printf("2. Bloquear segundo andar\n");
    printf("3. Desbloquear segundo andar\n");
    printf("4. Abrir estacionamento\n");
    printf("5. Ver status da vagas\n");
    scanf("%d", &comando);
    return comando;   
}

int main(){
    int valread;
    char buffer[1024] = {0};

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        printf("Erro ao criar o socket\n");
        return -1;
    }
    
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_address.sin_port = htons(SERVER_PORT);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        printf("Erro ao vincular o socket com o endereço do servidor\n");
        close(server_socket);
        return -1;
    }
    if (listen(server_socket, 2) == -1) {
        printf("Erro ao escutar por conexões dos clientes\n");
        close(server_socket);
        return -1;
    }
    struct sockaddr_in client_address1, client_address2;
    int client_socket1 = accept(server_socket, (struct sockaddr *)&client_address1, (socklen_t *)&client_address1);
   
    if (client_socket1 == -1) {
        printf("Erro ao aceitar a conexão do primeiro andar\n");
        close(server_socket);
        return -1;
    }
    int client_socket2 = accept(server_socket, (struct sockaddr *)&client_address2, (socklen_t *)&client_address2);
    if (client_socket2 == -1) {
        printf("Erro ao aceitar a conexão do segundo andar\n");
        close(server_socket);
        return -1;
    }
    int comando = 5;

    while(comando > 0){

        comando = menu ();
        switch(comando){
            case 1:
                //Enviar comando para o andar 1 Fechar o estacionamento
                char *message1 = "1";
                send(client_socket1, message1, strlen(message1), 0);
            break;
            case 2:
                //Enviar comando para o andar 2 ligar a luz de lotado 
                char *message2 = "1";
                send(client_socket2, message2, strlen(message2), 0);
            break;
            case 3:
                //Enviar comando para liberar o andar 2
                char *message2 = "2";
                send(client_socket2, message2, strlen(message2), 0); 
            break;
            case 4:
                //Enviar comando para abrir estacionamento
                char *message1 = "2";
                send(client_socket1, message1, strlen(message1), 0); 
            break;
            case 5:
                //Imprimir status das vagas
                char *message1 = "3";
                send(client_socket1, message1, strlen(message1), 0);
                //valread = recv(client_socket1, buffer, 1024, 0);
                //printf("Vagas do andar 1: %s\n", buffer);
                char *message2 = "3";
                send(client_socket2, message2, strlen(message2), 0);
                //valread = recv(new_socket, buffer, 1024, 0);
                //printf("Vagas do andar 2: %s\n", buffer);                                                                                                                                             
            break;
        }    
    }

    close(client_socket1);
    close(client_socket2);
    close(server_socket);

    return 0;
}