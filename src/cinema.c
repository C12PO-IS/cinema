
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../include/shm_const.h"

typedef pid_t Tableau_Fils[1000];

extern int creer_initialiser_semaphore();
extern int * attacher_segment_memoire();
extern int P();
extern int V();

extern int creer_segment_memoire();


void fin_fils(int n, Tableau_Fils les_fils, int Signal){
  /* Envoi aux n fils dont le pid est dans le tableau les_fils le signal Signal */
  int i;
  for(i=0;i<n;i++)
    kill(les_fils[i],Signal);
}

void usage(char *nom){
  fprintf(stderr, "Usage : cinema <Nombre de caisses> <Titre d’un film> <Nombre de places pour ce film>\n");
}
int main(int argc, char *argv[])
{
    //pid_t pid_entree; /* no du processus du processus entree  */
    //pid_t pid_sortie; /* no du processus du processus sortie */

    int code_retour_fin_fils;
    int i;
    char param[9];

    int nombre_places_cinema; /* Pour écriture dans la shm */
    char * nombre_places_cinema_str; /* Pour conversion du semid (int) en chaine */
     
    int nombre_caisse;
    char * nom_film;
    int nombre_place;

    int shmid; /* Id du segment de mémoire partagé */
    int semid; /* Id du sémaphore */

    char shmid_str[20]; /* Pour conversion du shmid (int) en chaine */
    char semid_str[20]; /* Pour conversion du semid (int) en chaine */

    int * mem; /* Adresse du segment de mémoire partagée */

    pid_t pid;
    pid_t pid_afficheur;
    Tableau_Fils tab_caisse;

    char param_gnome_terminal[80];

    if ( argc != 4 ) {
        usage(argv[0]);
        return -1;
    }
     
    nombre_caisse = atoi(argv[1]);
    nom_film = argv[2];

    nombre_places_cinema_str = argv[3];
    nombre_places_cinema = atoi(nombre_places_cinema_str);


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

    *mem=nombre_places_cinema;

    /* Conversion des shmid et semid  (int) en chaine pour appel programme externe */
    //sprintf(shmid_str, "%d", shmid);
    //sprintf(semid_str, "%d", semid);

    /* création des fils*/

    for(i=0;i<nombre_caisse;i++){
      pid = fork();
      if (pid == -1){  /* Probleme a la creation du fils*/
           perror("pb fork sur création entrée"); 
           //fin();
        break;
      }
      if (pid==0){
        tab_caisse[i] = pid;
        printf("Cinema - Creation de la caisse numero %d avec pid = %d\n",i+1,getpid());
        sprintf(param, "%d", i + 1);
        execl("./caisse","caisse", param,NULL);
      }
    }
    /* processus père */
    sleep(1);
    printf("Père, on attend 5s \n");
    sleep(5);
    //fin_fils(nombre_caisse,tab_caisse,SIGINT);
     while(1){
      pid = waitpid(-1, &code_retour_fin_fils, WUNTRACED | WCONTINUED);
      if(pid == -1){
          break;
        }
        if(WIFEXITED(code_retour_fin_fils)!= 0){
          printf("La caisse avec le pid %d est mort\n",pid);
        }
     }
      printf("Cinema -  Tous les caisses sont férmé !\n");
     exit(0);
}