// Arquivo: fila.h
//    Este arquivo deve ser incluido no programa principal da seguinte forma:
//    #include "fila.h"
//


// Declaracao da fila

#define TAMMAXFILA 4

typedef struct fila {
	int buffer[TAMMAXFILA];
	int inicio;
	int fim;
	int nitens;
	int tammax;
} Fila;

int InitFila(Fila *F);
int FilaVazia(Fila *F);
int FilaCheia(Fila *F);
void InserirFila(Fila *F, int item);
int RetirarFila(Fila *F);

	

