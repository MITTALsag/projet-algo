#ifndef RAINBOW_TABLE_H
#define RAINBOW_TABLE_H

#include "hash.h"

/* Type pour les mots de passe */
typedef char Password[M+1];

/* Type pointeur vers fonction de réduction */
typedef void (*ReductionFunc)(pwhash,int ,int, Password, Password);

/* Type pointeur vers fonction de hachage */
typedef pwhash (*HashFunc)(const void *);


/*=======================================================================================================*/

/* Type pour les noeuds de la table */
typedef struct Node 
{
    Password pass0;
    Password passL;
    struct Node* next;
} Node;


/*=======================================================================================================*/

/* Type pour les tables de hachage */
typedef struct Table 
{
    Node* buckets[N];  // Tableau de pointeurs vers les listes chaînées
    size_t size;       // Nombre d'éléments dans la table
    size_t capacity;   // Capacité maximale de la table
} Table;


/* Initialisation de la table */
Table* init_table(void);

/* Recherche dans la table */
void free_table(Table** table_ptr);

/* Insertion d'un élément dans la table si l'élément n'existe pas déjà */
int table_insert(Table* table, const Password pass0, const Password passL);

Table* create_table(char* file);


/*=======================================================================================================*/

/* Type pour la Rainbow Table */
typedef struct RainbowTable 
{
    Table* tables[R];                   // R tables
    int idx;
    // ReductionFunc reductions[L];        // L fonctions de réduction différentes
    ReductionFunc reductions;            // L fonctions de réduction différentes
    HashFunc hash;                   // L fonctions de hashage différentes
} RainbowTable;

/* Initialisation de la Rainbow Table */
RainbowTable* init_rainbow_table(void);


/* Applique la chaîne de hachage et réduction i fois*/
void apply(const Password pass0, Password result, const RainbowTable* rt, int i);

/* Insertion d'une table à un index spécifique */
int rainbowtable_insert(RainbowTable* rt, Table* table);

/* Libération de la Rainbow Table */
void free_rainbow_table(RainbowTable* rt);

char* rainbow_find(RainbowTable* rt, Password passL);


/*=======================================================================================================*/


#endif