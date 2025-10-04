
#include <rainbow_table.h>


// Génère un mot de passe aléatoire de M lettres minuscules
void generate_random_password(Password password) 
{
    for (int i = 0; i < M; i++) 
    {
        password[i] = 'a' + (rand() % 26);  // Lettre entre 'a' et 'z'
    }
    password[M] = '\0';  // Terminaison de chaîne
}


void fill_files(int len, char* files[])
{
    if (len != R && len != R+1)
    {
        printf("Il doit y avoir %d ou %d fichier passé en paramètre\n", R, R+1);
        printf("Si il y a %d fichiers, alors les pass0 seront choisit aléatoirement.\n", R);
        printf("Sinon il seront choisit à partir du %dème fichier. (un par ligne et il faut donc %d ligne dans ce fichier).\n", R+1, N*R);
        return;
    }
    
    RainbowTable* new_R = init_rainbow_table();
    Table* current_table = init_table();

    FILE* pass_file = NULL;

    if (len == R+1)
        pass_file = fopen(files[len-1], "r");

    for (int i = 0; i<len; i++)
    {
        FILE* current_file = fopen(files[i], "w");
        
        int nb_lines = 0;
        while (nb_lines != N)
        {
            Password pass0;
            Password passL;

            if (len == R)
                generate_random_password(pass0);
            else
            {
                fscanf(current_file, "%s", pass0);
                pass0[M] = '\0';
            }

            apply(pass0, passL, new_R, 0, L);

            if(table_insert(current_table, pass0, passL) != -1)
            {
                nb_lines++;
                fprintf(current_file, "%s %s\n", pass0, passL);
            }

        }

        fclose(current_file);
        free_table(&current_table);

        // Si décommenté il faut décommenté les deux lignes
        // rainbowtable_insert(new_R, current_table);
        // current_table = init_table();
    }
    free_rainbow_table(new_R);

    if (pass_file)
        fclose(pass_file);
}

int main(int argc, char* argv[])
{
    if (argc < R+1)
    {   
        printf("Utilisation :\n./rainbow_create fich_1.txt fich_2.txt ... fich_R.txt [pass0.txt]\n");
        printf("Ou les fich_i.txt seront remplies des couples pass0 passL. Et si il y a un %deme fichier les pass0 seront pris à partir de ce fichier\n", R+1);
        return 1;
    }

    int nb_files = argc - 1; // Sans le nom du programme

    
    fill_files(nb_files, argv+1);


    return 0;
}