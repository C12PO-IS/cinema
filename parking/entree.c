#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

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

bool entree_voiture(int *mem, int semid) {                                       
bool place_attribuee=false;

/* On protège l'accès à la shm */
P(semid);

/* Reste-t-il des places libres ? */
if (*mem == 0) {
  /* No more */
  printf("Dans la shm il y a %d places\n", *mem);
}
else {
  /* On écrit dans la shm */
  *mem=(*mem - 1);
  printf("Dans la shm il y a %d places\n", *mem);
  place_attribuee=true;
}

/* On protège l'accès à la shm */
V(semid);

return (place_attribuee);
}


/******************************************************************************/
/*
 * Programme principal
*/
/******************************************************************************/
int main(int argc, char *argv[]) {

unsigned int  delais=3;

int shmid=atoi(argv[1]);
int semid=atoi(argv[2]);

int *mem;

/*
printf("Je suis %s, shmid=%d, semid=%d\n", argv[0], shmid, semid);
*/

/* Attachement du segment de mémoire partagée */
mem=attacher_segment_memoire(mem, &shmid);

while (1) {
  attente_aleatoire(delais);
  printf("Une voiture se présente\n");
  while (entree_voiture(mem, semid) == false) {
    sleep(1);  
  }
}

return(0);
}
