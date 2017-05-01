#include "gui.h"


int main(int argc, char *argv[]){

        InitGUI(MODOCLIENTE);

        // InsereTextoChat("MARIA     : Ola maria, como vai voce?");
        // sleep(1);
        // InsereTextoChat("JOAO      : Vou bem, e voce?");
        // sleep(1);
        // InsereTextoChat("MARIA     : Vou bem tambem.");
        // sleep(1);
        // InsereTextoChat("JOAO      : 12345678901234567890123456789012345678901234567890123456789012345678901234567890");
        // sleep(1);
        // InsereTextoChat("MARIA     : Ola maria, como vai voce?");
        // sleep(1);
        // InsereTextoChat("JOAO      : Vou bem, e voce?");
        // sleep(1);
        // InsereTextoChat("MARIA     : Vou bem tambem.");
        // sleep(1);
        // InsereTextoChat("MARIA     : HUEHUEHUEHUEHUEHUEHUE");
        // sleep(1);
        // InsereTextoChat("MARIA     : HUEHUEHUEHUEHUEHUEHUE");
        // sleep(1);
        // InsereTextoChat("MARIA     : HUEHUEHUEHUEHUEHUEHUE");
        // sleep(1);
        // InsereTextoChat("MARIA     : HUEHUEHUEHUEHUEHUEHUE");
        // sleep(1);
        // InsereTextoChat("MARIA     : HUEHUEHUEHUEHUEHUEHUE");
        // sleep(1);
        // InsereTextoChat("MARIA     : HUEHUEHUEHUEHUEHUEHUE");
        // sleep(1);
        // InsereTextoChat("MARIA     : Essa eh boa");
        // sleep(1);
        // InsereTextoChat("MARIA     : Essa vai essa vai essa vai Essa vai essa vai essa vai Essa vai essa vai essa vai Essa vai essa vai essa vai ");
        // sleep(1);
        // InsereTextoChat("MARIA     : HUEHUEHUEHUEHUEHUEHUE");
        // sleep(1);
        // InsereTextoChat("MARIA     : Ola maria, como vai voce?");
        // sleep(1);
        // InsereTextoChat("JOAO      : Vou bem, e voce?");
        // sleep(1);
        // InsereTextoChat("MARIA     : Vou bem tambem.");
        // sleep(1);
        // InsereTextoChat("MARIA     : 1");
        // sleep(1);

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