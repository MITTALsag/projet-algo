// #include <hash.h>
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
    RainbowTable* new_R = init_rainbow_table();
    Table* current_table;

    for (int i = 0; i<len; i++)
    {
        FILE* current_file = fopen(files[i], "r");
        
        int nb_lines = 0;
        while (nb_lines != N)
        {
            current_table = init_table();
            Password pass0;
            Password passL;
            generate_random_password(current_pass);

            apply(current_pass, passL, R);

            if(table_insert(current_table, pass0, passL) != -1)
            {
                nb_lines++;
                fprintf(current_file, "%s %s\n", pass0, passL);
            }

        }
        rainbowtable_insert(new_R, current_table, i);
        fclose(current_file);
    }

}

int main(int argc, char* argv[])
{
    if (argc < R+1)
    {   
        printf("Utilisation :\n./rainbow_create fich_1.txt fich_2.txt ... fich_%d.txt [pass0.txt]\n", R);
        printf("Ou les fich_i.txt seront remplies des couples pass0 passL. Et si il y a un %deme fichier les pass0 seront pris à partir de ce fichier\n", R+1);
        return 1;
    }

    int nb_files = argc - 1; // Sans le nom du programme

    // Si il y a exactement R fichiers
    if (nb_files == R)
        fill_files(nb_files, argv+1);
    else
        fill_files_password(argv+1)


    return 0;
}