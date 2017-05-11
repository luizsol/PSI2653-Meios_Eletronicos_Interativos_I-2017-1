/** @file 	elemento.c
 *  @brief 	Implementa tabela com sd, nome 
 *              
 *         	Source: PSI2653 - Meios Eletrônicos Interativos I
 *         	Repository: https://github.com/luizsol/MEI
 *  @author 	Volnys Borges Bernal
 * 		Luiz Sol (luizedusol@gmail.com)
 *		Caio Vinícius Batista Pereira
 *  @date	2017/05/11
 */

#include "element.h"

/** @brief funcoes para obter e configurar os 
 *         as variaveis internas do elemento
 *  @param estado, nome, sd
 */

int Get_estado(Elemento *e){
	return e->estado;
	}

void Set_estado(Elemento *e, int s){
	e->estado=s;
}

char* Get_nome(Elemento *e){
	return e->nome;
}



void Set_nome(Elemento *e){
	printf("Digite o nome do usuario:");
	char data[64];
	fgets(data,sizeof data,stdin);
	printf("%s ",data);	
	e->nome=data;
}

int Get_sd(Elemento *e){
	return e->sd;
}

void Set_sd(Elemento *e, int sock){
	e->sd=sock;
	}




