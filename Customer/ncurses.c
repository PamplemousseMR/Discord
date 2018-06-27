#include "ncurses.h"

WINDOW* MESSAGES = NULL;
WINDOW* KEYBOARD = NULL;
WINDOW* CUSTOMERS = NULL;
WINDOW* EXIT = NULL;

static WINDOW* MESSAGES_BOX = NULL;
static WINDOW* CUSTOMERS_BOX = NULL;
static WINDOW* KEYBOARD_BOX = NULL;
static WINDOW* EXIT_BOX = NULL;

void initNcurses()
{
    initscr();              /* Démarre le mode ncurses */
    cbreak();               /* Pour les saisies clavier (desac. mise en buffer) */
    keypad(stdscr, TRUE);   /* Active les touches spécifiques */
    curs_set(FALSE);        /* Masque le curseur */
    refresh();              /* Met à jour l'affichage */
    timeout(UPDATE);        /* Entree non bloquante */
    intrflush(stdscr, 0);
    nonl();
    noecho();               /* désactive les affichages */
    mousemask(BUTTON1_CLICKED, NULL);   /* Active le clic gauche de la souris*/
    if(has_mouse() != TRUE)
    {
        fprintf(stderr, "[initNcurses] Aucune souris n'est détectée.\n");
        exit(EXIT_FAILURE);
    }
}

void endNcurses()
{
    endwin();
}

void initWindows()
{
    MESSAGES = newwin(MESSAGE_LINE-2, MESSAGE_COL-2, 1, 1);
    MESSAGES_BOX = newwin(MESSAGE_LINE, MESSAGE_COL, 0, 0);
    box(MESSAGES_BOX, ACS_VLINE, ACS_HLINE);
    wrefresh(MESSAGES_BOX);
    wrefresh(MESSAGES);
    scrollok(MESSAGES, TRUE);

    CUSTOMERS = newwin(CUSTOMER_LINE-2, CUSTOMER_COL-2, 1, MESSAGE_COL+1);
    CUSTOMERS_BOX = newwin(CUSTOMER_LINE, CUSTOMER_COL, 0, MESSAGE_COL);
    box(CUSTOMERS_BOX, ACS_VLINE, ACS_HLINE);
    wrefresh(CUSTOMERS_BOX);
    wrefresh(CUSTOMERS);
    scrollok(CUSTOMERS, TRUE);

    KEYBOARD = newwin(KEYBOARD_LINE-2, KEYBOARD_COL-2, MESSAGE_LINE+1, 1);
    KEYBOARD_BOX = newwin(KEYBOARD_LINE, KEYBOARD_COL, MESSAGE_LINE, 0);
    box(KEYBOARD_BOX, ACS_VLINE, ACS_HLINE);
    wrefresh(KEYBOARD_BOX);
    wrefresh(KEYBOARD);
    scrollok(KEYBOARD, TRUE);

    EXIT = newwin(EXIT_LINE-2, EXIT_COL-2, MESSAGE_LINE+1, KEYBOARD_COL+1);
    EXIT_BOX = newwin(EXIT_LINE, EXIT_COL, MESSAGE_LINE, KEYBOARD_COL);
    box(EXIT_BOX, ACS_VLINE, ACS_HLINE);
    wrefresh(EXIT_BOX);
    mvwprintw(EXIT,0,EXIT_COL/2 - 6,"DECONNEXION");
    wrefresh(EXIT);
}

void clearWindows()
{
    delwin(MESSAGES);
    delwin(MESSAGES_BOX);
    delwin(CUSTOMERS);
    delwin(CUSTOMERS_BOX);
    delwin(KEYBOARD);
    delwin(KEYBOARD_BOX);
}
