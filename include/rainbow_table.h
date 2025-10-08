#ifndef RAINBOW_TABLE_H
#define RAINBOW_TABLE_H

#include "hash.h"

/* Type pour les mots de passe */
typedef char Password[M+1];

/* Type pointeur vers fonction de réduction */
typedef void (*ReductionFunc)(pwhash,int ,int, char*, char*);

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

/* Création de la table à partir d'un fichier */
Table* create_table(char* file);


/*=======================================================================================================*/

/* Type pour stocker plusieurs candidats lors de la recherche */
#define MAX_CANDIDATES R  // Nombre maximum de candidats à tester par passL

typedef struct CandidateList 
{
    Password candidates[MAX_CANDIDATES];  // Liste des pass0 possibles
    int count;                            // Nombre de candidats trouvés
} CandidateList;


/* Libération de la liste de candidats */
void free_candidate_list(CandidateList* list);

/*=======================================================================================================*/

/* Type pour la Rainbow Table */
typedef struct RainbowTable 
{
    Table* tables[R];                // R tables
    int idx;                         // Index de la prochaine table à insérer
    ReductionFunc reductions;        // L fonctions de réduction différentes
    HashFunc hash;                   // L fonctions de hashage différentes
} RainbowTable;

/* Initialisation de la Rainbow Table */
RainbowTable* init_rainbow_table(void);

/* Insertion d'une table à la fin du tableau tables */
int rainbowtable_insert(RainbowTable* rt, Table* table);

/* Libération de la Rainbow Table */
void free_rainbow_table(RainbowTable* rt);

/* Recherche dans la Rainbow Table, retourne tous les candidats possibles */
CandidateList* rainbow_find(RainbowTable* rt, Password passL);


/*=======================================================================================================*/


#endif