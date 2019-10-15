#include <io.h>
#include <stdio.h>
#include <winsock2.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

char sendChangesToDatabase(char* requisition);

//Variáveis da fila global, utilizada para armazenar as requisições até que a thread das pthreads as recolha para uma fila local.
char queue[1][512];

int main(){
    
    //Inicializa o buffer de requisição.
    char request[100];

    //Inicializa os sockets do c para o windows.
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    struct sockaddr_in caddr;

    //Coloca o IP e a porta do servidor, baseado em variáveis padrões dos sockets.
    struct sockaddr_in saddr = {
        .sin_family      = AF_INET,
        .sin_addr.s_addr = htonl(INADDR_ANY),
        .sin_port        = htons(5000)
    };

    //Cria o socket de servidor, com os endereços padrões.
    int server = socket(AF_INET, SOCK_STREAM, 0);
    if(server == -1){
        printf("Socket creation failed!");
        return -1;    
    }
    else{
        printf("Socket created. Server started.\n");
    } 
    int client, x;
    int csize  = sizeof caddr;

    //Aloca o socket recém criado ao IP e porta designados acima.
    bind(server, (struct sockaddr *) &saddr, sizeof saddr);

    //Deixa o servidor "ouvindo" as requisições.
    listen(server, 5);

    //Loop infinito para aceitar requisições.
    while(1){
        printf("Started accepting requests.\n");
        //Zera o buffer para que em cada requisição se tenha um buffer novo.
        memset(request, 'F', 100);

        printf("\n6\n");

        //Aceita uma conexão de um cliente.
        client = accept(server, (struct sockaddr *) &caddr, &csize);
        printf("\n7\n");
        if(client < 0){
            printf("Error while accepting request.");
            return -1;
        }
        else{
            printf("Request accepted!\n");
        }

        while(1){
            //Recebe e printa na tela os bytes da requisição do cliente.
            printf("\n100\n");
            x = recv(client, request, sizeof request, 0);
            
            if(x<1) break;
            //char response = sendChangesToDatabase(request);
            printf("\n2\n");
            char newResponse[1];
            //newResponse[0] = response;
            newResponse[0] = 'A';
            //printf("\nServidor: %c\n", response);

            send(client, "oi", x, 0);
            printf("\n3\n");
        }
        printf("\n5\n");
        //Fecha a conexão com o cliente.
        closesocket(client);
        printf("\n4\n");

        printf("Request ended.\n");
    }

	return 0;
}



char sendChangesToDatabase(char* requisition){
    printf("%s\nENTREI\n", requisition);

    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    int client, connfd; 
    struct sockaddr_in servaddr, cli;

    client = socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(6000); 

    connect(client, (struct sockaddr *)&servaddr, sizeof(servaddr));

    send(client, requisition, (int)strlen(requisition), 0);

    char response[100];
    int x = recv(client, response, sizeof response, 0);

    closesocket(client);

    return response[0];
}