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
    //Inicializa os sockets do c para o windows.
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    int client, connfd; 
    struct sockaddr_in servaddr, cli; 
    
    //Cria o socket do cliente, com os endereços padrões.
    client = socket(AF_INET, SOCK_STREAM, 0); 

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
                printf("Opcao de listagem escolhida.");
                requisicaoFinal[n++] = 'L';
                requisicaoFinal[n++] = '-';

                while ((requisicaoFinal[n++] = getchar()) != '\n') ;
                n--;

                printf("\nDigite o numero da conta a ser listada:");
                while ((requisicaoFinal[n++] = getchar()) != '\n') ;
                n--;

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
            default:
                printf("Esta opcao nao existe. Escolha outra.\n");
                printf("-------------------------------------------------\n\n");
                
                break;
        }

        //Conecta o cliente ao servidor.
        if(connect(client, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
            //printf("Servidor fora do ar! Tente novamente em alguns segundos.\n");          
        }

        //Manda sua requisição ao servidor.
        send(client, requisicaoFinal, (int)strlen(requisicaoFinal), 0);
        printf("Request sent!\n");

        //Zera o buffer.
        memset(requisicaoFinal, 'F', 100);

        char response[100];
        int x = recv(client, response, sizeof response, 0);

        printf("\nServidor: %s\n", response);

        close(client);
        send(client, "", 0, 0);
        //Fecha a conexão com o servidor.
        

        n = 0;

        printf("Fim dessa transacao!\n");
        printf("-------------------------------------------------\n\n");
    }
}