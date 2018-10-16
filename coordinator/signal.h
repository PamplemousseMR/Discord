#ifndef __SIGNAL_H__
#define __SIGNAL_H__

#define _XOPEN_SOURCE 199309L

#include <stdlib.h>	/* exit */
#include <signal.h>     /* sigXXX */
#include <stdio.h>      /* printf */

/* initialise le signal SIGINT */
void initSignal();

#endif
