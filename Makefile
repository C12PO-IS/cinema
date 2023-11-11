# Project: cinema

OBJ  = sem_op.o shm_op.o aleatoire.o
LINKOBJ  = $(OBJ)
BIN  = cinema caisse
CFLAGS = -g #-Wall

all: cinema caisse wash

cinema: $(LINKOBJ) include/shm_const.h src/cinema.c
	$(CC) src/cinema.c $(LINKOBJ) -o cinema $(CFLAGS)

caisse: $(LINKOBJ) include/shm_const.h src/caisse.c
	$(CC) src/caisse.c $(LINKOBJ) -o caisse $(CFLAGS)

shm_op.o: src/shm_op.c include/shm_const.h
	$(CC) -c src/shm_op.c $(CFLAGS)

sem_op.o: src/sem_op.c include/shm_const.h
	$(CC) -c src/sem_op.c $(CFLAGS)

aleatoire.o: src/aleatoire.c
	$(CC) -c src/aleatoire.c $(CFLAGS)

wash:
	rm -f $(OBJ)
	./clean_ipc.sh

clean: 
	rm -f $(OBJ) $(BIN)


