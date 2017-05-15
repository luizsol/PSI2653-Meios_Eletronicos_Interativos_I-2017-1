# Exercício Programa 3: Chat UDP

Este programa tem por objetivo implementar uma sala de chat para múltiplos usuários, a exemplo de programas como o [mIRC](https://pt.wikipedia.org/wiki/MIRC).

Essa versão do programa implementa somente o cliente. Uma outra tentativa de implementação do cliente e do servidor está dentro da pasta `OLD`.

## Requisitos de Projeto
Segundo o [enunciado](82-Trabalho-UDP-chat-v6.pdf) do exercício deverão ser implementados tanto o cliente como o servidor, e ambos deverão respeitar um protocolo de comunicação predeterminado.

### Servidor
* **Porta de comunicação**: Deve aguardar requisições na porta 10.000/UDP
* **Capacidade da sala**: Deve permitir sessões de chat com até 3 usuários simultaneamente
* **Mensagem de lotação**: Deve apresentar a mensagem `Número de usuários excedido` quando exceder a capacidade de 3 usuários de chat (3 clientes chat)
* **Armazenamento do socketaddress**: Deve guardar o `socketaddress` do cliente quando receber mensagem de pedido de entrada no chat (`USER`)
* **Armazenamento do nome de usuário**:Deve armazenar o nome do usuário
* **Verificação do socketaddress**: Deve verificar, para cada mensagem recebida, o `socketaddress` de origem.
* **Heartbeat**: Deve, a cada 30s, encaminhar mensagem `TESTE` a cada usuário com a finalidade de verificar se ainda está ativo. Caso duas mensagens de teste não sejam respondidas, deve realizar a saída do usuário do Chat
* **Mensagem de Logout**: Quando receber uma mensagem `EXIT` deve enviar a seguinte mensagem a todos os clientes `<user>: Saiu`

### Cliente
* **Porta de comunicação**: Deve enviar datagramas UDP para a porta 10.000 do servidor chat
* **Threads de comunicação**: Deve possuir dois threads:
  * Thread transmissor: Obtém a mensagem do usuário e a transmite ao servidor
  * Thread receptor: Aguarda mensagens do servidor e apresenta no terminal
* **Apresentação**: A tela de recepção deve apresentar as mensagens para o usuário da seguinte forma:
  * `Maria__> Olá a todos`
  * `Ricardo> Olá Maria`
  * `Jose__> Olá Maria, seja bem vinda`
* **Heartbeat**: Deve, a cada 30s, encaminhar mensagem `TESTE` ao servidor com a finalidade de verificar se a conexão está ativa ou se o servidor está ativo. Caso duas mensagens de teste não sejam respondidas, deve mostrar na tela do usuário e terminar o programa.

### Comunicação
* **Formato da Mensagens**:
  * A mensagem utilizada na comunicação entre o cliente e servidor é codificada em ASCII.
  * A mensagem possui duas partes:
     * Comando: tamanho de 5 caracteres
     * Dado: tamanho variável, de 1 a 90 bytes (incluindo caractere `\0`)
* **Mensagens do Cliente**:

Nome | Descrição | Formato | Dimensões | Comportamentos esperados | Possíveis Mensagens de Resposta 
--- | --- | --- | --- | --- | ---
`USER` | Primeira mensagem enviada do cliente ao servidor. Serve como uma solicitação de entrada na sala de chat. | `USER:<nome do usuário>\0` | `USER:`[5 bytes]; `<nome do usuário>`[1 a 10 bytes]; `\0`[1 byte] | 1) Servidor deve armazenar o nome e o `socketaddress` do novo usuário; 2) O servidor deve enviar uma resposta ao usuário | `OKOK`(Sucesso), `BUSY`(Número de usuários excedido) 
`UP` | Envio de texto de mensagem do cliente ao servidor. | `UP :<texto da mensagem>\0` | `UP :`[5 bytes]; `<texto da mensagem>`[1 a 79 bytes]; `\0`[1 byte] | 1) O servidor deve obter o nome do usuário de sua tabela de controle a partir do `socketaddress` da mensagem recebida. | -
`EXIT` | Pedido de saída do Chat | `EXIT:\0` | `EXIT:`[5 bytes]; `\0`[1 byte] | 1) Servidor deve liberar slot ocupado pelo usuário; 2) Servidor deve gerar mensagem `DOWN` a todos clientes com o conteúdo `<usuário> saiu.` | `BYE` (Sucesso)
`TEST` | Pedido de teste de conexão | `TEST:\0` | `TEST:`[5 bytes]; `\0`[1 byte] | 1) Receptor da mensagem `TEST` deve responder com mensagem `OKOK`; 2) A mensagem deve ser enviada a cada 30s | -
`OKOK` | Resposta de um teste de conexão. | `OKOK:\0` | `OKOK:`[5 bytes]; `\0`[1 byte] | - | -

* **Mensagens do Servidor**:

Nome | Descrição | Formato | Dimensões | Comportamentos esperados | Possíveis Mensagens de Resposta 
--- | --- | --- | --- | --- | ---
`DOWN` | Envio de texto de mensagem do servidor ao cliente. | `DOWN:<nome do usuário> :<texto da mensagem>\0` | `DOWN:`[5 bytes]; `<nome do usuário>`[10 bytes]; `:<texto da mensagem>`[1 a 79 bytes] ; `\0`[1 byte] | - | -
`BUSY` | Indicação de excesso de usuários. | `BUSY:\0` | `BUSY:`[5 bytes]; `\0`[1 byte] | - | -
`BYE` | Confirmação de saida de cliente | `BYE :\0` | `BYE :`[5 bytes]; `\0`[1 byte] | - | -
`TEST` | Pedido de teste de conexão | `TEST:\0` | `TEST:`[5 bytes]; `\0`[1 byte] | 1) Receptor da mensagem `TEST` deve responder com mensagem `OKOK`; 2) A mensagem deve ser enviada a cada 30s | -
`OKOK` | Resposta de um teste de conexão. | `OKOK:\0` | `OKOK:`[5 bytes]; `\0`[1 byte] | - | -

## Estrutura do Projeto

### lista.c | lista.h
Implementam as estruturas e funções de filas com suporte a concorrência.

### fila.c | fila.h
Implementam as estruturas e funções de filas com suporte a concorrência.

### gui.c | gui.h
Implementam as estruturas e funções da interface homem-máquina com suporte a concorrência.

### rede.c | rede.h
Implementam as estruturas e funções de acesso aos sockets de rede com suporte a concorrência.

### cliente.c | cliente.h
Implementam o programa do cliente

## Utilização

### Pre-requisitos
* Sistema Operacional: Linux 
* Compilador: GCC 6.2 ou maior
* Bibliotecas: Pthreads e Ncurses

### Compilação
* Compilação de cliente e servidor: `make`
* Compilação do cliente: `make cliente`
* Compilação do servidor: `make servidor`
* Apagar todos os arquivos gerados pela compilação: `make clean`

### Execução do servidor
* Nome do binário: `servidor-chat`
* Sinopse: `servidor-chat [-p |-P <porta>] [-c | -C <max_cl>] [-v | -V <verbose>] [-o | -O <arquivo>]`
* Opções
  * `-p <porta>` Porta a ser utilizada pelo servidor (padrão: 10000)
  * `-c <max_cl>` Número máximo de clientes simultâneos (padrão: 3)
  * `-v <verbose>` Opção para mostrar o log de conversa durante a execução do programa (padrão: 1 (sim))
  * `-o <arquivo>` Arquivo para armazenamento do log de conversa (padrão: nenhum)

### Execução do cliente
* Nome do binário: `cliente-chat`
* Sinopse: `cliente-chat [-p |-P <porta>] [-o | -O <arquivo>]`
* Opções
  * `-p <porta>` Porta a ser utilizada pelo cliente (padrão: 10000)
  * `-o <arquivo>` Arquivo para armazenamento do log de conversa (padrão: nenhum)
