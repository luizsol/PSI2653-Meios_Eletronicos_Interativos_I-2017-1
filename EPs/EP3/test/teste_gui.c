#include "gui.h"


int main(int argc, char *argv[]){
        int ch;

        InitJanelas(MODOSERVIDOR);


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