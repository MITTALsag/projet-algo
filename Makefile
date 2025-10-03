CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./include
LDFLAGS = -lm

# Fichiers sources
SRC_COMMON = src/rainbow_table.c src/hash.c
SRC_CREATE = src/rainbow_create.c
SRC_ATTACK = src/rainbow_attack.c
SRC_HASH_MANY = src/hash_many.c

# Fichiers objets
OBJ_COMMON = $(SRC_COMMON:.c=.o)
OBJ_CREATE = $(SRC_CREATE:.c=.o)
OBJ_ATTACK = $(SRC_ATTACK:.c=.o)
OBJ_HASH_MANY = $(SRC_HASH_MANY:.c=.o)

# Cibles principales
all: rainbow_create 

# Liens des exécutables
rainbow_create: $(OBJ_CREATE) $(OBJ_COMMON)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# rainbow_attack: $(OBJ_ATTACK) $(OBJ_COMMON)
# 	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# hash_many: $(OBJ_HASH_MANY) $(OBJ_COMMON)
# 	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Règle générique pour compiler les .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Nettoyage
clean:
	rm -f src/*.o rainbow_create rainbow_attack hash_many

re: clean all

# Nettoyage complet
distclean: clean
	rm -f found_6.txt found_7.txt table*.txt

# Installation (crée les dossiers manquants)
init:
	mkdir -p src include

# Affichage d'aide
help:
	@echo "Cibles disponibles:"
	@echo "  all          - Compile tous les programmes"
	@echo "  rainbow_create - Programme de création des tables"
	@echo "  rainbow_attack - Programme d'attaque des hashs"
	@echo "  hash_many    - Programme de calcul de hashs multiples"
	@echo "  clean        - Supprime les fichiers objets et exécutables"
	@echo "  distclean    - Supprime aussi les fichiers générés"
	@echo "  help         - Affiche cette aide"

# .PHONY: all clean distclean init help