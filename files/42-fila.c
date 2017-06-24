// arquivo: fila.c
//    Este arquivo deve ser compilado junto com o programa principal:
//        cc -o prog prog.c fila.c

#include "fila.h" 

#define  FALSE 0
#define  TRUE  1

int InitFila(struct fila *F)
        {
	F->inicio = 0;
	F->fim    = 0;
	F->nitens = 0;
	F->tammax = TAMMAXFILA;
        }

int FilaVazia(struct fila *F)
        {
	if (F->nitens == 0)
		return(TRUE);
	else
		return(FALSE);
        }

int FilaCheia(struct fila *F)
        {
	if (F->nitens == F->tammax)
		return(TRUE);
	else
		return(FALSE);
        }

void InserirFila(struct fila *F, int item)
        {
	if (FilaCheia(F))
	        {
		printf("ERRO: tentando inserir em fila cheia. \n");
		exit(1);
	        }
	F->buffer[F->fim] = item;
	F->fim = (F->fim +1) % F->tammax;
	F->nitens = F->nitens + 1;
        }

int RetirarFila(struct fila *F) 
        {
	int item;

	if (FilaVazia(F))
	        {
		printf("ERRO: tentando retirar de fila vazia. \n");
		exit(1);
	        }
        item = F->buffer[F->inicio];
	F->inicio = (F->inicio + 1) % F->tammax;
	F->nitens = F->nitens - 1;
	return(item);
        }

