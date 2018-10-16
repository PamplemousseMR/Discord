#ifndef __OPTION_H__
#define __OPTION_H__

#define _XOPEN_SOURCE 199309L

#include <stdlib.h>     /* exit */
#include <stdio.h>	/* fprintf */
#include <unistd.h>	/* getopt */

#include "../commun/commun.h"

extern key_t MSG_KEY;   /* la cle de la file de messages */
extern key_t SEM_KEY;   /* la cle du tableau de semaphores */
extern key_t MEM_KEY;   /* la cle du segment de memoire partagee */
extern int CLEAR;

/* enregistre les options des les variables 'extern' */
void getOption(int, char*[]);

#endif
