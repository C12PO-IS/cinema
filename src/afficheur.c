
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

extern int * attacher_segment_memoire();

int main (int argc, char *argv[])
{
    pid_t pid;  
    int * mem; /* Adresse du segment de mémoire partagée */
    int shmid;
    
    pid = getpid();

    /* Recuperation SMP :    */
    shmid = shmget(CLE_SHM,SHM_SIZE,0);
    if (shmid == -1){
        printf("(Fils %d) Pb recuperation SMP\n",pid);
        exit(-1);
    }

     /* Attachement du segment de mémoire partagée */
    mem=attacher_segment_memoire(mem, &shmid);
    
    #ifdef DEBUG
    printf("\t(afficheur %d \n",pid);
    #endif

    printf("\t (Afficheur) Il y a %d billet(s) pour le film %s !\n", *mem, argv[1]);
    
    sleep(3);
    /*afficheur*/
    while(1){
	
	if( *mem > 0){
		printf("Il reste %d place(s) pour le film %s\n", *mem, argv[1]);
	}
		
	if( *mem <= 1){
		printf("Tout les billets ont été vendu !!\n");
	}
	sleep(5);
    }
    exit(1);
}
