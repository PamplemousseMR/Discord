#include "getch.h"

int CURSOR_POS = 0;

void make_buffer(input_line_t* buf)
{
    int i;
    for(i=0 ; i<MESSAGE_SIZE ; ++i)
        buf->ln[i] = ' ';
    buf->ln[0] = '\0';
    buf->cursor = 0;
}

void clean_buffer(input_line_t* buf)
{
    int i;
    for(i=0 ; i<MESSAGE_SIZE ; ++i)
        buf->ln[i] = ' ';
    buf->ln[0] = '\0';
}

static void add_char(input_line_t *buf, char ch)
{
    if(buf->cursor<MESSAGE_SIZE-1)
    {
        buf->ln[buf->cursor] = ch;
        buf->ln[buf->cursor+1] = '\0';
        ++buf->cursor;
    }
}

static int souris_getpos(int *x, int *y, int *bouton)
{
    MEVENT event;
    int resultat = getmouse(&event);

    if(resultat == OK)
    {
        *x = event.x;
        *y = event.y;
        *bouton = event.bstate;
    }
    return resultat;
}

static int handle_input(input_line_t *buf, int key, int total_customer)
{
    int X, Y, bouton;
    if(!(key & KEY_CODE_YES) && isprint(key))
    {
        add_char(buf, key);
        return 0;
    }

    switch(key)
    {
    case ERR:
        break;
    case KEY_MOUSE:
        if(souris_getpos(&X, &Y, &bouton) == OK)
        {
            if(bouton & BUTTON1_CLICKED)
            {
                if(X>MESSAGE_COL && Y<MESSAGE_LINE)
                {
                    CURSOR_POS = Y-2;
                    if(CURSOR_POS<0)CURSOR_POS=0;
                    if(CURSOR_POS>=total_customer) CURSOR_POS=total_customer-1;
                }
                else if(X>MESSAGE_COL && Y>=MESSAGE_LINE)
                    exit(EXIT_SUCCESS);
            }
        }
        break;
    case KEY_BACKSPACE:
        if(buf->cursor>0)
        {
            --buf->cursor;
            buf->ln[buf->cursor] = '\0';
        }
        break;
    case '\t':
        add_char(buf, '\t');
        break;
    case KEY_ENTER:
    case '\r':
    case '\n':
        buf->cursor = 0;
        return 1;
        break;
    }
    return 0;
}

int getch_non_blocking(input_line_t *buf, int total_customer)
{
    int key = getch();
    if(key == ERR)
        return 0;
    return handle_input(buf, key, total_customer);
}
