#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define ATTENTE_MAX 7

void attente_aleatoire(unsigned int delais) {

/* Initialisation du désordre */
srandom(time(NULL) % delais);

/* Attente */
sleep((unsigned int)(random() % ATTENTE_MAX));

}
