#include <hash.h>



pwhash target_hash_function (const void *data) {
    uint8_t*p=(uint8_t*)data,*e=p+M;
    uint64_t r=0x14020a57acced8b7,x,h=SEED;
    while(p+8<=e)memcpy(&x,p,8),x*=r,p+=8,x=x<<31|x>>33,h=h*r^x,h=h<<31|h>>33;
    while(p<e)h=h*r^*(p++);
    return(h=h*r+M,h^=h>>31,h*=r,h^=h>>31,h*=r,h^=h>>31,h*=r,h);
}

// fonction de mélange rapide
//récupérée de MurmurHash3 pour essayer
//de produire des changements importants
// avec efforts minimes tout en étant facile à inverser.
//Source de la méthode : https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp#L81
uint64_t avalanche(uint64_t x)
{
    x ^= x >> 33;
    x *= 0xff51afd7ed558ccd;
    x ^= x >> 33;
    return x;
}

// fonction de réduction
void reduction(uint64_t hash, int variation, int allowed_chars_length, char* allowed_chars, char* pass) 
{
    //On mélange déjà un peu le hash d'origine en avance via l'avalanche
    // auquel on fournit le hash mélangé via xor au nombre d'or * variation pour
    // répartir les changements sur les bits de façon équiloibrée
    uint64_t state = avalanche(hash ^ ((uint64_t)variation * 0x9e3779b97f4a7c15));


    for (int i = 0; i < M; i++) 
    {
        //On récupère un char de l'alphabet avec le modulo de la taille de l'alphabet et l'état actuel
        //du hash altéré
        pass[i] = allowed_chars[state % allowed_chars_length];
        //On altère de nouveau le hash pour la suite du mot de passe
        //Cette fois sans le nombre d'or, uniquement avec la position du char et la variation
        //de la fonction de réduc
        state = avalanche(state + i + variation);
    }

    //Ajoute \0 pour finir la chaine de char
    pass[M] = '\0';
}
