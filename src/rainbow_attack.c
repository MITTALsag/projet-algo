#include <rainbow_table.h>
#include <stdbool.h>


/*=======================================================================================================*/


/* Attaque des hashs dans le fichier attackme en utilisant les tables passées en paramètre
   et écriture des résultats dans le fichier result 
*/
int attackme(char** files, int len, char* attackme, char* result) 
{
    if (len != R) 
    {
        printf("Il doit y avoir %d fichiers de tables passés en paramètre\n", R);
        return -1;
    }

    // initialisation 
    RainbowTable* rt = init_rainbow_table();
    FILE* fa = fopen(attackme, "r");
    FILE* fr = fopen(result, "w");

    if (!fa || !fr || !rt) 
    {
        printf("Erreur lors de l'ouverture des fichiers ou de l'initialisation de la Rainbow Table.\n");
        if (fa) fclose(fa);
        if (fr) fclose(fr);
        free_rainbow_table(rt);
        return -1;
    }

    // on charge les tables dans la Rainbow Table
    for (int i = 0; i < len; i++) 
    {
        Table* t = create_table(files[i]);
        if (!t) 
        {
            printf("Erreur lors de la création de la table à partir du fichier %s.\n", files[i]);
            fclose(fa);
            fclose(fr);
            free_rainbow_table(rt);
            return -1;
        }
        rainbowtable_insert(rt, t);
    }

    pwhash h;
    Password pwd, temp_pwd;
    char* abc = "abcdefghijklmnopqrstuvwxyz";

    int total_hashes = 0;
    int found_count = 0;

    // Pour chaque hash dans le fichier attackme
    while (fscanf(fa, "%lX", &h) != EOF)
    {
        total_hashes++;

        // Affichage de la progression
        if (total_hashes % 100 == 0)
            printf("Attaque du hash numéros %d.\n", total_hashes);

        
        // réinitialise le flag de trouvaille pour chaque hash
        bool found = false;

        // pour chaque position dans la chaîne, de la fin vers le début
        for (int start_pos = L - 1; start_pos >= 0 && !found; start_pos--) 
        {
            // initialise le hash courant
            pwhash current_hash = h;

            // on prolonge le hash jusqu'à la fin de la chaîne pour obtenir passL
            for (int pos = start_pos; pos < L; pos++) 
            {
                // alternance réduction/hash pour prolonger la chaîne
                rt->reductions(current_hash, pos, 26, abc, pwd);
                if (pos < L - 1) 
                {
                    current_hash = rt->hash(pwd);
                }
            }

            // pwd est maintenant le mot de passe à la fin de la chaîne
            // on vérifie si ce point existe dans notre rainbow table
            // on récupère TOUS les candidats possibles (gère les collisions entre différentes tables)
            CandidateList* candidates = rainbow_find(rt, pwd);

            if (candidates && candidates->count > 0) 
            {
                // on yteste CHAQUE candidat (car plusieurs chaînes peuvent avoir le même passL)
                for (int c = 0; c < candidates->count && !found; c++) 
                {
                    strcpy(temp_pwd, candidates->candidates[c]);

                    // recrée la chaîne depuis le début jusqu'à start_pos
                    for (int pos = 0; pos <= start_pos; pos++) 
                    {
                        // hash le mdp courant
                        pwhash test_hash = rt->hash(temp_pwd);

                        // on vérifie si le hash du candidat correspond au hash d'origine
                        if (test_hash == h) 
                        {
                            // mdp trouvé
                            fprintf(fr, "%s\n", temp_pwd);

                            found = true; 
                            found_count++;  
                            break;          
                        }

                        // si ce n'est pas le hash recherché et qu'on n'est pas à la fin
                        // on réduit pour obtenir le prochain mdp dans la chaîne
                        if (pos < start_pos) {
                            rt->reductions(test_hash, pos, 26, abc, temp_pwd);
                        }
                    }
                }
            }
            free_candidate_list(candidates);
            candidates = NULL;
        }

        if (!found) 
        {
            fprintf(fr, "\n");  // hash non trouvé
        }
    }

    // affiche le résumé de l'attaque
    printf("Attaque terminée : %d/%d hashs trouvés (%.2f%%)\n",
           found_count, total_hashes, (100.0 * found_count) / total_hashes);

    fclose(fa);
    fclose(fr);
    free_rainbow_table(rt);

    return 0;
}



int main(int argc, char* argv[])
{
    int nb_files = argc - 1; // Sans le nom du programme

    return attackme(argv+1, nb_files-2, argv[argc-2], argv[argc-1]);

}