/* -----------------------------------------------------------------------------
 * Title:       htmllib
 * File:        htmllib.c
 * Author:      Luiz Sol
 * Version:     0.0 (2017-05-27)
 * Date:        2017-05-27
 * Description: EP 4 de PSI2653.
 * -----------------------------------------------------------------------------
 */

#include "htmllib.h"
#include <string.h>
#include <stdio.h>
#include <dirent.h>

#define HTTPADDR "http://localhost:8080"
#define HTMLPART1 "<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><title>Listagem de diretório</title></head><body>"
#define HTMLPART3 "<ul style=\"list-style-type:none\">"
#define HTMLPART5 "</ul></body></html>"

int getlevelspath(char *path);
int generatepart2(char *path, char *output);
int generatepart4(char *path, char *output);

/* Profundidade do diretório
 *	  Determina a profundidade da árvore de diretórios
 *    definido pelo path.
 *    Exemplos:
 *    / = 0
 *    /home = 1
 *    /home/ = 1
 *    /home/user/downloads = 3
 *    user/downloads = 1
 *    ~ = 0
 *    ~/downloads = 1
 *
 *    path: diretorio a ser avaliado
 *    retorno: profundidade da árvore do path 
 *             (-1 em caso de erro)
 */
int getlevelspath(char *path){
	if(strlen(path) == 0){
		return -1;
	}
	int lvl = 0;
	for(int i = 1; path[i] != '\0'; i++){
		if(path[i-1] == '/'){
			lvl++;
		}
	}
	return lvl;
}

/* Gera a segunda parte do HTML
 *	  Gera a segunda parte do HTML a ser entregue ao
 *    usuário, contendo o caminho do diretório atual
 *    bem como os links para os diretórios raízes
 *    Exemplos:
 *    path = "/home/user/arquivos/" =>
 *    output = 
 *    "<h2>
 *    <a href=\"http://localhost:8080/\">/</a>
 *    <a href=\"http://localhost:8080/home/\">home/</a> 
 *    <a href=\"http://localhost:8080/home/user\">user/</a> 
 *    <a href=\"http://localhost:8080/home/user/arquivos/\">arquivos/</a>
 *    </h2>"
 *
 *    path: diretorio a ser avaliado
 *    output: string na qual sera apensado o resultado
 *    retorno: o resultado da operação
 *            (0 se sucesso, -1 em caso de erro)
 */
int generatepart2(char *path, char *output){
	strcat(output, "<h2>");
	int lvl = getlevelspath(path);
	//Gerar paths dos diretórios superiores:
	//Array com as strings do path dos diretórios superiores:
	char * pathdiretorios[lvl+1];
	//Array com as strings dos nomes dos diretórios superiores:
	char * nomediretorios[lvl+1];
	//Copiando o path original em todas as posições e
	//apagando os diretórios filho:
	for(int i = 0; i <= lvl; i++){
		//Definindo paths dos diretórios superiores
		pathdiretorios[i] = strdup(path);
		int k = i;
		int erase = 0;
		for(unsigned int j = 1; j < strlen(pathdiretorios[i]); j++){
			if(k == 0){
				erase = 1;
			}
			if(erase){
				pathdiretorios[i][j] = '\0';
			} else if(pathdiretorios[i][j] == '/'){
				k--;
			}
		}
		//Definindo nome dos diretórios superiores
		if(i == 0){ //Copiando o nome do diretório raiz
			nomediretorios[i] = strdup(pathdiretorios[i]);
		} else { //Copiando o nome somente do subdiretório
			nomediretorios[i] = 
				strdup((pathdiretorios[i] 
							+ strlen(pathdiretorios[i-1])));
		}
		//Adicionando as tags html à resposta
		strcat(output, "<a href=\"");
		strcat(output, HTTPADDR);
		strcat(output, pathdiretorios[i]);
		strcat(output, "\">");
		strcat(output, nomediretorios[i]);
		strcat(output, "</a> ");
	}
	strcat(output, "</h2>");
	return 0;
}

/* Gera a quarta parte do HTML
 *	  Gera a quarta parte do HTML a ser entregue ao
 *    usuário, contendo os conteúdos do diretório 
 *    apontado por path e seus respectivos links
 *    Exemplos:
 *    path = "/home/user/arquivos/" =>
 *    output = 
 *    "<li><a href="http://localhost:8080/home/user/arquivos/foto1.jpg">foto1.jpg</a></li>
 *     <li><a href="http://localhost:8080/home/user/arquivos/texto.txt">texto.txt</a></li>
 *     <li><a href="http://localhost:8080/home/user/arquivos/subdiretorio/">subdiretorio/</a></li>  
 *    "
 *
 *    path: diretorio a ser avaliado
 *    output: string na qual sera apensado o resultado
 *    retorno: o resultado da operação
 *            (0 se sucesso, -1 em caso de erro)
 */
int generatepart4(char *path, char *output){
	DIR           *d;
	struct dirent *dir;
	d = opendir(path);
	if(d){
		while ((dir = readdir(d)) != NULL){
			if(strcmp(dir->d_name,".") != 0 &&
				strcmp(dir->d_name,"..")){
					strcat(output, "<li><a href=\"");
					strcat(output, HTTPADDR);
					strcat(output, path);
					if(output[strlen(output)-1] != '/'){
						strcat(output, "/");
					}
					strcat(output, dir->d_name);
					strcat(output, "\">");
					strcat(output, dir->d_name);
					strcat(output, "</a></li>");
			}
		}
	closedir(d);
	}
	return 0;
}

/* Gera a resposta HTML que lista o diretório apontado por path
 *    path: diretorio a ser avaliado
 *    output: string na qual sera apensado o resultado
 *    retorno: o resultado da operação
 *            (0 se sucesso, -1 em caso de erro)
 */
int generatedirhtml(char *path, char *output){
	strcat(output, HTMLPART1);
	if(generatepart2(path, output) == -1){
		return -1;
	}
	strcat(output, HTMLPART3);
	if(generatepart4(path, output) == -1){
		return -1;
	}
	strcat(output, HTMLPART5);
	return 0;
}
