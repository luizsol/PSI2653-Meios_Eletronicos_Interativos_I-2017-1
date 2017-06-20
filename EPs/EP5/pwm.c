/* -----------------------------------------------------------------------------
 * Title:       pwm
 * File:        pwm.c
 * Author:      Gabriel Crabbé -- Caio Pereira
 * Version:     0.0 (2017-06-12)
 * Date:        2017-06-12
 * Description: Exercício 5 de PSI2653.
 * -----------------------------------------------------------------------------
 */

#include "lumiar.h"


/**
 * Thread principal do driver PWM.
   Estipula quando a escala de 0 a 100?
 */
void *pwmService(void *conf)
{
	if(export_gpio(16)==0)
		perror("Erro no export");
	if(direction_gpio(16,OUTPUT)==0)
		perror("Erro em configurar direção");
	
	while(1){
	value_gpio(16,1);	
	wait(30); //tempo em alto
	value_gpio(16,0);
	wait(30); //tempo em baixo
	}
	return NULL;
}


/**
 * Função de interface com a main. 
   Valores de 0-100
 */
int setOperatingPoint(int val)
{
	
		

	return 0;
}


/**
 * Função para exportar pinos 
*/	
int export_gpio(int pin)
{
     arquive = open ("/sys/class/gpio/export", O_WRONLY);
     if (arquive==-1)
     {
             printf("Arquivo abriu incorretamente\n");
             return 0;
     }
     snprintf(buffer, 3, "%d", pin);
     if(write(arquive, buffer, 3) == -1)
     {
             close(arquive);
             return 0;
     }
     close(arquive);
 
     return 1;
}



/*
 * Configura direção do pino (in/out)
 */	
int direction_gpio(int pin, int direction)
{
     arquive=0;
     snprintf(path, 35, "/sys/class/gpio/gpio%d/direction", pin);
     arquive = open (path, O_WRONLY);
     if (arquive==-1)
     {
             return 0;
     }
     snprintf(buffer, 3, "%d", pin);
     if (write( arquive, ((direction == INPUT)?"in":"out"), 3 )==-1)
     {
             close(arquive);
             return 0;
     }
     close(arquive);
     return 1;
}



//  Escreve no pino
 int value_gpio(int pin, int value)
{
     arquive=0;
     snprintf(path, 35, "/sys/class/gpio/gpio%d/value", pin);
     arquive = open(path, O_WRONLY);
     if (arquive == -1)
     {
             return 0;
     }
     if (write (arquive, ((value == HIGH)?"1":"0"), 1) == -1)
     {
             close(arquive);
             return 0;
     }
     close(arquive);
     return 1;
}

//Libera pino 
int unexport_gpio(int pin)
{
     arquive = open ("/sys/class/gpio/unexport", O_WRONLY);
     if (arquive==-1)
     {
             printf("Arquivo abriu incorretamente\n");
             return 0;
     }
     if(write(arquive, buffer, 3) == -1)
     {
             close(arquive);
             return 0;
     }
     return 1;
}


