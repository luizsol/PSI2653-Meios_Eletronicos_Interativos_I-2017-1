#include <stdio.h>
#include <string.h>
#include <libgen.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define  BUFFERSIZE 200

// Compose Path: altera PATH 
//    oldpath: path original (ex: /home/jose/test
//    path   : path para ser alterado (ex: "..", "/tmp", "prog1" )
//             (imagine na forma: "cd ..", "cd /tmp", "cd prog1")
//    newpath: path resultante da composicao de oldpath e path
//             (ex: /home/jose, /tmp, /jome/jose/test/prog1)

int composepath(char *oldpath,char *path,char *newpath)
    {
    char * olddir;
    char * oldbase;
    char * oldpath1;
    char * oldpath2;
    char * dir;
    char * base;
    char * path1;
    char * path2;
    int    status;

    oldpath1 = strdup(oldpath);
    oldpath2 = strdup(oldpath);
    olddir   = dirname(oldpath1);
    oldbase  = basename(oldpath2);

    path1 = strdup(path);
    path2 = strdup(path);
    dir   = dirname (path1);
    base  = basename(path2);

    strcpy(newpath,"/");

    if (path[0]=='/')
        {
        strcpy(newpath,path);
        status=0;
        }
    else if ((strcmp(dir,".")==0) && (strcmp(base,".")==0))
        {
        strcpy(newpath,oldpath);
        status=0;
        }
    else if ((strcmp(dir,".")==0) && (strcmp(base,"..")==0))
        {
        strcpy(newpath,olddir);
        status=0;
        }
    else if ((strcmp(dir,".")==0) && (strcmp(oldbase,"/")==0))
        {
        strcpy(newpath,oldpath);
        strcat(newpath,path);
        status=0;
        }
    else if (strcmp(dir,".")==0)
        {
        strcpy(newpath,oldpath);
        strcat(newpath,"/");
        strcat(newpath,path);
        status=0;
        }
    else
        {
        strcpy(newpath,oldpath);
        status=-1;
        }
    free(oldpath1);
    free(oldpath2);
    free(path1);
    free(path2);
    return(status);
    } 

int main()
   {
   int          status;
   char         line[BUFFERSIZE];
   char         path[BUFFERSIZE];
   char         oldpath[BUFFERSIZE];
   char         newpath[BUFFERSIZE];
   int          fd;
   struct stat statp;
   
   getcwd(oldpath,BUFFERSIZE);
   printf("PATH = %s\n",oldpath);
   while (1)
       {
       printf("> cd ");
       fgets(line,BUFFERSIZE,stdin);
       sscanf(line,"%s",path);
       status = composepath(oldpath,path,newpath);
       if (status == -1)
          {
          printf("ERRO \n");
          continue;
          }

       // verifica existencia e permissao de acesso
       status = stat(newpath,&statp);
       if (status != 0)
          {
          perror("Erro stat()");
          printf("ERRO: no acesso ao diretorio!\n");
          continue;
          }
       if (S_ISDIR(statp.st_rdev))
          {
          printf("ERRO: diretorio nao encontrado!\n");
          continue;
          }

       printf("PATH = %s\n",newpath);
       strcpy(oldpath,newpath);
       }
   }
