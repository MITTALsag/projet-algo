
#include <rainbow_table.h>


// Génère un mot de passe aléatoire de M lettres minuscules avec terminaison nulle
void generate_random_password(Password password) 
{
    for (int i = 0; i < M; i++) 
    {
        password[i] = 'a' + (rand() % 26);  // Lettre entre 'a' et 'z'
    }
    password[M] = '\0';  // Terminaison de chaîne
}


/* Remplit les fichiers avec des mots de passe */
int fill_files(int len, char* files[])
{
    // Vérification du nombre de fichiers
    if (len != R && len != R+1)
    {
        printf("Il doit y avoir %d ou %d fichier passé en paramètre\n", R, R+1);
        printf("Si il y a %d fichiers, alors les pass0 seront choisit aléatoirement.\n", R);
        printf("Sinon il seront choisit à partir du %dème fichier. (un par ligne et il faut donc %d ligne dans ce fichier).\n", R+1, N*R);
        return 1;
    }
    
    // Initialisation de la Rainbow Table
    RainbowTable* new_R = init_rainbow_table();

    if (!new_R) 
    {
        printf("Échec de l'initialisation de la Rainbow Table.\n");
        return 1;
    }

    FILE* pass_file = NULL;

    int nb_files = len;

    // Si il y a R+1 fichiers, le dernier est le fichier de pass0
    if (len == R+1)
    {
        nb_files--;
        printf("Ouverture du fichier %s pour lire les mots de passe depuis celui-ci.\n", files[len-1]);
        pass_file = fopen(files[len-1], "r");
    }

    // Pour chaque fichier, créer une table et la remplir
    for (int i = 0; i<nb_files; i++)
    {
        // Ouvrir le fichier en écriture (créé ou écrasé)
        FILE* current_file = fopen(files[i], "w");
        
        // Vérification de l'ouverture du fichier
        if (!current_file)
        {
            printf("Échec de l'ouverture du fichier %s pour écriture.\n", files[i]);
            if (pass_file)
                fclose(pass_file);
            free_rainbow_table(new_R);
            return 1;
        }

        // Compteur de lignes écrites
        int nb_lines = 0;
        
        // Créer une nouvelle table
        Table* current_table = init_table();

        printf("Création de la Table %d dans %s.\n", i+1, files[i]);

        // Remplir la table avec des mots de passe
        while (nb_lines != N)
        {
            Password pass0;
            Password passL;

            // Si on a R+1 fichiers, lire le mot de passe depuis le fichier
            // Sinon le générer aléatoirement
            if (len == R)
                generate_random_password(pass0);
            else
            {
                // Si on ne peut pas lire un mot de passe, il n'y en a pas assez dans le fichier
                // On libère tout et on quitte
                if (fscanf(pass_file, "%s", pass0) == EOF)
                {
                    printf("Il n'y a pas assez de mot de passe dans le fichier %s.\n", files[len-1]);
                    fclose(current_file);
                    fclose(pass_file);
                    free_rainbow_table(new_R);
                    free_table(&current_table);
                    return 1;
                }
                pass0[M] = '\0';
            }

            // Calcul de passL
            Password temp_pass;
            strcpy(temp_pass, pass0);
            for (int i = 0; i<L; i++)
            {
                pwhash temp_hash = new_R->hash(temp_pass);
                new_R->reductions(temp_hash, i, 26, "abcdefghijklmnopqrstuvwxyz", temp_pass);
            }
            strcpy(passL, temp_pass);


            /*
            Insertion dans la table si le couple n'existe pas déjà
            Si l'insertion est faite, on écrit dans le fichier et on incrémente le compteur
            Sinon on génère un nouveau mot de passe et on recommence
            */
            int insert_result = table_insert(current_table, pass0, passL);
            if(insert_result == 0)
            {
                nb_lines++;
                fprintf(current_file, "%s %s\n", pass0, passL);
            }
            else if (insert_result != 0 && insert_result != -1)
            {
                printf("Erreur lors de l'insertion du couple %s %s dans la table.\n", pass0, passL);
                fclose(current_file);
                if (pass_file)
                    fclose(pass_file);
                free_rainbow_table(new_R);
                free_table(&current_table);
                return 1;
            }
            // Si insert_result == -1, le couple existe déjà, on ne fait rien et on recommence

        }

        // Fermeture du fichier
        fclose(current_file);

        printf("Table %d créée avec succès dans %s. Taille: %ld, Capacité: %ld, Collisions: %d = %d/%ld = (%.2f%%)\n\n", 
               i+1, files[i], current_table->size, current_table->capacity, current_table->collisions, 
               current_table->collisions, current_table->size, (float)current_table->collisions / current_table->size * 100);

        // Libération de la table courante
        free_table(&current_table);

        // Insertion de la table dans la Rainbow Table mais pas nécessaire ici
        // rainbowtable_insert(new_R, current_table);
    }
    free_rainbow_table(new_R);

    // Fermeture du fichier de pass0 si il y en a un
    if (pass_file)
        fclose(pass_file);

    return 0;
}

int main(int argc, char* argv[])
{
    int nb_files = argc - 1; // Sans le nom du programme

    return fill_files(nb_files, argv+1);

}