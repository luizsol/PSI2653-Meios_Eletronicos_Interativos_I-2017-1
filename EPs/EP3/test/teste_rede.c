/** @file       teste_rede.c
 *  @brief      Programa para testar a biblioteca gui.h sem 
 *              sockets
 *                      
 *              Repository: https://github.com/luizsol/MEI
 *  @author     Luiz Sol (luizedusol@gmail.com)
 *  @date       2017/05/01
 */

#include "fila.h"

/** @file       teste_gui.c
 *  @brief      Programa para testar a biblioteca gui.h sem 
 *              sockets
 *                      
 *              Repository: https://github.com/luizsol/MEI
 *  @author     Luiz Sol (luizedusol@gmail.com)
 *  @date       2017/05/01
 */

#include "gui.h"
#include "rede.h"


int main(int argc, char *argv[]){

        InitGUI(MODOCLIENTE);

        while(execGUI){
                if(TamFila(filaInput)){
                        char * conteudo = 
                                malloc(TAMNOME + 2 + 
                                        TAMMAXINPUT + 1);
                
                *conteudo = '\0';

                strcat(conteudo, "EU        ");
                strcat(conteudo, ": ");
                char * output = PopFila(filaInput);
                strcat(conteudo, output);
                free(output);

                        InsereTextoChat(conteudo);
                }
        }
        return 0;
}