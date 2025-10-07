#include <hash.h>



pwhash target_hash_function (const void *data) {
    uint8_t*p=(uint8_t*)data,*e=p+M;
    uint64_t r=0x14020a57acced8b7,x,h=SEED;
    while(p+8<=e)memcpy(&x,p,8),x*=r,p+=8,x=x<<31|x>>33,h=h*r^x,h=h<<31|h>>33;
    while(p<e)h=h*r^*(p++);
    return(h=h*r+M,h^=h>>31,h*=r,h^=h>>31,h*=r,h^=h>>31,h*=r,h);
}

// fonction de mélange rapide
static inline uint64_t fast_mix(uint64_t x) 
{
    x ^= x >> 33;
    x *= 0xff51afd7ed558ccd;
    x ^= x >> 33;
    return x;
}

// fonction de réduction
void reduction(uint64_t hash, int variation, int allowed_chars_length, char* allowed_chars, char* pass) 
{

    uint64_t state = fast_mix(hash ^ ((uint64_t)variation * 0x9e3779b97f4a7c15));
    
    // Générer un mot de passe de M caractères
    for (int i = 0; i < M; i++) 
    {
        pass[i] = allowed_chars[state % allowed_chars_length];
        state = fast_mix(state + i + variation);
    }
    pass[M] = '\0';
}

