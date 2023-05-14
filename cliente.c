#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5000

int main() {
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
    printf("Mensagem recebida do servidor: %s\n", buffer);

    close(client_socket);

    return 0;
}
