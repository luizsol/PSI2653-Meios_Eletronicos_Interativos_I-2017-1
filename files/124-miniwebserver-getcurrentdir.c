#include <stdio.h>
#include <string.h>
#include <libgen.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define  PATHSIZE 200

// getcurrentdir()
//      Retorna o diretorio corrente atual
//      Entrada: buffer para string e tamanho deste buffer
//      Saida: ponteiro para string

char * getcurrentdir(char *path, int pathsize)
    {
    char *p;

    p = getcwd(path,pathsize);
    if (p==NULL)
      perror("Erro na chamada getcwd");
    return(p);
    }
	
int main()
   {
   char    * p;
   char      path[PATHSIZE];
   
   p = getcurrentdir(path,PATHSIZE);
   if (p != NULL)
      printf("PATH = %s\n",path);
   }
