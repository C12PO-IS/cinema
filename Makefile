# Project: parking

OBJ  = sem_op.o shm_op.o aleatoire.o
LINKOBJ  = $(OBJ)
BIN  = parking entree sortie
CFLAGS = -g #-Wall

all: parking entree sortie

parking: $(LINKOBJ) shm_const.h parking.c
	$(CC) parking.c $(LINKOBJ) -o parking $(CFLAGS)

sortie: $(LINKOBJ) shm_const.h sortie.c
	$(CC) sortie.c $(LINKOBJ) -o sortie $(CFLAGS)

entree: $(LINKOBJ) shm_const.h entree.c
	$(CC) entree.c $(LINKOBJ) -o entree $(CFLAGS)

shm_op.o: shm_op.c shm_const.h
	$(CC) -c shm_op.c $(CFLAGS)

sem_op.o: sem_op.c shm_const.h
	$(CC) -c sem_op.c $(CFLAGS)

aleatoire.o: aleatoire.c
	$(CC) -c aleatoire.c $(CFLAGS)

clean: 
	rm -f $(OBJ) $(BIN)


