/** @file   cliente.c
 *  @brief  Implementa o cliente do chat
 *                      
 *         	Repository: 
 *			https://github.com/luizsol/MEI/tree/master/EPs/EP3
 *  @author Luiz Sol (luizedusol@gmail.com)
 *  @date   2017/05/15
 */

#include "cliente.h"

/* Variáveis globais 										*/
char portaSRV[20]; /* Porta de comunicação com o servidor 	*/
char ipSRV[IPMAXSTRSIZE]; /* IPV4 do servidor 				*/
char userName[MAXNAMESIZE]; /* Nome do usuário 				*/

/** @brief Função de entrada
 *
 *  @param argc quantidade de parâmetros passados na execução
 *  @param argv array de parâmetros passados na execução
 *  @return o status de fechamento do programa
 */
int main(int argc, char * argv[]){
	
	portaSRV[0] = '\0';
	ipSRV[0] = '\0';
	userName[0] = '\0';

	sprintf(portaSRV, "%d", 10000); //Valor padrão da porta
	modoDebug = 0;
	strcpy(userName, "Anonimo");

	processa_parametros(argc, argv);

    InitCliente(ipSRV, portaSRV, userName);

    while(1){}

    return 0;
}

/** @brief Processa os parâmetros utilizados na chamada 
 *         do programa
 *
 *  @param argc quantidade de parâmetros passados na execução
 *  @param argv array de parâmetros passados na execução
 */
void processa_parametros(int argc, char* argv[]){
	int ipOk = 0;
	char opt;
	if(argc == 2){	/* Usuário só passou o IP como parâmetro*/
		if(strlen(optarg) < IPMAXSTRSIZE){
				strcpy(ipSRV, optarg);
				ipOk = 1;
			}
	} else {
		while ((opt = getopt(argc, argv, "p:i:u:d:")) != -1){
			switch(opt) {
	    	case 'p': /* Porta 								*/
				if(strlen(optarg) < 20){
	    			strcpy(portaSRV, optarg);
				}
				break;
			case 'i':	/* IP 								*/
				if(strlen(optarg) < IPMAXSTRSIZE){
					strcpy(ipSRV, optarg);
					ipOk = 1;
				}
				break;
			case 'd':	/* Modo Debug						*/
				if(atoi(optarg) >= 1){
					modoDebug = 1;
				}
				break;
			case 'u':	/* Usuário							*/
				if(strlen(optarg) < MAXNAMESIZE){
					strcpy(userName, optarg);
				}
				break;
				
			case '?':
	    		/* Casos em que o usuario utiliza um 		*
	    		 * parametro mas nao determina um valor 	*/
				if (optopt == 'd') {
	    				modoDebug = 1;
	    			
	  			} 
				break;
			}
		}
	}
	if(ipOk == 0){
		printf("ERRO: IP do servidor (-i XXX.XXX.XXX.XXX) nao fornecido. Saindo\n");
		exit(1);
	}

}
