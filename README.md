# Account-Management-System

Este repositório foi feito como EP1 da matéria de Redes de Computadores.

O servidor funciona como um sistema de administração de um banco, com algumas contas e seu saldo em dinheiro. Ele possui 4 funções 
básicas a serem chamadas pelo cliente: a função de depósito de dinheiro em alguma conta, a função de saque de dinheiro de alguma 
conta, a função de transferência de dinheiro entre contas e a função de listar o saldo de alguma conta. Tudo isso é feito por meio
da comunicação com a outra aplicação de banco de dados, que representa um banco de dados em memória. 

O cliente, por sua vez, se comunica com o servidor após escolher uma das funções por meio de um menu apresentado via texto. Ele escolhe
a opção e faz uma requisição via socket ao servidor.
