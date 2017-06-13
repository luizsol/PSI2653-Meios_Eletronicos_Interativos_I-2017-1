/* -----------------------------------------------------------------------------
 * Title:       web
 * File:        web.h
 * Author:      Gabriel Crabbé
 * Version:     0.0 (2017-06-12)
 * Date:        2017-06-12
 * Description: Eexercício 5 de PSI2653.
 * -----------------------------------------------------------------------------
 */

#ifndef WEB_H
#define WEB_H

#define WORKER_THREADS 3


struct webConfig
{
	char base[PATH_MAX];
	unsigned short port;
};


#endif /* WEB_H */
