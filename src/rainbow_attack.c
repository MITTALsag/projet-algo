#include <rainbow_table.h>
#include <stdbool.h>


/*=======================================================================================================*/


void attackme(char* files[], int len, char* attackme, char* result)
{
    RainbowTable* rainbow = init_rainbow_table();

    FILE* attack = fopen(attackme, "r");
    FILE* fresult = fopen(result, "w");

    for (int idx = 0 ; idx<len ; idx++)
    {
        Table* new_table = create_table(files[idx]);
        rainbowtable_insert(rainbow, new_table);
    }

    pwhash current_hash;

    while(fscanf(attack, "%llX", &current_hash) != EOF)
    {
        bool found = false;
        char* pass0 = NULL;
        int position = -1;

        for (int i = L-1; i >= 0 && !found; i--)
        {
            Password possible_passL;
            pwhash temp_hash = current_hash;

            rainbow->reductions(temp_hash, i, 26, "abcdefghijklmnopqrstuvwxyz", possible_passL);
            apply(possible_passL, possible_passL, rainbow, i+1, L);

            pass0 = rainbow_find(rainbow, possible_passL);

            if (pass0 != NULL)
            {
                found = true;
                position = i;
            }
        }

        bool password_found = false;

        if (found)
        {
            Password candidate;
            strcpy(candidate, pass0);

            for (int j = 0; j <= position && !password_found; j++)
            {
                pwhash hash_temp = rainbow->hash(candidate);

                if (hash_temp == current_hash)
                {
                    fprintf(fresult, "%s\n", candidate);
                    password_found = true;
                }
                else
                {
                    rainbow->reductions(hash_temp, j, 26, "abcdefghijklmnopqrstuvwxyz", candidate);
                }
            }

            if (!password_found)
            {
                pwhash final_hash = rainbow->hash(candidate);
                if (final_hash == current_hash)
                {
                    fprintf(fresult, "%s\n", candidate);
                    password_found = true;
                }
            }
        }

        if (!password_found)
        {
            fprintf(fresult, "\n");
        }
    }

    fclose(attack);
    fclose(fresult);
    free_rainbow_table(rainbow);
}




int main(int argc, char* argv[])
{
    int nb_files = argc - 1; // Sans le nom du programme

    if (nb_files < R+2)
    {   
        printf("Utilisation :\n./rainbow_attack fich_1.txt fich_2.txt ... fich_%d.txt attackme.txt result.txt\n", R);
        printf("Ou les fich_i.txt sont les tables, attackme.txt le fichier des hashs à casser, et result.txt le fichier de sortie\n");
        return 1;
    }

    
    attackme(argv+1, nb_files-2, argv[argc-2], argv[argc-1]);

    return 0;
}