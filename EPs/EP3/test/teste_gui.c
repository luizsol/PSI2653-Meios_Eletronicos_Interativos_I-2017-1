#include "gui.h"


int main(int argc, char *argv[]){
        int ch;

        InitGUI(MODOCLIENTE);

        InsereTextoChat("MARIA     : Ola maria, como vai voce?");
        sleep(1);
        InsereTextoChat("JOAO      : Vou bem, e voce?");
        sleep(1);
        InsereTextoChat("MARIA     : Vou bem tambem.");
        sleep(1);
        InsereTextoChat("JOAO      : 12345678901234567890123456789012345678901234567890123456789012345678901234567890");
        sleep(1);
        InsereTextoChat("MARIA     : Ola maria, como vai voce?");
        sleep(1);
        InsereTextoChat("JOAO      : Vou bem, e voce?");
        sleep(1);
        InsereTextoChat("MARIA     : Vou bem tambem.");
        sleep(1);
        InsereTextoChat("MARIA     : HUEHUEHUEHUEHUEHUEHUE");
        sleep(1);
        InsereTextoChat("MARIA     : HUEHUEHUEHUEHUEHUEHUE");
        sleep(1);
        InsereTextoChat("MARIA     : HUEHUEHUEHUEHUEHUEHUE");
        sleep(1);
        InsereTextoChat("MARIA     : HUEHUEHUEHUEHUEHUEHUE");
        sleep(1);
        InsereTextoChat("MARIA     : HUEHUEHUEHUEHUEHUEHUE");
        sleep(1);
        InsereTextoChat("MARIA     : HUEHUEHUEHUEHUEHUEHUE");
        sleep(1);
        InsereTextoChat("MARIA     : Essa eh boa");
        sleep(1);
        InsereTextoChat("MARIA     : Essa vai essa vai essa vai Essa vai essa vai essa vai Essa vai essa vai essa vai Essa vai essa vai essa vai ");
        sleep(1);
        InsereTextoChat("MARIA     : HUEHUEHUEHUEHUEHUEHUE");
        sleep(1);
        InsereTextoChat("MARIA     : Ola maria, como vai voce?");
        sleep(1);
        InsereTextoChat("JOAO      : Vou bem, e voce?");
        sleep(1);
        InsereTextoChat("MARIA     : Vou bem tambem.");
        sleep(1);
        InsereTextoChat("MARIA     : 1");
        sleep(1);

        while((ch = getch()) != KEY_F(1))
        {       
                // switch(ch)
                // {       case KEY_LEFT:
                //                 destroy_win(my_win);
                //                 my_win = create_newwin(height, width, starty,--startx);
                //                 break;
                //         case KEY_RIGHT:
                //                 destroy_win(my_win);
                //                 my_win = create_newwin(height, width, starty,++startx);
                //                 break;
                //         case KEY_UP:
                //                 destroy_win(my_win);
                //                 my_win = create_newwin(height, width, --starty,startx);
                //                 break;
                //         case KEY_DOWN:
                //                 destroy_win(my_win);
                //                 my_win = create_newwin(height, width, ++starty,startx);
                //                 break;  
                // }
                // wrefresh(janelaTitulo);
                // wrefresh(janelaChat);
                // wrefresh(janelaInput);
                // refresh();
        }
                
        endwin();                       /* End curses mode                */
        return 0;
}