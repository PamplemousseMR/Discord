#define _XOPEN_SOURCE 199309L

#include <stdio.h>	/* printf */
#include <sys/msg.h>	/* msgget */
#include <stdlib.h>	/* exit */
#include <sys/shm.h>    /* Pour shmget, shmctl */
#include <sys/wait.h>   /* Pour waitpid */
#include <sys/types.h>  /* Pour pid_t */
#include <unistd.h>     /* Pour fork */
#include <sys/sem.h>    /* Pour semget, semctl, semop */

#include "../commun/commun.h"
#include "option.h"
#include "ncurses.h"
#include "signal.h"
#include "getch.h"

int msg;            /* la file de messages */
customer_t* mem;    /* la memoire partagée */
int sem;

/* =============== Etablir la connexion =============== */

static void enableConnexion()
{
    ask_t ask;
    rep_t rep;
    customer_t cus;
    int shmid;

    memset(&ask, 0, sizeof(ask_t));
    memset(&rep, 0, sizeof(rep_t));
    memset(&cus, 0, sizeof(customer_t));

    /* envoi d'une demande */
    ask.type = TYPE_ASK;
    ask.co = CONNECTION;
    if(msgsnd(msg, &ask, sizeof(ask_t) - sizeof(long), 0) == -1)
    {
        perror("[enableConnexion] Erreur lors de l'envoi de la demande");
        exit(EXIT_FAILURE);
    }
    /* réponse du coordinateur */
    if(msgrcv(msg, &rep, sizeof(rep_t) - sizeof(long), TYPE_REP, 0) == -1)
    {
        perror("[enableConnexion] Erreur lors de la reception du coordinateur");
        exit(EXIT_FAILURE);
    }
    /* si la reponse est valide*/
    if(rep.ok)
    {
        /*envoi des infos */
        cus.type = TYPE_CUSTOMER;
        cus.pid = getpid();
        strcpy(cus.id, ID);
        cus.nbMes = 0;
        if(msgsnd(msg, &cus, sizeof(customer_t) - sizeof(long), 0) == -1)
        {
            perror("[enableConnexion] Erreur lors de l'envoi de la configuration");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        /* le nombre de client max est atteint */
        printf("[enableConnexion] Le maximum de client est atteint\n");
        exit(EXIT_SUCCESS);
    }

    /* récuperation de la memoire partagée et des sémaphores*/
    if((shmid = shmget(rep.mem, 0, 0)) == -1)
    {
        perror("[enableConnexion] la memoire partagee n'existe pas");
        exit(EXIT_FAILURE);
    }
    if((mem = (customer_t*)shmat(shmid, NULL, 0)) == (void*)-1)
    {
        perror("[enableConnexion] Erreur lors de l'attachement du segment de memoire partagee");
        exit(EXIT_FAILURE);
    }

    /* récuperation des sémaphores */
    if((sem = semget(rep.sem, 0, 0)) == -1)
    {
        perror("[enableConnexion] Erreur lors de la recuperation du tableau de semaphores ");
        exit(EXIT_FAILURE);
    }

    printf("[enableConnexion] Connexion etablie\n");
}

/* =============== Sémaphores =============== */

static void can(int id, int check)
{
    struct sembuf op;
    op.sem_num = id;
    op.sem_op = -1;
    op.sem_flg = 0;
    refresh();
    if(semop(sem, &op, 1) == -1)
    {
        perror("[can] Erreur lors de l'operation sur le semaphore ");
        if(check)
            exit(EXIT_FAILURE);
    }
}

static void go(int id, int check)
{
    struct sembuf op;
    op.sem_num = id;
    op.sem_op = 1;
    op.sem_flg = 0;
    refresh();
    if(semop(sem, &op, 1) == -1)
    {
        perror("[go] Erreur lors de l'operation sur le semaphore ");
        if(check)
            exit(EXIT_FAILURE);
    }
}

/* =============== Boucle principale et mise à jour =============== */

int checkInput(char str[MESSAGE_SIZE], int send_pos)
{
    message_t message;
    if(strcmp(str,"")!=0)
    {
        if(strcmp(str,"EXIT")==0)
            return 1;
        else
        {
            wprintw(MESSAGES,"%s : %s\n",ID,str);
            wrefresh(MESSAGES);
            /* envoi du message au destinataire */
            if(send_pos>=0)
            {
                can(send_pos,1);
                message.type = TYPE_SEND_MESSAGE(mem[send_pos].pid);
                strcpy(message.id, ID);
                strcpy(message.msg, str);
                if(msgsnd(msg, &message, sizeof(message_t) - sizeof(long), 0) == -1)
                    perror("[checkInput] Erreur lors de l'envoi du message");
                ++mem[send_pos].nbMes;
                go(send_pos,1);
            }
        }
    }
    return 0;
}

void update()
{
    int i;
    int ok;
    message_t message_struct;
    struct input_line lnbuffer;
    int j;
    int total_customer;
    int end;
    int send_pos; /* la position du destinataire dans la mémoire partagée */

    make_buffer(&lnbuffer);

    /* initialise l'affichage */
    wclear(KEYBOARD);
    wprintw(KEYBOARD,"%s : ",ID);
    wrefresh(KEYBOARD);

    end = 0;
    while(!end)
    {
        send_pos = -1;
        j = 0;
        total_customer = 0;
        /* afficher les clients connectés */
        wclear(CUSTOMERS);
        mvwprintw(CUSTOMERS,0,CUSTOMER_COL/2-4,"CLIENTS\n");
        for(i=0 ; i<MAX_CLIENT ; ++i)
        {
            can(i,1);
            if(mem[i].pid != -1 && mem[i].pid != getpid())
            {
                ++total_customer;
                if(j==CURSOR_POS)
                {
                    wprintw(CUSTOMERS,"X:%d, %s\n",mem[i].pid, mem[i].id);
                    send_pos = i;
                }
                else
                    wprintw(CUSTOMERS," :%d, %s\n",mem[i].pid, mem[i].id);
                j++;
            }
            go(i,1);
        }
        wrefresh(CUSTOMERS);

        /* afficher les messages reçus */
        ok = 1;
        for(i=0 ; i<MAX_CLIENT && ok; ++i)
        {
            can(i,1);
            if(mem[i].pid == getpid())
            {
                ok = 0;
                while(mem[i].nbMes != 0)
                {
                    --mem[i].nbMes;
                    if(msgrcv(msg, &message_struct, sizeof(message_t) - sizeof(long), TYPE_REC_MESSAGE, 0) == -1)
                    {
                        perror("[update] Erreur lors de la reception de message");
                        exit(EXIT_FAILURE);
                    }
                    wprintw(MESSAGES,"%s : %s\n",message_struct.id, message_struct.msg);
                }
                wrefresh(MESSAGES);
            }
            go(i,1);
        }
        /* attendre une saisie clavier */
        if(getch_non_blocking(&lnbuffer, total_customer))
        {
            end = checkInput(lnbuffer.ln,send_pos);
            clean_buffer(&lnbuffer);
        }
        wclear(KEYBOARD);
        wprintw(KEYBOARD,"%s : %s",ID,lnbuffer.ln);
        wrefresh(KEYBOARD);
    }
}

/* =============== Fin du programme =============== */

static void clearProg()
{
    int ok;
    int i;
    message_t message_struct;
    ask_t ask;

    memset(&message_struct, 0, sizeof(message_t));
    memset(&ask, 0, sizeof(ask_t));

    /* envoi d'une demande d'arrêt au coordinateur */
    ask.type = TYPE_ASK;
    ask.co = DISCONECT;
    if(msgsnd(msg, &ask, sizeof(ask_t) - sizeof(long), 0) == -1)
        perror("[clearProg] Erreur lors de l'envoi de la demande");

    /* vider les messages s'il en reste */
    ok = 1;
    for(i=0 ; i<MAX_CLIENT && ok; ++i)
    {
        /* ne pas faire de 'exit' ici en cas de problème */
        can(i,0);
        if(mem[i].pid == getpid())
        {
            ok = 0;
            while(mem[i].nbMes != 0)
            {
                --mem[i].nbMes;
                if(msgrcv(msg, &message_struct, sizeof(message_t) - sizeof(long), TYPE_REC_MESSAGE, 0) == -1)
                    perror("[update] Erreur lors de la reception de message");
            }
        }
        go(i,0);
    }

    clearWindows();
    endNcurses();

    printf("[main] Deconnexion\n");
    _exit(EXIT_SUCCESS);
}

/* =============== Main =============== */

int main(int argc,char* argv[])
{
    getOption(argc, argv);
    /* file de messages */
    if((msg = msgget(MSG_KEY, 0)) == -1)
    {
        perror("[main] La file de messages n'existe pas");
        exit(EXIT_SUCCESS);
    }

    if(atexit(clearProg) != 0)
    {
        perror("[main] probleme lors de l'enregistrement du nettoyage");
        exit(EXIT_FAILURE);
    }

    initSignal();

    enableConnexion();
    initNcurses();
    initWindows();

    update();

    clearWindows();
    endNcurses();
    return 0;
}

