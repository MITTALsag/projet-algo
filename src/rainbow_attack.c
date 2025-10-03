#include <rainbow_table.h>


/*=======================================================================================================*/











int main(int argc, char* argv[])
{
    if (argc < R+1)
    {   
        
        printf("Utilisation :\n./rainbow_create fich_1.txt fich_2.txt ... fich_%d.txt [pass0.txt]\n", R);
        printf("Ou les fich_i.txt seront remplies des couples pass0 passL. Et si il y a un %deme fichier les pass0 seront pris à partir de ce fichier\n", R+1);
        return 1;
    }

    int nb_files = argc - 1; // Sans le nom du programme

    
    fill_files(nb_files, argv+1);


    return 0;
}