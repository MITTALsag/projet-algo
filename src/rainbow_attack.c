#include <rainbow_table.h>
#include <stdbool.h>


/*=======================================================================================================*/


/* Attaque des hashs dans le fichier attackme en utilisant les tables passées en paramètre
   et écriture des résultats dans le fichier result 
*/
void attackme(char** files, int len, char* attackme, char* result) 
{
    // Vérification du nombre de fichiers
    if (len != R) 
    {
        printf("Il doit y avoir %d fichiers de tables passés en paramètre\n", R);
        return;
    }

    // Initialisation de la Rainbow Table et ouverture des fichiers attackme et result
    RainbowTable* rt = init_rainbow_table();
    FILE* fa = fopen(attackme, "r");
    FILE* fr = fopen(result, "w");

    // Vérification des ouvertures
    if (!fa || !fr || !rt) 
    {
        printf("Erreur lors de l'ouverture des fichiers ou de l'initialisation de la Rainbow Table.\n");
        if (fa) fclose(fa);
        if (fr) fclose(fr);
        free_rainbow_table(rt);
        return;
    }

    // Charger les tables dans la Rainbow Table
    for (int i = 0; i < len; i++) 
    {
        Table* t = create_table(files[i]);
        if (!t) 
        {
            printf("Erreur lors de la création de la table à partir du fichier %s.\n", files[i]);
            fclose(fa);
            fclose(fr);
            free_rainbow_table(rt);
            return;
        }
        rainbowtable_insert(rt, t);
    }

    pwhash h;
    Password pwd, temp_pwd;
    char* abc = "abcdefghijklmnopqrstuvwxyz";

    // Initialisation des compteurs
    int total_hashes = 0;
    int found_count = 0;

    // Pour chaque hash dans le fichier attackme
    while (fscanf(fa, "%lX", &h) != EOF)
    {
        total_hashes++;

        // Affichage de la progression tous les 100 hashs
        if (total_hashes % 100 == 0)
            printf("Attaque du hash numéros %d.\n", total_hashes);

        
        // Réinitialiser le flag de trouvaille pour chaque hash
        bool found = false;

        // Pour chaque position de mot de passe possible dans la chaîne
        // On part de la fin de la chaîne vers le début pour optimiser
        for (int start_pos = L - 1; start_pos >= 0 && !found; start_pos--) 
        {
            // Initialiser le hash courant
            pwhash current_hash = h;

            // Générer le mot de passe potentiel à partir du hash courant
            for (int pos = start_pos; pos < L; pos++) 
            {
                // Réduction
                rt->reductions(current_hash, pos, 26, abc, pwd);

                // Hashage sauf si on est à la dernière position    
                if (pos < L - 1) 
                {
                    current_hash = rt->hash(pwd);
                }
            }

            // pwd est maintenant le mot de passe à la fin de la chaîne
            // Vérifier si ce point existe dans notre rainbow table
            char* chain_start = rainbow_find(rt, pwd);

            // Si on a trouvé une chaîne qui finit par pwd
            if (chain_start) 
            {
                strcpy(temp_pwd, chain_start);

                // Recréer la chaîne depuis le début jusqu'à start_pos
                for (int pos = 0; pos <= start_pos; pos++) 
                {
                    // Hash le mot de passe courant
                    pwhash test_hash = rt->hash(temp_pwd);

                    // Vérifier si le hash correspond au hash d'origine
                    if (test_hash == h) 
                    {
                        // Mot de passe trouvé
                        fprintf(fr, "%s\n", temp_pwd);

                        found = true; 
                        found_count++;  // Incrémenter le compteur de réussites
                        break;          // Sortir de la boucle des positions
                    }

                    // Si ce n'est pas le hash recherché
                    // Réduire pour obtenir le prochain mot de passe dans la chaîne
                    rt->reductions(test_hash, pos, 26, abc, temp_pwd);
                }

                // if (!found)  fausse alerte. On continue avec la prochaine position
            }
        }

        // Si le mot de passe n'a pas été trouvé, écrire une ligne vide
        // Car on a testé toutes les positions possibles
        if (!found) 
        {
            fprintf(fr, "\n");  // Ligne vide pour mot de passe non trouvé
        }
    }

    // Afficher le résumé de l'attaque
    printf("Attaque terminée : %d/%d hashs trouvés (%.2f%%)\n",
           found_count, total_hashes, (100.0 * found_count) / total_hashes);

    // Fermeture des fichiers et libération de la mémoire
    fclose(fa);
    fclose(fr);
    free_rainbow_table(rt);
}



int main(int argc, char* argv[])
{
    int nb_files = argc - 1; // Sans le nom du programme

    attackme(argv+1, nb_files-2, argv[argc-2], argv[argc-1]);

    return 0;
}