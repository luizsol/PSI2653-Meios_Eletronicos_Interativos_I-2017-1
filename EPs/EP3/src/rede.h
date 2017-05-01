/** @file 	rede.h
 *  @brief 	Implementa uma interface para utilizacao de
 * 			sockets UDP
 *         	
 *         	Repository: 
 *			https://github.com/luizsol/MEI/tree/master/EPs/EP3
 *  @author	Luiz Sol (luizedusol@gmail.com)
 *  @date	2017/05/01
 */

#ifndef __REDE__H
#define __REDE__H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define STDPORTA 10000	/* Porta para comunicacao UDP padrao*/

int porta;	/* Porta para comunicacao UDP 					*/
int sd;	/* Socket descriptor 								*/
int status; /* Status da comunicacao 						*/

#endif