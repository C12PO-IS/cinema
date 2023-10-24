
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "shm_const.h"

extern int creer_initialiser_semaphore();
extern int * attacher_segment_memoire();
extern int P();
extern int V();

extern int creer_segment_memoire();

int main(int argc, char *argv[])
{
    pid_t pid_entree; /* no du processus du processus entree  */
    pid_t pid_sortie; /* no du processus du processus sortie */

    int code_retour_fin_entree;
    int code_retour_fin_sortie;

    int nombre_places_parking; /* Pour écriture dans la shm */
    char * nombre_places_parking_str; /* Pour conversion du semid (int) en chaine */

    int shmid; /* Id du segment de mémoire partagé */
    int semid; /* Id du sémaphore */

    char shmid_str[20]; /* Pour conversion du shmid (int) en chaine */
    char semid_str[20]; /* Pour conversion du semid (int) en chaine */

    int * mem; /* Adresse du segment de mémoire partagée */

    char param_gnome_terminal[80];

    if ( argc != 2 ) {
        fprintf(stderr, "Usage : %s nombre_places\n", argv[0]);
        return(3);
    }

    nombre_places_parking_str=argv[1];
    nombre_places_parking=atoi(nombre_places_parking_str);


 /* Avant de créer les fils :
    * - on crée le semaphore
    * - on crée le segment de mémoire partagé
    * - on s'attache le segment de mémoire partagé
    * - on initialise le noombre de places de parking libres dans la shm
    */

    /* Création sémaphore */
    creer_initialiser_semaphore(CLE_SEM, &semid);

    /* Création segment de mémoire partagé */
    creer_segment_memoire(CLE_SHM, &shmid);

/*
printf("DEBUg : parking : shmid=%d\n", shmid);
*/

    /* Attachement du segment de mémoire partagée */
    mem=attacher_segment_memoire(mem, &shmid);

    /* Pas besoin de sémaphore on est seul :-) */
    *mem=nombre_places_parking;



    /* Conversion des shmid et semid  (int) en chaine pour appel programme externe */
    sprintf(shmid_str, "%d", shmid);
    sprintf(semid_str, "%d", semid);

    /* création du fils entrée */
    pid_entree = fork();

    if (pid_entree == -1) {
        /* Erreur */
        perror("pb fork sur création entrée");
        return(1);
    }

    if (pid_entree == 0) {
        /*
        printf("Je suis le fils entree, je vais faire execl dans 10s shmid_str=%s, semid_str=%s\n", shmid_str, semid_str);
        */
        execl("entree", "entree", shmid_str, semid_str, NULL);
    }

    if (pid_entree >0) {
        /* processus père */

        /* création du fils sortie */
        pid_sortie = fork();

        if (pid_sortie == -1) {
            /* Erreur */
            perror("pb fork sur création sortie");
            return(1);
        }

        if (pid_sortie == 0) {
             execl("sortie", "sortie", shmid_str, semid_str, nombre_places_parking_str, NULL);
        }

        /* processus père */

        printf("Père, on attend 1000s \n");
        sleep(1000);
/*
        waitpid(pid_entree, &code_retour_fin_entree, 0);

        waitpid(pid_sortie, &code_retour_fin_sortie, 0);
*/
        printf("P: processus père fin\n");
        return(0);
    }
}
