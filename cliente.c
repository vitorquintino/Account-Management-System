#include <io.h>
#include <stdio.h>
#include <winsock2.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>

#define PORT 5000
#define MAX_REQUISITIONS 1

int main(){
    //Inicializa o buffer de requisição.
    char request[512];

    //Inicializa os sockets do c para o windows.
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    int client, connfd, n; 
    struct sockaddr_in servaddr, cli; 
    
    //Cria o socket do cliente, com os endereços padrões.
    client = socket(AF_INET, SOCK_STREAM, 0); 
    if(client == -1){
        printf("Socket creation failed!");
        return -1;    
    }
    else{
        printf("Socket created. Server started.\n");
    }

    //Coloca o IP e a porta do servidor, baseado em variáveis padrões dos sockets.
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT); 

    int i = 0;
    while(i++ < MAX_REQUISITIONS){
        //Conecta o cliente ao servidor.
        connect(client, (struct sockaddr *)&servaddr, sizeof(servaddr));

        //Faz o cliente inserir a requisição.
        printf("Enter the request string: ");

        //Coloca o texto de requisição no buffer.
        n = 0; 
        while ((request[n++] = getchar()) != '\n') ;

        //Manda um número arbitrário de requisições (todas iguais) ao servidor. Nesse caso, são 12.
        int i = 0;
        while(i<12){
            //Manda sua requisição ao servidor.
            send(client, request, (int)strlen(request), 0);
            printf("Request sent!\n");
            i++;
        }
        //Zera o buffer.
        memset(request, 0, 512);

        //Fecha a conexão com o servidor.
        close(client);
    }
}