#include <rainbow_table.h>



int main(void)
{
    FILE* fr = fopen("result.txt", "r");

    FILE* rr = fopen("crack_perso.txt", "w");

    Password pass0;
    while(fscanf(fr, "%s", pass0) != EOF)
    {
        if (strcmp(pass0, "\n") != 0){
            pwhash ha = target_hash_function(pass0);

            fprintf(rr, "%#010llX\n", ha);
        }
    }


    fclose(rr);
    fclose(fr);
    return 0;
}