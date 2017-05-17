# Exercício Programa 3: Chat UDP

Este programa tem por objetivo implementar o cliente de uma sala de chat para múltiplos usuários, a exemplo de programas como o [mIRC](https://pt.wikipedia.org/wiki/MIRC).

## Utilização

### Pre-requisitos
* Sistema Operacional: Linux 
* Compilador: GCC 6.2 ou maior
* Bibliotecas: Pthreads e Ncurses

### Compilação
* Compilação de cliente `make`
* Apagar todos os arquivos gerados pela compilação: `make clean`

### Execução do cliente
* Nome do binário: `cliente-chat`
* Sinopse: `cliente-chat -i <IPSrv> [-p <porta>] [-u <Usuário>] [-d (0|1)]`
* Opções:
  * `-i <IPSrv>` IP do servidor de chat (Obrigatório)
  * `-p <porta>` Porta a ser utilizada pelo cliente (padrão: `10000`) (Opcional)
  * `-u <Usuário>` Nome de usuário que deseja utilizar (padrão: `Anonimo`) (Opcional)
  * `-d (0|1)` Ativar modo debug (padrão: `0` = desativado) (Opcional)

## Requisitos de Projeto
Segundo o [enunciado](82-Trabalho-UDP-chat-v6.pdf) do exercício os grupos deverão implementar o cliente ou o servidor, e ambos deverão respeitar um protocolo de comunicação predeterminado.

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

Nosso grupo ficou responsável por implementar o cliente do chat. A princípio tentamos implementar também o servidor (e por isso algumas partes do nosso código parecem ser inúteis mas isso se deve ao fato de que foram implementadas para serem genéricas) mas por falta de tempo nos restringimos a implementar somente o necessário.

O projeto tem a seguinte estrutura de arquivos:
```
.
├── 82-Trabalho-UDP-chat-v6.pdf
├── Makefile
├── obj
├── README.md
├── src
│   ├── cliente.c
│   ├── cliente.h
│   ├── fila.c
│   ├── fila.h
│   ├── gui.c
│   ├── gui.h
│   ├── lista.c
│   ├── lista.h
│   ├── rede.c
│   └── rede.h
└── test
    ├── echo_rede.c
    ├── teste_fila.c
    ├── teste_gui.c
    └── teste_lista.c
```
O arquivo `82-Trabalho-UDP-chat-v6.pdf` contém o enunciado do projeto e o arquivo `README.md` contém o texto que você está lendo.

O arquivo `Makefile` automatiza o processo de compilação do projeto bem como dos programas de teste das bibliotecas.

O diretório `obj` é o destino dos arquivos `.o` gerados durante o processo de compilação, já o diretório `test` contém o código-fonte dos programas que testam as bibliotecas utilizadas pelo cliente de chat.

Por último, o diretório `src` contém os códigos-fonte utilizados na compilação do cliente de chat. Abaixo seguem uma breve descrição de cada um deles.

### lista.c | lista.h
Implementam as estruturas e funções de listas ligadas com suporte a concorrência utilizando mutex e semáforos.

A alocação dos elementos na lista se dá dinamicamente (através de `malloc()` e `free()`), portanto é recomendado que o usuário da biblioteca evite alterar as estruturas `Item` e `Lista` diretamente, devendo preferencialmente utilizar as seguintes funções:

* `Lista * NewLista();`: cria e inicializa uma nova fila.
* `int InsereNovoItemIndice(Lista *L, void * conteudo, int indice);`: insere um novo conteudo na lista em um determinado índice. Função bloqueante.
* `int PushInicio(Lista *L, void * conteudo);`: insere um conteúdo no início da lista.
* `int PushFim(Lista *L, void * conteudo);`: insere um conteúdo no fim da lista.
* `void * RemoveItemIndice(Lista *L, int indice);`: remove um conteúdo da lista na posição de um dado índice. Retorna `NULL` caso o índice esteja fora do range de índices. Função bloqueante.
* `void * PopInicio(Lista *L)`: remove o primeiro conteúdo da lista. retorna `NULL`caso a lista esteja vazia. Função bloqueante.
* `void * PopFim(Lista *L)`: remove o último conteúdo da lista. Função bloqueante.
* `void * GetConteudoIndice(Lista *L, int indice)`: retorna um ponteiro para o conteúdo de um item na posição de um determinado índice. Função bloqueante.
* `void PrintLista(Lista *L);`: imprime o conteúdo da lista via `printf()`.
* `int TamLista(Lista *L)`: determina o tamanho da lista.

### fila.c | fila.h
Implementam as estruturas e funções de filas com suporte a concorrência. Ela utiliza a biblioteca de Listas na sua implementação. Suas principais funções são:

* `Fila * NewFila();`: cria e inicializa uma nova fila.
* `int PushFila(Fila *F, void * conteudo);`: insere um conteúdo no fim da fila.
* `void * PopFila(Fila *F);`: remove um conteúdo do início da fila. Função bloqueante.
* `void PrintFila(Fila *F);`: imprime o conteúdo da fila via `printf()`.
* `int TamFila(Fila *F);`: determina o tamanho da fila.

### gui.c | gui.h
Implementam as estruturas e funções da interface gráfica com suporte a concorrência. Ela utiliza a biblioteca [ncurses](https://en.wikipedia.org/wiki/Ncurses) na sua implementação.

A primeira função a ser executada nessa biblioteca deve ser a `void InitGUI(int modo);`, que tem como parâmetro o modo de execução, que pode assumir os valores `MODOCLIENTE` e `MODOSERVIDOR`. Caso essa função seja executada no `MODOSERVIDOR` serão criadas 2 janelas, a primeira contendo o título do programa (`GUIjanelaTitulo`) e a segunda responsável por conter o texto do chat (`GUIjanelaChat`). Já no `MODOCLIENTE` será criada também outra janela que conterá o texto a ser digitado pelo usuário.

Assim que é executada, a função `InitGUI()` irá inicializar as variáveis de controle da interface gráfica bem como configurar o ncurses. A seguir ela irá calcular as dimensões e posicionamento das janelas a serem criadas, apresentará as telas e iniciará uma thread responsável por processar os inputs do usuário (`void * _InputManager(void * arg)`).

A thread de processamento de input do usuário armazena os caracteres válidos inserido pelo usuário e assim que detecta os caractere `\n` adiciona o conteúdo desse buffer na `filaInput`, fila essa que posteriormente será processada por outra thread da biblioteca `rede.c|rede.h`. Outra atribuição dessa thread é detectar o pressionamento da tecla `F2` e sinalizar o encerramento do programa alterando o valor da flag `execGUI`.

Outras funções podem adicionar textos à janela do chat por meio da função `void InsereTextoChat(char * texto)`, que adiciona o texto à lista `textosChat`, lista essa que será apresentada na janela de chat. Caso a quantidade de linhas a serem apresentadas na janela exceda o espaço disponível, os textos mais antigos serão removidos da lista até que haja espaço para o novo texto.

Todas as outros funções servem propósitos operacionais internos e não devem ser utilizadas externamente.

### rede.c | rede.h
Implementam as estruturas e funções de acesso e uso de sockets UDP bem como o protocolo de comunicação utilizado pelo chat.

A função que dá início ao processo de inicialização das estruturas, variáveis e threads de comunicação do cliente é a `void InitCliente(char * srvIP, char * porta, char * nome);`. Para ela devem ser passados o IP do servidor, a porta de comunicação e o nome de usuário a ser utilizado. Essa função irá inicializar a interface gráfica, converter e armazenar os parâmetros de comunicação com o servidor (`int AdicionaHost(char * nome, unsigned long s_addr, unsigned short sin_port);`), incializar os sockets de comunicação com o servidor (`int InitSocket(int porta);`) e inicializar a thread responsável por constantemente enviar as mensavens digitadas pelo usuário (`_ThreadCliente`);

Dentor da função `InitCliente()`, além de se configurar e inicializar o socket de comunicação com o servidor, também são inicializadas outras 3 threads, sendo elas a `void * _ThreadTX(void * arg)`, a `void * _ThreadRX(void * arg)` e a `void * _ThreadCorreios(void * arg)`. As duas primeiras, como os nomes indicam, são responsáveis por continuamente enviar e receber mensagens de texto do chat (`RawMsg`). Já a `_ThreadCorreios()` processa as mensagens recebidas e toma decisões do que fazer a partir dos seus conteúdos e do protocolo de comunicação.

Outra thread de grande importância para o chat é a `void * _threadKeepAlive(void * host)`, que de tempos em tempos verifica se o servidor ainda está on-line por meio da mensagem `TEST` e caso seja detectada a desconeção ela inicia o procedimento de fechamento do chat.

Todas as outros funções servem propósitos operacionais internos e não devem ser utilizadas externamente.

### cliente.c | cliente.h
Implementam o programa do cliente.

Além da função `main()` a única outra função aqui implementada é a `void processa_parametros(int argc, char* argv[])`, que processa os parâmetros da chamada do programa. Uma vez processados esses parâmetros, a função `InitCliente()` é executada, dando início à execução do cliente do chat.