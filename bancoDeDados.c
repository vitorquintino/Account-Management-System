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

char* persistChanges(char* requisition);
void* initializeAccounts();
int deposit(int conta, int valor);

//Variáveis da fila global, utilizada para armazenar as requisições até que a thread das pthreads as recolha para uma fila local.
char queue[1][512];

int contas[10];

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
        .sin_port        = htons(6000)
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

    initializeAccounts();

    //Cria a thread que vai controlar as pthreads.
    //pthread_t thread;
    //pthread_create(&thread, NULL, respondRequisitions, NULL);

    //Loop infinito para aceitar requisições.
    while(1){
        printf("Started accepting requests.\n");
        //Zera o buffer para que em cada requisição se tenha um buffer novo.
        memset(request, 'F', 100);

        //Aceita uma conexão de um cliente.
        client = accept(server, (struct sockaddr *) &caddr, &csize);
        if(client < 0){
            printf("Error while accepting request.");
            return -1;
        }
        else{
            printf("Request accepted!\n");
        }

        while(1){
            //Recebe e printa na tela os bytes da requisição do cliente.
            x = recv(client, request, sizeof request, 0);

            persistChanges(request);

            //Sai do loop quando o cliente não envia mais nenhum byte de requisição.
            if(x<1) break;

            //Coloca a requisição na fila global.
            //strncpy(queue[lastOnTheQueue++], request, strlen(request) - 1);

            //Coloca o valor do último char como o caracter especial de fim de strings, para que requisições anteriores não interfiram nas futuras.
            //int last = lastOnTheQueue - 1;
            //queue[last][strlen(request) - 1] = '\0';

            memset(request, 'F', 100);
        }

        //Fecha a conexão com o cliente.
        closesocket(client);

        printf("Request ended.\n");
    }

	return 0;
}



char* persistChanges(char* requisition){
    if(requisition[1] != 'F'){
        //printf("%s\nENTREI\n", requisition);
        printf("%c %c %c\n", requisition[0], requisition[2], requisition[4]);
        
        char repsonse[3];

        int contaOrigem = -1, contaDestino = -1, valor = -1;
        char contaOrigemRequest[100];
        char contaDestinoRequest[100];
        char valorRequest[100];

        char function = requisition[0];
        switch(function){
            case 'D': ;
                

                int requisitionIterator = 2, charIterator = 0;
                while(requisition[requisitionIterator] != '-'){
                    contaDestinoRequest[charIterator++] = requisition[requisitionIterator++];
                }
                contaDestino = atoi(contaDestinoRequest);

                charIterator = 0;
                requisitionIterator++;

                while(requisition[requisitionIterator] != 'F'){
                    valorRequest[charIterator++] = requisition[requisitionIterator++];
                }

                valor = atoi(valorRequest);

                int resposta = deposit(contaDestino, valor);

                printf("Resposta: %d\n", resposta);
                return "";
                break;
            case 'S':

                break;
            case 'T':

                break;
            case 'L':

                break;
            default:

                break;
        }
    }
}

void* initializeAccounts(){
    int i = 0;
    for(i = 0; i < 10; i++){
        contas[i] = 100;
    }
}

int deposit(int conta, int valor){

    int tamanho = (int) (sizeof(contas)/sizeof(contas[0]));

    printf("\n%d %d %d\n", tamanho, conta, valor);
    if((int) (sizeof(contas)/sizeof(contas[0])) < conta){
        return -1;
    }
    return 1;
}