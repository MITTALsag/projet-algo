#include <rainbow_table.h>



int main(void)
{
    FILE* f = fopen("mdp.txt", "r");

    FILE* r = fopen("crack_perso.txt", "w");

    Password pass0;
    while(fscanf(f, "%s", pass0) != EOF)
    {
        pwhash ha = target_hash_function(pass0);

        fprintf(r, "%#010llX\n", ha);
    }

    fclose(f);
    fclose(r);
    return 0;
}