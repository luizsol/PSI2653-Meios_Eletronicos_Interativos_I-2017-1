#Exercício Programa 3: Chat UDP

Este programa tem por objetivo implementar uma sala de chat para múltiplos usuários, a exemplo de programas como o [mIRC](https://pt.wikipedia.org/wiki/MIRC).

##Requisitos
Segundo o [enunciado](82-Trabalho-UDP-chat-v6.pdf) do exercício deverão ser implementados tanto o cliente como o servidor, e ambos deverão respeitar um protocolo de comunicação predeterminado.

###Servidor
* **Porta de comunicação**: Deve aguardar requisições na porta 10.000/UDP
* **Capacidade da sala**: Deve permitir sessões de chat com até 3 usuários simultaneamente
* **Mensagem de lotação**: Deve apresentar a mensagem `Número de usuários excedido` quando exceder a capacidade de 3 usuários de chat (3 clientes chat)
* **Armazenamento do socketaddress**: Deve guardar o `socketaddress` do cliente quando receber mensagem de pedido de entrada no chat (USER)
* **Armazenamento do nome de usuário**:Deve armazenar o nome do usuário
* **Verificação do socketaddress**: Deve verificar, para cada mensagem recebida, o `socketaddress` de origem.
* **Heartbeat**: Deve, a cada 30s, encaminhar mensagem `TESTE` a cada usuário com a finalidade de verificar se ainda está ativo. Caso duas mensagens de teste não sejam respondidas, deve realizar a saída do usuário do Chat
* **Mensagem de Logout**: Quando receber uma mensagem `EXIT` deve enviar a seguinte mensagem a todos os clientes `<user>: Saiu`

###Cliente
* **Porta de comunicação**: Deve enviar datagramas UDP para a porta 10.000 do servidor chat
* **Threads de comunicação**: Deve possuir dois threads:
 * Thread transmissor: Obtém a mensagem do usuário e a transmite ao servidor
 * Thread receptor: Aguarda mensagens do servidor e apresenta no terminal
* **Apresentação**: A tela de recepção deve apresentar as mensagens para o usuário da seguinte forma:
 * `Maria__> Olá a todos`
 * `Ricardo> Olá Maria`
 * `Jose__> Olá Maria, seja bem vinda`
* **Heartbeat**: Deve, a cada 30s, encaminhar mensagem `TESTE` ao servidor com a finalidade de verificar se a conexão está ativa ou se o servidor está ativo. Caso duas mensagens de teste não sejam respondidas, deve mostrar na tela do usuário e terminar o programa.

###Comunicação
* **Formato da Mensagens**:
 * A mensagem utilizada na comunicação entre o cliente e servidor é codificada em ASCII.
 * A mensagem possui duas partes:
     * Comando: tamanho de 5 caracteres
     * Dado: tamanho variável, de 1 a 90 bytes (incluindo caractere `\0`)
* **Mensagens do Cliente**:

Nome | Descrição | Formato | Dimensões | Comportamentos esperados | Possíveis Mensagens de Resposta 
--- | --- | --- | --- | --- | ---
`USER` | Primeira mensagem enviada do cliente ao servidor. Serve como uma solicitação de entrada na sala de chat. | `USER:<nome do usuário>\0` | `USER:`[5 bytes]; `<nome do usuário>`[1 a 10 bytes]; `\0`[1 byte] | 1) Servidor deve armazenar o nome e o `socketaddress` do novo usuário; 2) O servidor deve enviar uma resposta ao usuário | `OKOK`(Sucesso), `BUSY`(Número de usuários excedido) 
`UP` | Envio de texto de mensagem do cliente ao servidor. | `UP :<texto da mensagem>\0` | `UP :`[5 bytes]; `<texto da mensagem>`[1 a 79 bytes]; `\0`[1 byte] | -
`EXIT` | Pedido de saída do Chat | `EXIT:\0` | `EXIT:`[5 bytes]; `\0`[1 byte] | `BYE` (Sucesso)

 * __*USER*__: primeira mensagem enviada do cliente ao servidor. Serve como uma solicitação de entrada na sala de chat
     * **Formato**: 

Mensagem | Conteúdo | `\0`
--- | --- | --- 
5 bytes | Max 10 bytes | 1 byte
--- | --- | ---
`USER:` | `<nome do usuário>` | `\0`
 
 * teste

##Utilização

###Pre-requisitos

###Compilação

###Execução do servidor

###Execução do cliente

