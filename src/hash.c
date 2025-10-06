#include <hash.h>



pwhash target_hash_function (const void *data) {
    uint8_t*p=(uint8_t*)data,*e=p+M;
    uint64_t r=0x14020a57acced8b7,x,h=SEED;
    while(p+8<=e)memcpy(&x,p,8),x*=r,p+=8,x=x<<31|x>>33,h=h*r^x,h=h<<31|h>>33;
    while(p<e)h=h*r^*(p++);
    return(h=h*r+M,h^=h>>31,h*=r,h^=h>>31,h*=r,h^=h>>31,h*=r,h);
}

// Adapté de : https://github.com/qsantos/rainbow/blob/master/rtable.c#L239
void reduction(uint64_t hash, int variation, int allowed_chars_length, char* allowed_chars, char* pass)
{
    if (!pass || !allowed_chars || allowed_chars_length <= 0) return;

    uint32_t step = (uint32_t)variation;

    const unsigned char* hash_bytes = (const unsigned char*)&hash;

    for (int i = 0; i < M; i++, step >>= 8)
    {
        unsigned char byte = hash_bytes[i % sizeof(uint64_t)] ^ (unsigned char)step;
        pass[i] = allowed_chars[byte % allowed_chars_length];
    }
    pass[M] = '\0';
}