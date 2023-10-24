#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/******************************************************************************/
/*
 * Fonctions externes
*/
/******************************************************************************/
extern void attente_aleatoire();
extern int * attacher_segment_memoire();
extern int P();
extern int V();

/******************************************************************************/
/*
 * Fonctions 
*/
/******************************************************************************/

int sortie_voiture(int *mem, int semid, int nombre_places_parking) {                                       

/* On protège l'accès à la shm */
P(semid);

  /* Si le nombre de places dans la shm = nombre de places du parking
 * alors plus de voitures à faire sortir 
 * */
  if (*mem != nombre_places_parking) {
  /* On écrit dans la shm */
  *mem=(*mem + 1);
  printf("\tDans la shm il y a %d places\n", *mem);
  }

/* On protège l'accès à la shm */
V(semid);

return(0);

}



/******************************************************************************/
/*
 * Programme principal
*/
/******************************************************************************/
int main(int argc, char *argv[]) {

unsigned int  delais=4;

int shmid=atoi(argv[1]);
int semid=atoi(argv[2]);
int nb_places=atoi(argv[3]);

int *mem;


/* Attachement du segment de mémoire partagée */
mem=attacher_segment_memoire(mem, &shmid);

while (1) {
  attente_aleatoire(delais);
  printf("\tUne voiture sort\n");
  sortie_voiture(mem, semid, nb_places);
}

return(0);
}


