CC = cc
CFLAGS = -pedantic -Wconversion -Wextra -Werror -Wall -std=c11
LDFLAGS= 

all: genere-texte genere-mots ac-matrice ac-hachage

genere-texte: genere-texte.o
	$(CC) $^ $(LDFLAGS) -o $@

genere-texte.o: genere-texte.c
	$(CC) -c $(CFLAGS) $^ -o $@

genere-mots: genere-mots.o
	$(CC) $^ $(LDFLAGS) -o $@

genere-mots.o: genere-mots.c
	$(CC) -c $(CFLAGS) $^ -o $@

ac-matrice: main.o ac-matrice.o ahocorasick.o
	$(CC) $^ $(LDFLAGS) -o $@

main.o:  main.c
	$(CC) -c $(CFLAGS) $^ -o $@

ac-matrice.o: ac-matrice.c
	$(CC) -c $(CFLAGS) $^ -o $@

ac-hachage: main.o ac-hachage.o ahocorasick.o
	$(CC) $^ $(LDFLAGS) -o $@

ac-hachage.o: ac-hachage.c
	$(CC) -c $(CFLAGS) $^ -o $@

ahocorasick.o : ahocorasick.c
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -f *.o genere-mots genere-texte res-ac-hachage res-ac-matrice texte3.txt mots3.txt ac-hachage ac-matrice
