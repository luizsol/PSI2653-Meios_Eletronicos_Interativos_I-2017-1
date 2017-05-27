/* -----------------------------------------------------------------------------
 * Title:       htmllib
 * File:        htmllib.c
 * Author:      Luiz Sol
 * Version:     0.0 (2017-05-27)
 * Date:        2017-05-27
 * Description: EP 4 de PSI2653.
 * -----------------------------------------------------------------------------
 */

#ifndef HTMLLIB_H
#define HTMLLIB_H

/* Gera a resposta HTML que lista o diretório
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
int generatedirhtml(char *path, char *output);

#endif