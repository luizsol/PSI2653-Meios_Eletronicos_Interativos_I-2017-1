/** @file       teste_rede.c
 *  @brief      Programa para testar a biblioteca rede.h
 *                      
 *              Repository: 
 *				https://github.com/luizsol/MEI
 *  @author     Luiz Sol (luizedusol@gmail.com)
 *  @date       2017/05/01
 */

#include "rede.h"

extern int    errno;

int main(){  
    InitServidor("10000");
    while(execGUI){}
    return 0;
}