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
        char* pass0=NULL;
        int i = L-1;

        while (i>=0 && !found)
        {
            Password passible_passL;

            rainbow->reductions(current_hash, i, 26, "abcdefghijklmnopqrstuvwxyz", passible_passL);
            
            apply(passible_passL, passible_passL, rainbow, i+1, L);

            pass0 = rainbow_find(rainbow, passible_passL);

            found = pass0!=NULL;

            i--;
        }

        Password candidate;
        if (found)
        {
            found = true;

            apply(pass0, candidate, rainbow, 0, i+1);

            fprintf(fresult, "%s", pass0);
        }
        else
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