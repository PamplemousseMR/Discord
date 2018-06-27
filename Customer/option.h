#ifndef __OPTION_H__
#define __OPTION_H__

#define _XOPEN_SOURCE 199309L

#include <stdlib.h>     /* exit */
#include <stdio.h>	/* fprintf */
#include <getopt.h>	/* getopt */
#include <string.h>     /* strcpy */

#include "../Commun/commun.h"

extern key_t MSG_KEY;               /* la clé de la file de messages */
extern char ID[MAX_ID_SIZE];        /* l'id du client */

/* enregistre les options dans les variables 'extern' */
void getOption(int argc,char* argv[]);

#endif
