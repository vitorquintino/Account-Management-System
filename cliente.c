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
#define MSG_DEPOSITO "1) Deposito de dinheiro"
#define MSG_SAQUE "2) Saque de dinheiro"
#define MSG_TRANSFER "3) Transferencia de dinheiro"
#define MSG_LISTAGEM "4) Listagem de saldo"

int main(){

    /*char* a = "123";
    int b = atoi(a);
    printf("%d" , b);
    return -1;*/

    //Inicializa os sockets do c para o windows.
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    int client, connfd; 
    struct sockaddr_in servaddr, cli; 
    
    //Cria o socket do cliente, com os endereços padrões.
    client = socket(AF_INET, SOCK_STREAM, 0); 
    if(client == -1){
        //printf("Socket creation failed!");
        return -1;    
    }
    else{
        //printf("Socket created. Server started.\n");
    }

    //Coloca o IP e a porta do servidor, baseado em variáveis padrões dos sockets.
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT); 

    int n = 0;
    char requisicaoFinal[100];

    memset(requisicaoFinal, 'F', 100);

    int i = 0;
    while(i == 0){
        //Printa o menu ao cliente.
        printf("-------------------------------------------------");
        printf("\nEscolha uma opcao:\n");
        printf("%s\n", MSG_DEPOSITO);
        printf("%s\n", MSG_SAQUE);
        printf("%s\n", MSG_TRANSFER);
        printf("%s\n", MSG_LISTAGEM);
        printf("-------------------------------------------------\n");

        char opcao = '1';
        opcao = getchar();

        char opcaoConfirmar;
        switch(opcao){
            case '1':
                printf("Opcao de deposito escolhida.");
                requisicaoFinal[n++] = 'D';
                requisicaoFinal[n++] = '-';

                while ((requisicaoFinal[n++] = getchar()) != '\n') ;
                n--;

                printf("\nDigite o numero da conta de destino:");
                while ((requisicaoFinal[n++] = getchar()) != '\n') ;
                n--;

                requisicaoFinal[n++] = '-';

                printf("\nDigite o valor a ser depositado:");
                while ((requisicaoFinal[n++] = getchar()) != '\n') ;
                n--;
                
                opcaoConfirmar = 'X';

                printf("\nDeseja confirmar? (S/N)\n");
                while(opcaoConfirmar != 'S' && opcaoConfirmar != 'N'){
                    opcaoConfirmar = getchar();
                }
                
                if(opcaoConfirmar == 'S'){}
                else if(opcaoConfirmar == 'N'){
                    printf("\nTransacao abortada.\n\n");
                    printf("-------------------------------------------------\n");

                    while ((requisicaoFinal[n++] = getchar()) != '\n') ;

                    continue;
                }

                while ((requisicaoFinal[n++] = getchar()) != '\n') ;

                break;
            case '2':
                printf("Opcao de saque escolhida.");
                requisicaoFinal[n++] = 'S';
                requisicaoFinal[n++] = '-';

                while ((requisicaoFinal[n++] = getchar()) != '\n') ;
                n--;

                printf("\nDigite o numero da conta a ser sacada:");
                while ((requisicaoFinal[n++] = getchar()) != '\n') ;
                n--;

                requisicaoFinal[n++] = '-';

                printf("\nDigite o valor a ser sacado:");
                while ((requisicaoFinal[n++] = getchar()) != '\n') ;
                n--;
                
                opcaoConfirmar = 'X';

                printf("\nDeseja confirmar? (S/N)\n");
                while(opcaoConfirmar != 'S' && opcaoConfirmar != 'N'){
                    opcaoConfirmar = getchar();
                }
                
                if(opcaoConfirmar == 'S'){}
                else if(opcaoConfirmar == 'N'){
                    printf("\nTransacao abortada.\n\n");
                    printf("-------------------------------------------------\n");

                    while ((requisicaoFinal[n++] = getchar()) != '\n') ;

                    continue;
                }

                while ((requisicaoFinal[n++] = getchar()) != '\n') ;

                break;
            case '3':
                printf("Opcao de transferencia escolhida.");
                requisicaoFinal[n++] = 'T';
                requisicaoFinal[n++] = '-';

                while ((requisicaoFinal[n++] = getchar()) != '\n') ;
                n--;

                printf("\nDigite o numero da conta a ser debitada:");
                while ((requisicaoFinal[n++] = getchar()) != '\n') ;
                n--;

                requisicaoFinal[n++] = '-';

                printf("\nDigite o numero da conta a ser creditada:");
                while ((requisicaoFinal[n++] = getchar()) != '\n') ;
                n--;

                requisicaoFinal[n++] = '-';

                printf("\nDigite o valor a ser transferido:");
                while ((requisicaoFinal[n++] = getchar()) != '\n') ;
                n--;
                
                opcaoConfirmar = 'X';

                printf("\nDeseja confirmar? (S/N)\n");
                while(opcaoConfirmar != 'S' && opcaoConfirmar != 'N'){
                    opcaoConfirmar = getchar();
                }
                
                if(opcaoConfirmar == 'S'){}
                else if(opcaoConfirmar == 'N'){
                    printf("\nTransacao abortada.\n\n");
                    printf("-------------------------------------------------\n");

                    while ((requisicaoFinal[n++] = getchar()) != '\n') ;

                    continue;
                }

                while ((requisicaoFinal[n++] = getchar()) != '\n') ;
                break;
            case '4':
                requisicaoFinal[0] = 'L';
                break;
            default:
                printf("Esta opcao nao existe. Escolha outra.\n");
                printf("-------------------------------------------------\n\n");
                continue;
                break;
        }

        printf("%s", requisicaoFinal);
        n = 0;
        continue;

        //Conecta o cliente ao servidor.
        connect(client, (struct sockaddr *)&servaddr, sizeof(servaddr));

        //Manda um número arbitrário de requisições (todas iguais) ao servidor. Nesse caso, são 12.
        int i = 0;
        while(i<12){
            //Manda sua requisição ao servidor.
            send(client, requisicaoFinal, (int)strlen(requisicaoFinal), 0);
            printf("Request sent!\n");
            i++;
        }
        //Zera o buffer.
        memset(requisicaoFinal, 'F', 100);

        //Fecha a conexão com o servidor.
        close(client);

        n = 0;

        printf("Fim dessa transacao!\n");
        printf("-------------------------------------------------\n\n");
    }
}