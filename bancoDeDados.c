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

char persistChanges(char* requisition);
void* initializeAccounts();
int deposit(int conta, int valor);
int withdrawal(int conta, int valor);
int transfer(int contaOrigem, int contaDestino, int valor);

//Variáveis da fila global, utilizada para armazenar as requisições até que a thread das pthreads as recolha para uma fila local.
char queue[1][512];

//Contas em memória do programa, simulando um banco de dados.
int contas[10];

//Variáveis usadas apenas no caso da opção escolhida pelo cliente ser uma listagem.
int list = 0;
char listValue[100];

int main(){
    
    memset(listValue, 'F', 100);

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

    //Loop infinito para aceitar requisições.
    while(1){
        printf("Started accepting requests.\n");
        //Zera o buffer para que em cada requisição se tenha um buffer novo.
        memset(request, 'F', 100);
        memset(listValue, 'F', 100);

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

            char resultCode = persistChanges(request);

            //Sai do loop quando o cliente não envia mais nenhum byte de requisição.
            if(x<1) break;

            char newResponse[1];
            newResponse[0] = resultCode;

            //Caso seja uma listagem, a resposta ao servidor muda.
            if(list > 0){
                send(client, listValue, x, 0);
                list = 0;
                memset(listValue, 'F', 100);
            }
            else{
                send(client, newResponse, x, 0);
            }

            memset(request, 'F', 100);
        }

        //Fecha a conexão com o cliente.
        closesocket(client);

        printf("Request ended.\n");
    }

	return 0;
}


//Método que faz toda a lógica de persistência dos dados. Controla qual método será chamado baseado na requisição que chegou no servidor.
char persistChanges(char* requisition){
    if(requisition[1] != 'F'){
        //printf("%c %c %c\n", requisition[0], requisition[2], requisition[4]);

        int contaOrigem = -1, contaDestino = -1, valor = -1, requisitionIterator = -1, charIterator = -1, depositResponse = -1;
        char functionResponse = 'X';
        char contaOrigemRequest[100];
        char contaDestinoRequest[100];
        char valorRequest[100];

        char function = requisition[0];
        switch(function){
            case 'D': ;
                requisitionIterator = 2; 
                charIterator = 0;

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

                depositResponse = deposit(contaDestino, valor);
                functionResponse;

                if(depositResponse == 1){
                    functionResponse = 'C';
                }
                else{
                    functionResponse = 'E';
                }
                return functionResponse;
                break;
            case 'S': ;
                requisitionIterator = 2;
                charIterator = 0;

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

                depositResponse = withdrawal(contaDestino, valor);
                functionResponse;

                if(depositResponse == 1){
                    functionResponse = 'C';
                }
                else{
                    functionResponse = 'E';
                }
                return functionResponse;
                break;
            case 'T': ;
                requisitionIterator = 2; 
                charIterator = 0;

                while(requisition[requisitionIterator] != '-'){
                    contaOrigemRequest[charIterator++] = requisition[requisitionIterator++];
                }
                contaOrigem = atoi(contaOrigemRequest);

                charIterator = 0;
                requisitionIterator++;

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

                depositResponse = transfer(contaOrigem, contaDestino, valor);
                functionResponse;

                if(depositResponse == 1){
                    functionResponse = 'C';
                }
                else{
                    functionResponse = 'E';
                }
                return functionResponse;
                break;
            case 'L':
                requisitionIterator = 2; 
                charIterator = 0;

                while(requisition[requisitionIterator] != 'F'){
                    contaOrigemRequest[charIterator++] = requisition[requisitionIterator++];
                }
                contaOrigem = atoi(contaOrigemRequest);

                sprintf(listValue, "%d", contas[contaOrigem - 1]);
                list = 1;

                return 'C';
                break;
            default:

                break;
        }
    }
}

//Método que inicializa um número X (aqui são 10) de contas com o saldo 100.
void* initializeAccounts(){
    int i = 0;
    for(i = 0; i < 10; i++){
        contas[i] = 100;
    }
}

//Método que faz o depósito de uma conta
int deposit(int conta, int valor){
    if(conta < 0 || valor < 0) return -1;

    int tamanho = (int) (sizeof(contas)/sizeof(contas[0]));

    printf("\n%d %d %d\n", tamanho, conta, valor);
    if((int) (sizeof(contas)/sizeof(contas[0])) < conta){
        return -1;
    }

    contas[conta - 1] = contas[conta - 1] + valor;

    return 1;
}

//Método que faz o saque de dinheiro de uma conta
int withdrawal(int conta, int valor){
    if(conta < 0 || valor < 0) return -1;

    int tamanho = (int) (sizeof(contas)/sizeof(contas[0]));

    if((int) (sizeof(contas)/sizeof(contas[0])) < conta){
        return -1;
    }

    if(contas[conta - 1] - valor < 0){
        return -1;
    }

    contas[conta - 1] = contas[conta - 1] - valor;

    return 1;
}

//Método que faz a transferência de dinheiro entre contas
int transfer(int contaOrigem, int contaDestino, int valor){
    if(contaOrigem < 0 || contaDestino < 0 || valor < 0) return -1;

    int tamanho = (int) (sizeof(contas)/sizeof(contas[0]));
    if((int) (sizeof(contas)/sizeof(contas[0])) < contaOrigem || (int) (sizeof(contas)/sizeof(contas[0])) < contaDestino){
        return -1;
    }

    if(contas[contaOrigem - 1] - valor < 0){
        return -1;
    }

    contas[contaOrigem - 1] = contas[contaOrigem - 1] - valor;
    contas[contaDestino - 1] = contas[contaDestino - 1] + valor;

    return 1;
}  