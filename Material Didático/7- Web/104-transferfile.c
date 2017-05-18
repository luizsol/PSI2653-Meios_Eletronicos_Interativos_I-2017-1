#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFERSIZE 1024
#define LINESIZE    80 
#define PATHSIZE    80

// transferfile()
//    Realiza a leitura do conteudo de um arquivo, identificado
//    por seu caminho (path), transferindo seu conteudo para 
//    outro arquivo ou socket identificado pelo descritor "outfd"

int transferfile(char *path,int output_fd)
   {
   int          input_fd;     // input file descriptor
   int          status;
   int          n;
   char         buffer[BUFFERSIZE];
   char         str[10];
   struct stat  statp;
   
   input_fd = open(path,O_RDONLY);
   if (input_fd < 0)
      {
      perror("ERRO chamada open(): Erro na abertura do arquivo: ");
      return(-1);
      }

   // obtem tamanho do arquivo
   status = fstat(input_fd,&statp);
   if (status != 0)
      {
      perror("ERRO chamada stat(): Erro no acesso ao arquivo: ");
      status = close(input_fd);
      return(-1);
      }
   sprintf(str,"SIZE=%d\n",statp.st_size);
   write(output_fd,str,strlen(str));

   // le arquivo , por partes 
   do
      {
      n = read(input_fd,buffer,BUFFERSIZE);
      if (n<0)
	   {
           perror("ERRO: chamada read(): Erro na leitura do arquivo: ");
           status = close(input_fd);
           return(-1);
           }
      write(output_fd,buffer,n);
      }
      while(n>0);

   status = close(input_fd);
   if (status == -1) 
	   {
	   perror("ERRO: chamada close(): Erro no fechamento do arquivo: " );
           return(-1);
           }
   return(0);
   }
   

int main()
   {
   char  line[LINESIZE];
   char  path[PATHSIZE];
   int   status;

   printf("Entre com o nome do arquivo: ");
   fgets(line,BUFFERSIZE,stdin);
   sscanf(line,"%s",path);

   // ler arquivo indicado por "path" e escrever-lo no descritor "1" 
   // (saida padrado - stdout). Para escrever em arquivo ou socket, troce "1" 
   // pelo file descriptor (fd) do arquivo ou socket

   status = transferfile(path,1);
   if (status == -1)
	   printf("Erro na transferencia do arquivo. \n");
   }
