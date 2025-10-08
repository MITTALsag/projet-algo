#include <stdio.h>
#include <rainbow_table.h>



int main(int argc, char* argv[]) 
{
    // Verification des arguments
    if (argc != 3) 
    {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    // Ouvrir le fichier d'entrée
    FILE* input_file = fopen(argv[1], "r");
    if (!input_file) 
    {
        printf("Erreur lors de l'ouverture du fichier d'entrée '%s'\n", argv[1]);
        return 1;
    }
    
    // Ouvrir le fichier de sortie
    FILE* output_file = fopen(argv[2], "w");
    if (!output_file) 
    {
        printf("Erreur lors de la création du fichier de sortie '%s'\n", argv[2]);
        fclose(input_file);
        return 1;
    }
    
    // Lire chaque mot de passe, calculer son hash et l'écrire dans le fichier de sortie
    Password password;

    int count = 0;

    while (fscanf(input_file, "%s", password) != EOF) 
    {
        // Calcul du hash
        pwhash hash = target_hash_function(password);
        
        // Écriture du hash dans le fichier de sortie
        fprintf(output_file, "%016lX\n", (uint64_t)hash);
        count++;
    }

    printf("Mot de passe traités : %d\n", count);
    printf("Hashes écrits dans '%s'\n", argv[2]);

    // Fermer les fichiers
    fclose(input_file);
    fclose(output_file);
    
    return 0;
}