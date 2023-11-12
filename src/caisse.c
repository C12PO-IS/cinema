
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <time.h>
#include <stdbool.h>

#include "../include/shm_const.h"

extern int creer_initialiser_semaphore();
extern void attente_aleatoire();
extern int * attacher_segment_memoire();
extern int P();
extern int V();


int main(int argc, char *argv[])
{
    int segMem;
    pid_t pid;  
     int * mem; /* Adresse du segment de mémoire partagée */
    int i;
    int shmid;
    int sem;
    int numero_caisse = atoi(argv[1]);

    int nbr_famille;
    bool close = false;
    int delais = 3;

    srand(time(NULL)^ (getpid()<<16));
    pid = getpid();
    
    #ifdef DEBUG
    printf("\t(fils %d), La caisse numero %d, viens d'ouvrir !\n",pid, numero_caisse);
    #endif
    printf("La caisse numero %d, viens d'ouvrir !\n", numero_caisse);
    
    //recuperation semaphore
    sem = semget(CLE_SEM, 1, 0);
    if(sem == -1){
        fprintf(stderr, "(Fils) Pb recuperation semaphore\n");
        exit(-1);
    }

    /* Recuperation SMP :    */
    shmid = shmget(CLE_SHM,SHM_SIZE,0);
    if (shmid == -1){
        printf("(Fils %d) Pb recuperation SMP\n",pid);
        exit(-1);
    }

     /* Attachement du segment de mémoire partagée */
    mem=attacher_segment_memoire(mem, &shmid);

    /*simuler les clients*/
    while(1){
        attente_aleatoire(delais);
        nbr_famille = rand() % 6;
        /*SEMAPHORE BLOQUANT*/
        P(sem);
        /* Reste-t-il des places libres ? */
        if (*mem == 0) {
        /* No more */
        close =true;
        }
        /* On écrit dans la smp */
        else{
             if (*mem - nbr_famille >=0)
            {
		#ifdef DEBUG
                printf("\t (fils %d)Le(s) client(s) a pris %d place a la caisse numero %d\n",pid,nbr_famille,numero_caisse );
		#endif

                *mem=(*mem - nbr_famille);
		
		#ifdef DEBUG
        	printf("memoire %d\n",*mem);
		#endif

            }else{

		#ifdef DEBUG
                printf("\t (fils %d)Le(s) client(s) sont trop nombreuse(s) %d a la caisse numero  %d\n",pid,nbr_famille,numero_caisse );
		#endif
            };
        }

        /* On protège l'accès à la shm */
        V(sem);
        if(close){
	    #ifdef DEBUG
            printf("\t (fils %d)la caisse numero %d se ferme! \n",pid,numero_caisse);
	    #endif
	    break;
        }
    }
    int time = rand() % 8;
    sleep(time);
  
    #ifdef DEBUG
    printf("\t (fils %d) (temps = %d)", pid,time);
    #endif

    printf("La caisse numero %d est fermé !\n",numero_caisse);

    exit(0);

}
