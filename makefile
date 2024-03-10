CC= gcc
CFLAGS= -Wall
OBJ= Main.o atr.o Liste.o Levenshtein.o
OBJ2= Main2.o atr.o Liste.o Levenshtein.o
OBJ3= Main3.o atr.o bk.o Levenshtein.o Liste.o

all: correcteur_0 correcteur_1 correcteur_2
	make clean

correcteur_0: $(OBJ) 
	$(CC) -g -o $@ $^ $(CFLAGS)

correcteur_1 : $(OBJ2)
	$(CC) -g -o $@ $^ $(CFLAGS)

correcteur_2 : $(OBJ3)
	$(CC) -g -o $@ $^ $(CFLAGS)

Main.o : Main.c atr.h Liste.h
Main2.o : Main2.c atr.h Liste.h Levenshtein.h 
Main3.o : Main3.c atr.h Liste.h Levenshtein.h bk.h
Levenshtein.o : Levenshtein.c Levenshtein.h Liste.h
Liste.o : Liste.c Liste.h atr.h
atr.o : atr.c atr.h
bk.o : bk.c bk.h Liste.h Levenshtein.h atr.h

%.o : %.c
	$(CC) -c $< $(CFLAGS)

clean : 
	rm -f *.o