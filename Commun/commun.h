#ifndef __COMMUN_H__
#define __COMMUN_H__

#include <sys/types.h>  /* pid_t */

#define DEFAULT_MSG_KEY 1956            /* clef de la file de message par default */
#define DEFAULT_SEM_KEY 1875            /* clef du tableau de semaphores par default */
#define DEFAULT_MEM_KEY 1963            /* clef de la memoire partager par default */

#define MAX_ID_SIZE 15                  /* taille maximum d'un id client */
#define MAX_CLIENT 10                   /* maximum de client */

#define TYPE_ASK 1                      /* valeur du type pour envoyer une demande au coordinateur */
#define TYPE_REP 2                      /* valeur du type pour envoyer la reponse a un client */
#define TYPE_CUSTOMER 3                 /* valeur du type pour envoyer les valeur pour enregistrer un client */
#define TYPE_REC_MESSAGE (3+getpid())   /* valeur du type pour recevoire un message */
#define TYPE_SEND_MESSAGE(pid) (3+pid)  /* valeur du type pour envoyer un message */

#define CONNECTION -1       /* valeur du pid pour une demmande de conection */
#define DISCONECT getpid()  /* valeur du pid pour une demmande de deconection */

#define UPDATE 1            /* temps entre chaque mise a jour */

#define MESSAGE_SIZE 1024   /* taille maximum d'un message */

#include <sys/shm.h>    /* key_t */

/* pour une demande de connection */
typedef struct ask
{
    long type;
    pid_t co;
}ask_t;

/* pour une reponse du coordinateur */
typedef struct rep
{
    long type;
    int ok;
    key_t mem;
    key_t sem;
}rep_t;

/* pour les infos client */
typedef struct customer
{
    long type;
    pid_t pid;
    char id[MAX_ID_SIZE];
    int nbMes;
}customer_t;

/* pour envoyer un message */
typedef struct message
{
    long type;
    char id[MAX_ID_SIZE];
    char msg[MESSAGE_SIZE];
}message_t;

#endif
