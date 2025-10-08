#!/bin/bash

# rainbow_create.sh - Script pour créer des rainbow tables avec options
# Usage: ./rainbow_create.sh <nb_tables> [output_dir] [pass_file]

# Si aucun argument, afficher l'aide
if [ $# -lt 1 ]; then
    echo "Usage: $0 <nb_tables> [output_dir] [pass_file]"
    echo "  nb_tables: Nombre de rainbow tables à créer (ex: 10, 15, 20) doit être = R défini dans hash.h"
    echo "  output_dir: Répertoire optionnel pour stocker les fichiers de sortie (par défaut: ./Tables)"
    echo "  pass_file:  Fichier optionnel contenant les mots de passe initiaux (un par ligne)"
    echo ""
    echo "Exemples:"
    echo "  $0 10                    # Créer 10 tables avec des mots de passe aléatoires dans ./Tables"
    echo "  $0 20 data passwords.txt # Créer 20 tables dans le répertoire ./data en utilisant passwords.txt"
    exit 1
fi

# Récupérer les arguments
NUM_TABLES=$1
PASS_FILE=""
OUTPUT_DIR="Tables"  # Répertoire par défaut


# Regarder si un répertoire de sortie est fourni
if [ $# -ge 2 ]; then
    OUTPUT_DIR=$2
fi


# Regarder si un fichier de mots de passe est fourni
if [ $# -ge 3 ]; then
    PASS_FILE=$3
    # Vérifier que le fichier de mots de passe existe
    if [ ! -f "$PASS_FILE" ]; then
        echo "Erreur : Le fichier de mots de passe '$PASS_FILE' n'existe pas!"
        exit 1
    fi
fi

# Créer le répertoire de sortie s'il n'existe pas
mkdir -p "$OUTPUT_DIR"

# Vérifier si R dans hash.h correspond au nombre demandé
R_VALUE=$(grep "^#define R " include/hash.h | awk '{print $3}')
if [ "$R_VALUE" != "$NUM_TABLES" ]; then
    echo "Erreur : hash.h définit R=$R_VALUE mais vous avez demandé $NUM_TABLES tables"
    echo "Vous devrez peut-être mettre à jour hash.h avec : #define R $NUM_TABLES"
    exit 1
fi

# Construire la liste des fichiers à passer à rainbow_create
FILE_LIST=""
for i in $(seq 1 $NUM_TABLES); do
    FILE_LIST="$FILE_LIST ${OUTPUT_DIR}/table${i}.txt"
done

# Ajouter le fichier de mots de passe si fourni
if [ -n "$PASS_FILE" ]; then
    FILE_LIST="$FILE_LIST $PASS_FILE"
fi

# Afficher ce qui va être exécuté
echo "Création de $NUM_TABLES rainbow tables dans le répertoire : $OUTPUT_DIR"
if [ -n "$PASS_FILE" ]; then
    echo "Utilisation des mots de passe depuis : $PASS_FILE"
else
    echo "Utilisation de mots de passe générés aléatoirement"
fi
echo ""
echo "Exécution : time ./exec/rainbow_create $FILE_LIST"
echo ""

# Exécuter la commande
time ./exec/rainbow_create $FILE_LIST

# Vérifier si la création a réussi
if [ $? -eq 0 ]; then
    echo ""
    echo "Rainbow tables créées avec succès!"
    echo "Fichiers créés dans : $OUTPUT_DIR/"

    # Afficher les statistiques
    echo ""
    echo "Statistiques des tables:"
    for i in $(seq 1 $NUM_TABLES); do
        FILE="${OUTPUT_DIR}/table${i}.txt"
        if [ -f "$FILE" ]; then
            LINES=$(wc -l < "$FILE")
            UNIQUE=$(awk '{print $2}' "$FILE" | sort | uniq | wc -l)
            echo "Table $i: $LINES entrées, $UNIQUE hachages uniques"
        fi
    done
else
    echo "Erreur : La création des rainbow tables a échoué !"
    exit 1
fi