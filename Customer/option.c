#include "option.h"

key_t MSG_KEY = (key_t)DEFAULT_MSG_KEY;
char ID[MAX_ID_SIZE] = "default";

static void printOptions()
{
    printf("Options :\n"
           "\th : afficher l'aide\n"
           "\tc : permet de specifier la cle de la file de messages\n"
           "\ti : permet de specifier le string identifiant du client\n"
           "\tdans l'interface, la commande EXIT permet de se deconnecter\n");
}

void getOption(int argc,char* argv[])
{
    int c;
    while((c = getopt(argc, argv, "hc:i:")) != -1)
    {
        switch(c)
        {
        case 'h' :
            printOptions();
            exit(EXIT_SUCCESS);
            break;
        case 'c' :
            MSG_KEY = (key_t)strtol(optarg, NULL, 10);
            if(MSG_KEY <= 0)
            {
                fprintf(stderr,"[getOption] La cle de la file de messages ne doit pas etre <= 0\n");
                exit(EXIT_FAILURE);
            }
            break;
        case 'i' :
            if(strlen(optarg) >= MAX_ID_SIZE)
            {
                fprintf(stderr,"[getOption] L'id a une taille superieure a %d\n",MAX_ID_SIZE);
                exit(EXIT_FAILURE);
            }
            strcpy(ID,optarg);
            break;
        }
    }
}
