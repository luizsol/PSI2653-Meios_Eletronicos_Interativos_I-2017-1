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


#include <linux/limits.h>


#define INI_DEFAULT_LDR_HIGH 0xFFFFFFFFu
#define INI_DEFAULT_LDR_LOW  0x00000000u

/**
 * Guarda os valores de calibração.
 */
struct calibration
{
	unsigned int high;
	unsigned int low;
};


#endif /* LDR_H */
