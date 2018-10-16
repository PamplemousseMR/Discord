#ifndef __GETLINE_H__
#define __GETLINE_H__

#define _XOPEN_SOURCE 199309L

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <ncurses.h>    /* les tailles des fenêtres */

#include "ncurses.h"

extern int CURSOR_POS;  /* la position du curseur sur la liste des clients */

/* pour enregistrer les 'char' au fur et à mesure */
typedef struct input_line
{
    char ln[MESSAGE_SIZE];
    int cursor;
}input_line_t;

/* créer le buffer */
void make_buffer(input_line_t*);
/* recuperer le 'char' de manière non bloquante, renvoie 0 si la chaîne frappée n'est pas terminée */
int getch_non_blocking(input_line_t*, int);
/* vider le buffer */
void clean_buffer(input_line_t*);

#endif
