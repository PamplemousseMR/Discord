#include "option.h"

key_t MSG_KEY = (key_t)DEFAULT_MSG_KEY;
key_t SEM_KEY = (key_t)DEFAULT_SEM_KEY;
key_t MEM_KEY = (key_t)DEFAULT_MEM_KEY;
int CLEAR = 0;

static void printOptions()
{
	printf("Options :\n"
		   "\th : afficher l'aide\n"
                   "\tc : permet de specifier la cle de la file de messages\n"
                   "\ts : permet de specifier la cle du tableau de semaphores\n"
                   "\tm : permet de specifier la cle du segment de memoire partagee\n"
                   "\td : permet de specifier de nettoyer les IPC en cas de probleme lors d'une precedente creation\n");
}

void getOption(int _argc,char* _argv[])
{
	int c;
    while((c = getopt(_argc, _argv, "hc:s:m:d")) != -1)
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
			case 's' :
				SEM_KEY = (key_t)strtol(optarg, NULL, 10);
				if(SEM_KEY <= 0)
				{
					fprintf(stderr,"[getOption] La cle du tableau de semaphores ne doit pas etre <= 0\n");
					exit(EXIT_FAILURE);
				}
			break;
			case 'm' :
				MEM_KEY = (key_t)strtol(optarg, NULL, 10);
				if(MEM_KEY <= 0)
				{
                                        fprintf(stderr,"[getOption] La cle du segment de memoire partagee ne doit pas etre <= 0\n");
					exit(EXIT_FAILURE);
				}
			break;
			case 'd' :
				CLEAR = 1;
			break;
			case '?' :
				fprintf(stderr,"[getOption] Option non reconnue\n");
				exit(EXIT_FAILURE);
			break;
		}
	}
}
