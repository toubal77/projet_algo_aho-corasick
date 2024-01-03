CC = cc
CFLAGS = -pedantic -Wconversion -Wextra -Werror -Wall -std=c11
LDFLAGS= 

all: genere-texte genere-mots

genere-texte: genere-texte.o
	$(CC) $^ $(LDFLAGS) -o $@

genere-texte.o: genere-texte.c
	$(CC) -c $(CFLAGS) $^ -o $@

genere-mots: genere-mots.o
	$(CC) $^ $(LDFLAGS) -o $@

genere-mots.o: genere-mots.c
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -f *.o genere-mots genere-texte texte3.txt mots3.txt
