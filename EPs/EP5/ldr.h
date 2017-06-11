/* -----------------------------------------------------------------------------
 * Title:       ldr
 * File:        ldr.h
 * Author:      Luiz Sol
 * Version:     0.0 (2017-06-11)
 * Date:        2017-06-11
 * Description: Eexercício 5 de PSI2653.
 * -----------------------------------------------------------------------------
 */

#ifndef LDR_H
#define LDR_H

#define UPERIOD 500000 // Período entre observações (500[ms])

#include "lumiarlib.h"
//Para instalar wiringPi:
//git clone git://git.drogon.net/wiringPi
//cd wiringPi
//./build


/** 
 * Inicializa e executa a thread responsável por calcular o valor da 
 * luminosidade calculado pelo ldr
 *
 * @return o status da inicialização. 0 => ok, -1 erro
*/
int ldr_initandrun(void);

#endif /* LDR_H */
