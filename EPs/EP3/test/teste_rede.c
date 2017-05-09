/** @file       teste_rede.c
 *  @brief      Programa para testar a biblioteca rede.h
 *                      
 *              Repository: 
 *				https://github.com/luizsol/MEI
 *  @author     Luiz Sol (luizedusol@gmail.com)
 *  @date       2017/05/01
 */

#include "rede.h"

extern int    errno;

int main(){  
    if(InitSocket(STDPORTA) == L_ERRO){
        printf("Erro no socket TX\n");
        return 1;
    }

	while(1){
        EnviaRawMsg("Opa!\n", inet_addr("192.168.1.36"), htons(10000));
		sleep(1);
	}
}
