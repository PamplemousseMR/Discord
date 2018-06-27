#ifndef __NCURSES_H__
#define __NCURSES_H__

#define _XOPEN_SOURCE 199309L

#include <stdlib.h>     /* NULL */
#include <ncurses.h>    /* ncurses */

#include "../Commun/commun.h"

#define MESSAGE_LINE (LINES-3)
#define MESSAGE_COL ((COLS/3)*2)
#define CUSTOMER_LINE (LINES-3)
#define CUSTOMER_COL (COLS - (COLS/3)*2)
#define KEYBOARD_LINE 3
#define KEYBOARD_COL ((COLS/3)*2)
#define EXIT_LINE 3
#define EXIT_COL (COLS - (COLS/3)*2)

extern WINDOW* MESSAGES;        /* le fenêtre de messages*/
extern WINDOW* KEYBOARD;        /* la fenêtre de clavier */
extern WINDOW* CUSTOMERS;       /* la fenêtre des clients */

/* initialise ncurses */
void initNcurses();
/* étient ncuses */
void endNcurses();
/* initalise les fenêtres */
void initWindows();
/* éteint les fenêtres */
void clearWindows();

#endif
