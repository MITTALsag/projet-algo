// CORRECTION :

#ifndef RAINBOW_TABLE_H
#define RAINBOW_TABLE_H

#include "hash.h"


typedef char Password[M+1];


typedef struct Node 
{
    Password pass0;
    Password passL;
    struct Node* next;
} Node;

typedef struct Table 
{
    Node* buckets[N];  // Table de hachage avec chaînage
    size_t size;
    size_t capacity;
} Table;


/* Type pointeur vers fonction de réduction */
typedef void (*ReductionFunc)(pwhash hash, Password result);

/* Type pointeur vers fonction de hachage */
typedef pwhash (*HashFunc)(const Password);

typedef struct RainbowTable 
{
    Table* tables[R];                   // R tables
    ReductionFunc reductions[L];        // L fonctions de réduction différentes
    HashFunc hash[L];                   // L fonctions de hashage différentes
} RainbowTable;

// Prototypes des fonctions
RainbowTable* init_rainbow_table(void);


void free_rainbow_table(RainbowTable* rt);

#endif