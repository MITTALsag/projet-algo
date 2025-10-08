#!/bin/bash

# rainbow_attack.sh - Scripts pour exécuter une attaque
# Usage: ./rainbow_attack.sh <num_tables> <input_hashes> <output_passwords> [table_dir]

if [ $# -lt 3 ]; then
    echo "Usage: $0 <num_tables> <input_hashes> <output_passwords> [table_dir]"
    echo "  num_tables:       Nombre de tables rainbow à utiliser (ex: 10, 15, 20)"
    echo "  input_hashes:     Fichier contenant les hashs à craquer"
    echo "  output_passwords: Fichier où les mots de passe trouvés seront écrits"
    echo "  table_dir:        Répertoire optionnel contenant les tables (par défaut: ./Tables)"
    echo ""
    echo "Exemples:"
    echo "  $0 10 crackme2025_6.txt found_6.txt Va lancer une attaque avec 10 tables dans le répertoire Tables sur le fichier crackme2025_6.txt et écrira les résultats dans found_6.txt"
    echo "  $0 15 crackme2025_6.txt found_6.txt data Va lancer une attaque avec 15 tables dans le répertoire data sur le fichier crackme2025_6.txt et écrira les résultats dans found_6.txt"
    exit 1
fi

NUM_TABLES=$1
INPUT_HASHES=$2
OUTPUT_PASSWORDS=$3
TABLE_DIR="${4:-Tables}"  # Répertoire par défaut ./Tables

# Vérifier que INPUT_HASHES existe
if [ ! -f "$INPUT_HASHES" ]; then
    echo "Erreur : Le fichier de hashs d'entrée '$INPUT_HASHES' est introuvable !"
    exit 1
fi

# Vérifier que le répertoire des tables existe
if [ ! -d "$TABLE_DIR" ]; then
    echo "Erreur : Le répertoire des tables '$TABLE_DIR' est introuvable !"
    exit 1
fi

# Construire la liste des fichiers de table
TABLE_LIST=""
for i in $(seq 1 $NUM_TABLES); do
    TABLE_FILE="${TABLE_DIR}/table${i}.txt"
    if [ ! -f "$TABLE_FILE" ]; then
        echo "Erreur : Le fichier de table '$TABLE_FILE' est introuvable !"
        echo "Assurez-vous que le répertoire '$TABLE_DIR' contient au moins $NUM_TABLES fichiers de table nommés table1.txt, table2.txt, ..., table${NUM_TABLES}.txt"
        exit 1
    fi
    TABLE_LIST="$TABLE_LIST $TABLE_FILE"
done

# Compter le nombre de hashs à craquer
NUM_HASHES=$(wc -l < "$INPUT_HASHES")

echo "Attaque de $NUM_HASHES hashs en utilisant $NUM_TABLES tables"
echo "Tables provenant de : $TABLE_DIR/"
echo "Les résultats seront écrits dans : $OUTPUT_PASSWORDS"
echo ""
echo "Exécution : ./rainbow_attack $TABLE_LIST $INPUT_HASHES $OUTPUT_PASSWORDS"
echo ""

# Lancer l'attaque
time ./exec/rainbow_attack $TABLE_LIST "$INPUT_HASHES" "$OUTPUT_PASSWORDS"

# Vérifier si l'attaque a réussi
if [ -f "$OUTPUT_PASSWORDS" ]; then
    echo ""
    echo "Fin de l'attaque."
    
    # Calculer le taux de réussite
    FOUND=$(grep -c '^[a-z]\{6\}$' "$OUTPUT_PASSWORDS" 2>/dev/null || echo 0)
    SUCCESS_RATE=$(echo "scale=2; $FOUND * 100 / $NUM_HASHES" | bc)
    
    echo "Résulats :"
    echo "  Nombre de hashs: $NUM_HASHES"
    echo "  Mots de passe trouvés: $FOUND"
    echo "  Taux de réussite: $SUCCESS_RATE%"
    echo "  Résultats enregistrés dans: $OUTPUT_PASSWORDS"
else
    echo "Erreur: L'attaque a échoué!"
    exit 1
fi