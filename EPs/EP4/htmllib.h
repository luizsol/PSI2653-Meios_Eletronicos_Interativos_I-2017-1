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

/* Gera a resposta HTML que lista o diretório apontado por path
 *    path: diretorio a ser avaliado
 *    output: string na qual sera apensado o resultado
 *    retorno: o resultado da operação
 *            (0 se sucesso, -1 em caso de erro)
 */
int generatedirhtml(char *path, char *output);

#endif