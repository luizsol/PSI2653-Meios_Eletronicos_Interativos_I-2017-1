// Arquivo: fila.h
//    Este arquivo deve ser incluido no programa principal da seguinte forma:
//    #include "fila.h"
//


// Declaracao da fila

#define TAMMAXFILA 4

struct fila {
	int buffer[TAMMAXFILA];
	int inicio;
	int fim;
	int nitens;
	int tammax;
} ;

int InitFila(struct fila *F);
int FilaVazia(struct fila *F);
int FilaCheia(struct fila *F);
void InserirFila(struct fila *F, int item);
int RetirarFila(struct fila *F);

	

