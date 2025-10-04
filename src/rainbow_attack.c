#include <rainbow_table.h>
#include <stdbool.h>


/*=======================================================================================================*/


void attackme(char* files[], int len, char* attackme, char* result)
{
    RainbowTable* rainbow = init_rainbow_table();

    FILE* attack = fopen(attackme, "r");
    FILE* fresult = fopen(result, "w");

    pwhash current_hash;
    Password current_pass;

    for (int idx = 0 ; idx<len ; idx++)
    {
        Table* new_table = create_table(files[idx]);
        rainbowtable_insert(rainbow, new_table);
    }


    while(fscanf(attack, "%llX", &current_hash) != 0)
    {
        bool found = false; 
        int i = 0;

        while (i<L && !found)
        {
            rainbow->reductions(current_hash, L-i-1, 26, "abcdefghijklmnopqrstuvwxyz", current_pass);
            apply()
            for (int j = 1; j<=i; j++)
            {
                current_hash = rainbow->hash(current_pass);
                rainbow->reductions(current_hash, L-j, 26, "abcdefghijklmnopqrstuvwxyz", current_pass);
            }

            char* pass0 = rainbow_find(rainbow, current_pass);

            if (pass0)
            {
                found = true;

                for(int k = i+2; k<=L; k++)
                {
                    pwhash new_hash = rainbow->hash(pass0);
                    rainbow->reductions(new_hash, L-k, 26, "abcdefghijklmnopqrstuvwxyz", pass0);
                }
                fprintf(fresult, "%s", pass0);
            }

            i++;
        }

        if (!found)
            fprintf(fresult, "\n");
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