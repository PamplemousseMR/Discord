#define _XOPEN_SOURCE 199309L

#include <stdio.h>	/* printf, perror */
#include <stdlib.h>	/* exit */
#include <sys/msg.h>	/* msgget */
#include <sys/stat.h>	/* S_IRUSR */
#include <errno.h>	/* errno */
#include <sys/types.h>  /* pid_t */
#include <sys/shm.h>    /* Pour shmget, shmctl */
#include <string.h>	/* strcpy */
#include <sys/sem.h>    /* Pour semget, semctl, semop */

#include "../Commun/commun.h"
#include "option.h"
#include "signal.h"

int msg;
int shmid;
customer_t* mem;
int sem;

/* =============== File de messages =============== */

static void initMessage()
{
    if((msg = msgget(MSG_KEY, S_IRUSR | S_IWUSR | IPC_CREAT | IPC_EXCL)) == -1)
    {
        if(errno == EEXIST)
            fprintf(stderr, "[initMessage] file (cle=%d) existante\n", MSG_KEY);
        else
            perror("[initMessage] Erreur lors de la creation de la file");
        exit(EXIT_FAILURE);
    }
    printf("[initMessage] File de messages cree\n");
}

static void clearMessage()
{
    if(msgctl(msg, IPC_RMID, 0) == -1)
        perror("[clearMessage] Erreur lors de la supression de la file ");
    else
        printf("[clearMessage] File de messages supprimee\n");
}

/* =============== Memoire partagée =============== */

static void initMemory()
{
    int i;
    char id[MAX_ID_SIZE] = "DEFAULT";

    if((shmid = shmget(MEM_KEY, sizeof(customer_t)*MAX_CLIENT, S_IRUSR | S_IWUSR | IPC_CREAT | IPC_EXCL)) == -1)
    {
        if(errno == EEXIST)
            fprintf(stderr, "[initMemory] Le segment de memoire partagee (cle=%d) existe deja\n", MEM_KEY);
        else
            perror("[initMemory] Erreur lors de la creation du segment de memoire");
        exit(EXIT_FAILURE);
    }

    if((mem = (customer_t*)shmat(shmid, NULL, 0)) == (void*)-1)
    {
        perror("[initMemory] Erreur lors de l'attachement du segment de memoire partagee");
        exit(EXIT_FAILURE);
    }

    for(i=0; i<MAX_CLIENT; ++i)
    {
        strcpy(mem[i].id,id);
        mem[i].pid = -1;
        mem[i].nbMes = 0;
    }

    printf("[initMemory] Memoire partagee cree\n");
}

static void clearMemory()
{
    if(shmctl(shmid, IPC_RMID, 0) == -1)
        perror("[clearMemory] Erreur lors de la supression du segment de memoire partagee");
    else
        printf("[clearMemory] Memoire partagee supprimee\n");
}

/* =============== Semaphores =============== */

static void initSemaphores()
{
    int i;
    unsigned short init[MAX_CLIENT];
    for(i=0 ; i<MAX_CLIENT ; ++i)
        init[i] = 1;

    /* Creation du tableau de semaphores */
    if((sem = semget(SEM_KEY, MAX_CLIENT, S_IRUSR | S_IWUSR | IPC_CREAT | IPC_EXCL)) == -1)
    {
        if(errno == EEXIST)
            fprintf(stderr, "[initSemaphores] Tableau de semaphores (cle=%d) existant\n", SEM_KEY);
        else
            perror("[initSemaphores] Erreur lors de la creation du tableau de semaphores");
        exit(EXIT_FAILURE);
    }

    /* Initialisation des semaphores */
    if(semctl(sem, 0, SETALL, init) == -1)
    {
        perror("[initSemaphores] Erreur lors de l'initialisation des semaphores");
        exit(EXIT_FAILURE);
    }

    printf("[initSemaphores] Semaphores crees\n");
}

static void clearSemaphores()
{
    if(semctl(sem, 0, IPC_RMID) == -1)
        perror("[clearSemaphores] Erreur lors de la suppresion du tableau de semaphores");
    else
        printf("[initSemaphores] Semaphores supprime\n");
}

/* =============== Fin du programme =============== */

static void clear()
{
    clearMessage();
    clearMemory();
    clearSemaphores();
    _exit(EXIT_SUCCESS);
}

/* =============== Suppression des IPC ===============*/

static void deleteIPC()
{
    printf("[deleteIPC] nettoyage\n");
    /* File */
    if((msg = msgget(MSG_KEY, 0)) == -1)
        perror("[deleteIPC] La file de messages n'existe pas");
    else
    {
        if(msgctl(msg, IPC_RMID, 0) == -1)
            perror("[deleteIPC] Erreur lors de la suppression de la file ");
        else
            printf("[deleteIPC] La file de messages est supprimee\n");
    }

    /* Memoire */
    if((shmid = shmget(MEM_KEY, 0, 0)) == -1)
        perror("[deleteIPC] la memoire partagee n'existe pas");
    else
    {
        if(shmctl(shmid, IPC_RMID, 0) == -1)
            perror("[deleteIPC] Erreur lors de la suppression du segment de memoire partagee");
        else
            printf("[deleteIPC] La memoire partagee est supprimee\n");
    }

    /* semaphores */
    if((sem = semget(SEM_KEY, 0, 0)) == -1)
        perror("[deleteIPC] Le tableau de semaphores n'existe pas");
    else
    {
        if(semctl(sem, 0, IPC_RMID) == -1)
            perror("[deleteIPC] Erreur lors de la suppresion du tableau de semaphores");
        else
            printf("[deleteIPC] Les semaphores sont supprimes\n");
    }
}

/* =============== Attente des clients =============== */

void waitForCustomer()
{
    ask_t ask;
    rep_t rep;
    customer_t cus;
    int i;
    int ok;
    int customer_count = 1;


    while(1)
    {
        /* attente d'une demande */
        if(msgrcv(msg, &ask, sizeof(ask_t) - sizeof(long), TYPE_ASK, 0) == -1)
        {
            perror("[waitForCustomer] Erreur lors de la reception d'une requete");
            _exit(EXIT_FAILURE);
        }
        /* si c'est une demande de connexion */
        if(ask.co == CONNECTION)
        {
            /* envoi des infos */
            if(customer_count < MAX_CLIENT)
            {
                /* établir la connexion */
                rep.type = TYPE_REP;
                rep.ok = 1;
                rep.mem = MEM_KEY;
                rep.sem = SEM_KEY;
                if(msgsnd(msg, &rep, sizeof(rep_t) - sizeof(long), 0) == -1)
                {
                    perror("[waitForCustomer] Erreur lors de l'envoi de la reponse");
                    _exit(EXIT_FAILURE);
                }
                /* réception des infos */
                if(msgrcv(msg, &cus, sizeof(customer_t) - sizeof(long), TYPE_CUSTOMER, 0) == -1)
                {
                    perror("[waitForCustomer] Erreur lors de la reception de la configuration");
                    _exit(EXIT_FAILURE);
                }
                /* ajout dans la memoire partagee */
                strcpy(mem[customer_count].id,cus.id);
                mem[customer_count].pid = cus.pid;
                mem[customer_count].nbMes = 0;
                ++customer_count;
                printf("[waitForCustomer] Le client %s , pid %d, s'est connecte\n",cus.id,cus.pid);
            }
            else
            {
                /* le maximum de client est atteint */
                rep.type = TYPE_REP;
                rep.ok = 0;
                rep.mem = 0;
                rep.sem = 0;
                if(msgsnd(msg, &rep, sizeof(rep_t) - sizeof(long), 0) == -1)
                {
                    perror("[waitForCustomer]  Erreur lors de l'envoi de la reponse de refus");
                    _exit(EXIT_FAILURE);
                }
                printf("[waitForCustomer] Un client a ete refuse\n");
            }
        }
        else
        {
            /* on déconnecte le client */
            ok = 0;
            for(i=0 ; i<MAX_CLIENT ; ++i)
            {
                if(ok)
                {
                    strcpy(mem[i-1].id, mem[i].id);
                    mem[i-1].pid = mem[i].pid;
                    mem[i-1].nbMes = mem[i].nbMes;
                }
                if(mem[i].pid == ask.co)
                    ok = 1;
            }
            if(ok)
            {
                --customer_count;
                printf("[waitForCustomer] le client de pid %d c'est deconnecte\n",ask.co);
            }
            else
                fprintf(stderr,"[waitForCustomer] le client de pid %d n'existe pas mais a demande une deconnexion\n",ask.co);
        }
    }
    _exit(EXIT_SUCCESS);
}

/* =============== Main =============== */

int main(int argc,char* argv[])
{
    /* récuperation des options */
    getOption(argc, argv);
    if(CLEAR)
    {
        deleteIPC();
        return EXIT_SUCCESS;
    }

    /* initialisation des IPC */
    if(atexit(clear) != 0)
    {
        perror("[main] probleme lors de l'enregistrement du netoyage des IPC");
        exit(EXIT_FAILURE);
    }
    initSignal();
    initMessage();
    initMemory();
    initSemaphores();

    /* attente de client */
    waitForCustomer();

    return EXIT_SUCCESS;
}

