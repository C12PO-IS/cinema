
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

#include "../include/shm_const.h"

extern int creer_initialiser_semaphore();
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

    srand(time(NULL)^ (getpid()<<16));
    pid = getpid();
    printf("\t(Fils %d), La caisse numero %d, viens d'ouvrir !\n",pid, numero_caisse);
    //recuperation semaphore
    sem = semget(CLE_SEM, 1, 0);
    if(sem == -1){
        fprintf(stderr, "(Fils) Pb recuperation semaphore\n");
        exit(-1);
    }
     //nb_serveurs = semctl(sem, 1, GETVAL, NULL);

    /* Recuperation SMP :    */
    shmid = shmget(CLE_SHM,SHM_SIZE,0);
    if (shmid == -1){
        printf("(Fils %d) Pb recuperation SMP\n",pid);
        exit(-1);
    }

     /* Attachement du segment de mémoire partagée */
    mem=attacher_segment_memoire(mem, &shmid);
    int time = rand() % 8;
    sleep(time);
    printf("\t (fils %d) (temps = %d)La caisse numero %d est fermé !\n",pid,time,numero_caisse);

    exit(0);

}
