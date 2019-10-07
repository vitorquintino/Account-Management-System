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

void* requisitionResponder(void* arg);
void* respondRequisitions(void* arg);
int threadAvailable();
int getAvailableThread();

//Struct dos argumentos passados a função de thread.
struct requisitionArgs {
    int threadNumber;
    char* requisition;
};

//Variáveis da fila global, utilizada para armazenar as requisições até que a thread das pthreads as recolha para uma fila local.
char queue[50][512];
int lastOnTheQueue;

//Marca quais das 10 threads estão sendo utilizadas. 0-> não está sendo utilizada. 1-> está sendo utilizada.
int usedThreads[10];

//Variável do semáforo 0-> recurso disponível. 1-> recurso indisponível.
int queueResourceAvailable;

int main(){

    //Inicializa a variável das threads como 0;
    int init;
    for(init=0; init<10; init++){
        usedThreads[init] = 0;
    }

    //Inicializa a variável que marca a última requisição como 0.
    lastOnTheQueue = 0;
    
    //Inicializa o buffer de requisição.
    char request[512];

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

    //Cria a thread que vai controlar as pthreads.
    pthread_t thread;
    pthread_create(&thread, NULL, respondRequisitions, NULL);

    //Loop infinito para aceitar requisições.
    while(1){
        printf("Started accepting requests.\n");
        //Zera o buffer para que em cada requisição se tenha um buffer novo.
        memset(request, 0, 512);

        //Aceita uma conexão de um cliente.
        client = accept(server, (struct sockaddr *) &caddr, &csize);
        if(client < 0){
            printf("Error while accepting request.");
            return -1;
        }
        else{
            printf("Request accepted!\n");
        }

        //Bloqueia o recurso das filas globais.
        queueResourceAvailable = 1;
        while(1){
            //Recebe e printa na tela os bytes da requisição do cliente.
            x = recv(client, request, sizeof request, 0);

            //Sai do loop quando o cliente não envia mais nenhum byte de requisição.
            if(x<1) break;

            //Coloca a requisição na fila global.
            strncpy(queue[lastOnTheQueue++], request, strlen(request) - 1);

            //Coloca o valor do último char como o caracter especial de fim de strings, para que requisições anteriores não interfiram nas futuras.
            int last = lastOnTheQueue - 1;
            queue[last][strlen(request) - 1] = '\0';
        }
        //Libera o recurso das filas globais.
        queueResourceAvailable = 0;

        //Fecha a conexão com o cliente.
        closesocket(client);

        printf("Request ended.\n");
    }

	return 0;
}



void* requisitionResponder(void* arg){
    //Aloca os argumentos em uma variável local.
    struct requisitionArgs* args = arg;

    //Printa que entrou na thread.
    printf("Entered thread %d %s\n", args->threadNumber, args->requisition);

    //Dorme por 10 segundos ("processamento da requisição").
    sleep(10);

    //Printa que terminou o processamento da thread.
    printf("Thread %d terminated\n", args->threadNumber);

    //Libera o uso da thread.
    usedThreads[args->threadNumber] = 0;

    //Libera o espaço da memória dos argumentos.
    free(args);
}


void* respondRequisitions(void* arg){
    //Guarda as threads utilizadas para responder as requisições.
    pthread_t threads[10];

    //Variáveis de fila local.
    char localQueue[50][512];
    char localLastOnTheQueue = 0;

    while(1){
        //If utilizado para copiar as requisições da fila global para a fila local.
        if(lastOnTheQueue > 0 && queueResourceAvailable == 0){
            while(lastOnTheQueue > 0){
                lastOnTheQueue--;

                //Copia da fila global para a fila local.
                strcpy(localQueue[localLastOnTheQueue], queue[lastOnTheQueue]);
                localLastOnTheQueue++;
            }
        }
        
        //If utilizado para se tiver uma thread disponível, alocar uma requisição para uma thread.
        if(threadAvailable() == 1){

            //If utilizado para só prosseguir se tiver alguma requisição pendente na fila local.
            if(localLastOnTheQueue > 0){
                int threadNumber = getAvailableThread();
                if(threadNumber > -1){
                    //Aloca um espaço de memória para o número da thread, para que o número não se perca.
                    int* threadAllocated = malloc(sizeof(*threadAllocated));
                    *threadAllocated = threadNumber;

                    //Aloca um espaço de memória para o struct e inicializa suas variáveis, que serão passadas à função da thread.
                    struct requisitionArgs* args = malloc(sizeof(struct requisitionArgs));  
                    args->requisition = localQueue[--localLastOnTheQueue];
                    args->threadNumber = *threadAllocated;

                    //Cria a thread que efetivamente vai processar a requisição.
                    pthread_t auxThread = threads[threadNumber];
                    pthread_create(&auxThread, NULL, requisitionResponder, args);
                    usedThreads[threadNumber] = 1;
                }
            }
        }
    }
}

//Método que retorna 1 se tiver alguma thread disponível e 0 se não tiver nenhuma thread disponível.
int threadAvailable(){
    int i = 0;
    for(int i = 0; i < 10; i++){
        if(usedThreads[i] == 0) return 1;
    }
    return 0;
}

//Método que retorna o número de uma thread disponível, ou -1 se nenhuma estiver disponível.
int getAvailableThread(){
    int i = 0;
    for(int i = 0; i < 10; i++){
        if(usedThreads[i] == 0) return i;
    }
    return -1;
}