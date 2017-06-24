# PSI2653 - Meios Eletrônicos Interativos I

* Instituição: [Escola Politécnica da Universidade de São Paulo](http://www.poli.usp.br/)
* Professor: [Volnys Borges Bernal](http://www.lsi.usp.br/~volnys/academic/curriculum/Curriculum-Lates-Volnys-Borges-Bernal.htm) ([volnys@lsi.usp.br](volnys@lsi.usp.br))
* Data: 1º semestre de 2017

## Apresentação da disciplina
* [Apresentação da disciplina - PSI 2653](files/01-PSI-2653.pdf)

## Revisão de arquitetura de computadores
* [Revisão - Arquitetura de computadores](files/02-RevArquitetura-v6.pdf)
* [Revisão - Endianess](files/03-Endianess-v2.pdf)

## Sistemas operacionais
* [Introdução aos sistemas operacionais](files/04-IntroSistemasOperacionais-v12.pdf)
* [Visão geral do sistema operacional UNIX](files/06-Unix-v5.pdf)
* [Visão geral do sistema operacional Linux](files/07-Linux-v5.pdf)
* [Processos](files/08-Processos-v10.pdf)
	* [Programa "perimetro.c"](files/09-perimetro.c)
	* [Programa "fatorial.c"](files/09-fatorial2.c)
	* [Programa "fork1.c"](files/10-fork1.c)
	* [Programa "fork2.c"](files/11-fork2.c)
* [Pilha de execução](files/12-PilhaExecucao-v10.pdf)
	* [Pilha de execução - exercício](files/13-PilhaExecucao-exercicio-v2.pdf)
	* [Leitura complementar - Pilha de execução](files/14-LeituraComplementar-Pilha-execucao.pdf)
	* [Leitura complementar - Pilha de execução e variáveis locais](files/15-LeituraComplementar-Pilha-execucao-e-variaveis-locais.pdf)
* [Threads](files/22-threads-v8.pdf)
	* [Programa "mythread.c"](files/23-mythread.c)
* [Sinais](files/25-Sinais.pdf)
	* [Programa sinal_intr.c](files/27-sinal_intr.c)
	* [Programa sinal_todos.c](files/28-sinal_todos.c)
	* [Programa temporizador.c](files/29-temporizador.c)

## Sincronização
* [Condição de disputa](files/30-CondicaoDisputa-v5.pdf)
* [Impasse e inanição](files/33-Impasse-Inanicao-v3.pdf)
* [Problemas clássicos de sincronização](files/31-ProblemasClassicos-v6.pdf)
* [Introdução à sincronização](files/35-IntroSincronizacao-v24.pdf)
* [Mutex](files/36-Mutex-v27.pdf)
	* [Programa prodcons_mutex_buffer.c](files/38-prodcons_mutex_buffer.c)
	* [Programa prodcons_mutex_buffer_yield.c](files/39-prodcons_mutex_buffer_yield.c)
* [Trabalho para sala de aula: Produtor consumidor com mutex e fila](files/40-Trabalho-prodcons-mutex-fila-v5.pdf)
	* [Arquivo de programa: fila.h](files/41-fila.h)
	* [Arquivo de programa: fila.c](files/42-fila.c)
* [Primitivas de sincronização de bloqueio explícito](files/43-PrimitivasBloqueioExplicito-v24.pdf)
	* [Programa worker.c](files/44-worker.c)
* [Trabalho 1: Produtor-consumidor com primitivas wait & signal](files/45-Trabalho-prodcons-wait-signal-v4.pdf) | [Resolução](EPs/EP1)
* [Semáforos](files/47-Semaforo-v23.pdf)
* [Trabalho 2: Produtor Consumidor com fila sincronizada com semáforos](files/48-Trabalho-prodcons-fila-com-semaforo-v5.pdf) |  [Resolução](EPs/EP2)

## Revisão TCP/IP
* [Revisão - Serviços de rede TCP/IP](files/60-Revisao-IntroServicoRedes-v7.pdf)
* [Revisão - Protocolo UDP](files/61-Revisao-udp-v5.pdf)
* [Revisão - Protocolo TCP](files/62-Revisao-tcp-v10.pdf)
* [Revisão - Protocolo DNS](files/63-dns-intr-v4.pdf)

## Programação Sockets
* [Resolução de nomes](files/71-ResolucaoNomes-v9.pdf)
	* [Programa: resolv-nome.c](files/72-resolv-nome.c)
	* [Programa: resolv-serv-upd.c](files/73-resolv-serv-udp.c)
	* [Programa: resolv-proto.c](files/74-resolv-proto.c)
* [Conversão de formatos](files/75-ConversaoFormatos-v6.pdf)
* [Programação Sockets](files/76-ProgSockets-v12.pdf)
* [Cliente UDP](files/77-ClienteUDP-v16.pdf)
	* [Programa: udp-echo-client.c](files/78-udp-echo-client.c)
	* [Programa: udp-daytime-client.c](files/79-udp-daytime-client.c)
* [Servidor UDP](files/80-ServidorUDP-v8.pdf)
	* [Programa udp-echo-server.c](files/81-udp-echo-server.c)
* [Trabalho 3: Chat UDP](files/82-Trabalho-UDP-chat-v6.pdf) | [Resolução](EPs/EP3)
* [Cliente TCP](files/83-ClienteTCP-v9.pdf)
	* [Programa: tcp-echo-client.c](files/84-tcp-echo-client.c)
* [Servidor TCP](files/85-ServidorTCP-v12.pdf)
	* [Programa: tcp-echo-server.c](files/86-tcp-echo-server.c)

## WEB
* [Protocolo HTTP](files/101-HTTP-kurose-v3.pdf)
* [Protocolo HTTP - Campos do cabeçalho](files/102-HTTP-CamposCabecalho-v1.pdf)
	* [Exercícios sobre protocolo HTTP](files/103-HTTP-exercicios-v4.pdf)
	* [Programa TransferFile.c](files/104-transferfile.c)
* [Linguagem HTML](files/105-HTML-v9.pdf)
	* [Página HTML "Hello World"](files/106-HTML-HelloWorld.html)
	* [Página HTML "Imagem USP"](files/107-HTML-ImagemUSP.html)
	* [Página HTML "Notícias"](files/108-HTML-Noticias.html)
	* [Página HTML "NameForm"](files/109-HTML-NameForm.html)
	* [Página HTML "Atribute Range"](files/110-HTML-AtributeRange.html)

## Mini Web Server
* [Trabalho 4: Mini web server](files/121-Trabalho-TCP-server-miniwebserver-v7.pdf) | [Resolução](EPs/EP4)
	* [Dicas para o trabalho Mini Web Server](files/122-Dicas-miniwebserver-v3.pdf)
	* [Função composepath()](files/123-miniwebserver-composepath.c)
	* [Função getcurrentdir()](files/124-miniwebserver-getcurrentdir.c)
	* [Função transferfile()](files/126-miniwebserver-transferfile.c)
	* [Função listdir()](files/125-miniwebserver-listdir.c)

## IOT
* [Raspberry PI](181-Raspberry_Pi_3_modelo_B.pdf)
* Controle de GPIO Página
	* O controle dos pinos de GPIO do RaspberryPI por um programa executado sobre o sistema operacional Linux pode ser realizado através de arquivos especiais.
	* O diretório /sys/class/gpio contém dois arquivos especiais que permitem habilitar o acesso a determinadas portas do GPIO:
		* export
		* unexport
	* Ao escrever o valor numérico da porta no arquivo export, o sistema gera um arquivo expecial que permite realizar o controle da porta. Para remover este controle basta fazer o mesmo no aquivo unexport.
	* Mais informações em: [https://www.embarcados.com.br/gpio-da-raspberry-pi-linguagem-c/](https://www.embarcados.com.br/gpio-da-raspberry-pi-linguagem-c/)
* [Uso de GPIO no Raspberry Pi](files/Entrada-Saída_no_RPi-GPIO.pdf)
* [Sensor de luminosidade e controle PWM de intensidade de lâmpada LED](files/185-LDR-pwm-raspberry-pi.pptx)
	* [Leitura complementar: OpenCV, RaspiCam e WiringPi](files/186-Raspicam_wiringpi.pdf)

## Linux
* [Cartão de referência - utilitários UNIX](files/201-unix-ref.pdf)
* [Sistema de arquivos UNIX](files/202-SistemaArquivos.pdf)

## Sistema Lumiar
* [Trabalho 5 (final): Sistema lumiar](files/221-Trabalho-Lumiar-v2.pdf) | [Resolução](EPs/EP5)
