/** @file 	elemento.h
 *  @brief 	Implementa tabela com sd, nome 
 *              
 *         	Source: PSI2653 - Meios Eletrônicos Interativos I
 *         	Repository: https://github.com/luizsol/MEI
 *  @author 	Volnys Borges Bernal
 * 		Luiz Sol (luizedusol@gmail.com)
 *		Caio Vinícius Batista Pereira
 *  @date	2017/05/11
 */

#ifndef __ELEMENTO__H
#define __ELEMENTO__H

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>

#define  FALSE 0
#define  TRUE  1
#define  DEBUG 1 //Modo debug

/**
 *  Estrutura que implementa a Fila com buffer circular
 */
typedef struct elemento {
	/*@{*/
	int estado; // indice que simboliza o estado atual sendo 0=livre e 1=ocupada 
	char nome[11]; // nome do usuário
	int sd; // socket adress do client
	} Elemento;

/** @brief funcoes para obter e configurar os 
 *         as variaveis internas do elemento
 *  @param estado, nome, sd
 */


int Get_estado(Elemento *e);

void Set_estado(Elemento *e, int s);

char* Get_nome(Elemento *e);

void Set_nome(Elemento *e);

int Get_sd(Elemento *e);

void Set_sd(Elemento *e, int sock); 





