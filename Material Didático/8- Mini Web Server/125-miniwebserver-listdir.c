#include <stdio.h>
#include <dirent.h>
#include <string.h>

#define min(x,y)   (((x)<(y))?(x):(y))

void append(char *dest,int buffersize, char *src)
    {
    int d;
    int i;

    d = strlen(dest);
    for (i=0; i<min(strlen(src),buffersize-1-d); i++)
       dest[d+i] = src[i];
    dest[d+i] = '\0';
    }

// Lista diretorio
//    path: diretorio a ser listado
//    buffer: buffer que contera' a string com a sequencia ASCII
//            resultado da listagem do diretorio (finalizada por '\0'
//    bufffersize: tamanho do buffer

void lista_diretorio(char *path,char *buffer,int buffersize)
    {
    DIR           * dirp;
    struct dirent * direntry;
    char            linha[80];
    int             i = 0;

    dirp = opendir(path);
    if (dirp ==NULL)
       {
       perror("ERRO: chamada opendir(): Erro na abertura do diretorio: ");
       snprintf(buffer,buffersize,"Erro na listagem diretorio!\n");
       return;
       }
    buffer[0]='\0';
    while (dirp) 
        {
	// Enquanto nao chegar ao fim do diretorio, leia cada entrada
        direntry = readdir(dirp);
	if (direntry == NULL)
           // chegou ao fim
           break;
        else
           {
           // ler entrada
           append(buffer,buffersize,direntry->d_name);
           append(buffer,buffersize,"\n");
           }
        }
    closedir(dirp);
    }

#define LINESIZE       80
#define PATHSIZE       80
#define LISTBUFFERSIZE 500

int main()
   {
   char line[LINESIZE];
   char path[PATHSIZE];
   char string[LINESIZE];
   char listbuffer[LISTBUFFERSIZE];
   int  fd;   // file descriptor (utilize 1 para terminal ou 
              //   socket descriptor para enviar via socket)

   printf("Entre com um diretorio: ");
   fgets(line,LINESIZE,stdin);
   sscanf(line,"%s\n",path);

   lista_diretorio(path,listbuffer,LISTBUFFERSIZE);
   sprintf(string,"\n>>> Diretorio=%s\n",path);

   fd=1;  // 1 == standar output (terminal)
   write(fd,string,strlen(string));
   write(fd,listbuffer,strlen(listbuffer));
   }
