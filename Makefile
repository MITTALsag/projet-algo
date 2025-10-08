CC = clang
# CFLAGS = -O0 -g -Wall -Wextra -Werror -std=c99 -I./include
CFLAGS = -Wall -Wextra -Werror -std=c99 -I./include -O3 -march=native
LDFLAGS = -lm

# Dossiers
OBJDIR = object
SRCDIR = src
BINDIR = exec

# Fichiers sources
SRC_COMMON = $(SRCDIR)/rainbow_table.c $(SRCDIR)/hash.c
SRC_CREATE = $(SRCDIR)/rainbow_create.c
SRC_ATTACK = $(SRCDIR)/rainbow_attack.c
SRC_HASH_MANY = $(SRCDIR)/hash_many.c

# Fichiers objets (dans object/)
OBJ_COMMON = $(SRC_COMMON:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
OBJ_CREATE = $(SRC_CREATE:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
OBJ_ATTACK = $(SRC_ATTACK:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
OBJ_HASH_MANY = $(SRC_HASH_MANY:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Cibles principales
all: $(BINDIR)/rainbow_create $(BINDIR)/rainbow_attack $(BINDIR)/hash_many

# Liens des exécutables
$(BINDIR)/rainbow_create: $(OBJ_CREATE) $(OBJ_COMMON)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/rainbow_attack: $(OBJ_ATTACK) $(OBJ_COMMON)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/hash_many: $(OBJ_HASH_MANY) $(OBJ_COMMON)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compilation des .c en .o dans le dossier object/
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<


# Scripts
scripts:
	@if [ -f rainbow_create.sh ]; then chmod +x rainbow_create.sh; fi
	@if [ -f rainbow_attack.sh ]; then chmod +x rainbow_attack.sh; fi


# Nettoyage
clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)/*

re: clean all

distclean: clean
	rm -f found_6.txt found_7.txt Tables*/table*.txt files*/*.txt

init:
	mkdir -p $(SRCDIR) include $(OBJDIR) $(BINDIR)

help:
	@echo "Cibles disponibles :"
	@echo "  all            - Compile tous les programmes"
	@echo "  rainbow_create - Programme de création des tables"
	@echo "  rainbow_attack - Programme d’attaque des hashs"
	@echo "  hash_many      - Programme de calcul de hashs multiples"
	@echo "  clean          - Supprime les fichiers objets et exécutables"
	@echo "  distclean      - Supprime aussi les fichiers générés"
	@echo "  init           - Crée les dossiers nécessaires"
