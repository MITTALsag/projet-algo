#include <rainbow_table.h>


/*=======================================================================================================*/
/* NODE */

/* Initialisation d'un nœud */
Node* init_node(const Password pass0, const Password passL) 
{
    Node* new_node = (Node*)malloc(sizeof(Node));

    if (!new_node) 
    {
        return NULL;  // Échec de l'allocation
    }
    
    strcpy(new_node->pass0, pass0);
    strcpy(new_node->passL, passL);
    new_node->next = NULL;
    
    return new_node;
}

/* Libération d'un nœud */
void free_node(Node* node) 
{
    if (node) 
    {
        free(node);
    }
}


/* Insertion d'un nœud au début de la liste chaînée */
int list_insert(Node** head, Node* new_node) 
{
    if (!new_node || !head) 
    {
        return -3; // Erreur de paramètre
    }
    
    // Si la liste est vide
    if (!*head) 
    {
        *head = new_node;
        new_node->next = NULL;
        return 0; // Pas de collision dans la liste chaînée
    } 
    else 
    {
        // Insertion au début
        new_node->next = *head;
        *head = new_node;
        return 1; // Collision détectée dans la liste chaînée
    }

    return -2; // Erreur inconnue
}


/* Libération de toute une liste chaînée */
void free_list(Node* head) 
{
    Node* current = head;

    // Parcourir et libérer chaque nœud
    while (current) 
    {
        Node* next = current->next;
        free_node(current);
        current = next;
    }
}

/*=======================================================================================================*/
/* TABLE */


/* Initialisation de la table */
Table* init_table(void) 
{
    Table* table = (Table*)malloc(sizeof(Table));
    
    if (!table) 
    {
        return NULL;
    }

    table->size = 0;        // Nombre d'éléments initialisé à 0
    table->capacity = TABLE_SIZE;    // Capacité égale au nombre de chaînes
    table->collisions = 0;  // Initialiser le nombre de collisions à 0

    for (size_t i = 0; i < TABLE_SIZE; i++) 
    {
        table->buckets[i] = NULL;  // Initialiser à NULL
    }
 
    return table;
}

/* Recherche dans la table */
Node* table_find(const Table* table, const Password passL) 
{
    if (!table || !passL) 
    {
        return NULL; // Erreur de paramètres
    }

    // Calculer l'index du bucket
    pwhash index = target_hash_function(passL) % table->capacity;

    // Parcourir la liste chaînée dans le bucket
    Node* current = table->buckets[index];

    // Parcourir la liste chaînée dans le bucket
    while (current)
    {
        // Vérifier si le mot de passe correspond
        if (strcmp(current->passL, passL) == 0) 
        {
            return current;
        }
        current = current->next;
    }
    return NULL;  // Non trouvé
}


/* Insertion dans la table */
int table_insert(Table* table, const Password pass0, const Password passL) 
{

    if (!table || !pass0 || !passL)
    {
        return -3; //Erreur de paramètres
    }

    // Si passL n'est pas deja dans la table
    if (!table_find(table, passL))
    {
        // Incrémenter la taille de la table
        table->size++;

        // Calculer l'index du bucket
        pwhash index = target_hash_function(passL) % table->capacity;
        
        // Créer et insérer le nouveau nœud
        Node* new_node = init_node(pass0, passL);
        if (!new_node) 
        {
            return -2;  // Échec de l'allocation
        }

        // Insérer le nouveau nœud dans la liste chaînée
        if (list_insert(&(table->buckets[index]), new_node) == 1)  // Collision détectée dans la liste chaînée
        {
            table->collisions++; // Incrémenter le compteur de collisions
        }

        return 0; // Succès
    }

    return -1; // Collision detecte entre deux passL identiques
}


/* Libération de la table */
void free_table(Table** table_ptr) 
{   

    if (!table_ptr || !*table_ptr) 
    {
        return;
    }
    
    // Récupérer la table à partir du pointeur
    Table* table = *table_ptr;
    

    // Libérer chaque liste chaînée dans les buckets
    for (size_t i = 0; i < table->capacity; i++) 
    {
        free_list(table->buckets[i]);
    }

    // Libérer la table elle-même
    free(table);

    // Mettre le pointeur à NULL pour éviter les accès invalides
    *table_ptr = NULL; 
}


/* Création de la table à partir d'un fichier */
Table* create_table(char* file)
{
    if (!file) 
    {
        printf("create_table : Erreur : nom de fichier NULL.\n");
        return NULL; // Erreur de paramètre
    }

    // Ouverture du fichier en lecture
    FILE* fi = fopen(file, "r");

    if (!fi) 
    {
        printf("create_table : Erreur : impossible d'ouvrir le fichier %s.\n", file);
        return NULL; // Échec de l'ouverture du fichier
    }

    char pass0[M + 1], passL[M + 1];  

    // Initialisation de la table
    Table* new_table = init_table();

    if (!new_table) 
    {
        fclose(fi);
        return NULL; // Échec de l'initialisation de la table
    }

    // Lecture des mots de passe depuis le fichier et insertion dans la table
    while (fscanf(fi, "%s %s", pass0, passL) == 2) 
    { 
        table_insert(new_table, pass0, passL);
    }

    fclose(fi);

    return new_table;
}


/*=======================================================================================================*/
/* CANDIDATE LIST */

/* Initialisation de la liste de candidats */
CandidateList* init_candidate_list(void)
{
    CandidateList* list = (CandidateList*)malloc(sizeof(CandidateList));
    
    if (!list) 
    {
        return NULL; // Échec de l'allocation
    }
    
    list->count = 0; // Initialiser le compteur à 0
    return list;
}

/* Ajout d'un candidat à la liste */
int list_candidates_add(CandidateList* list, const Password pass0)
{
    if (!list || !pass0) 
    {
        return -1; // Erreur de paramètres
    }  

    if (list->count >= MAX_CANDIDATES) 
    {
        return -2; // Liste pleine
    }

    strcpy(list->candidates[list->count], pass0);
    list->count++;
    return 0; // Succès
}

/* Libération de la liste de candidats */
void free_candidate_list(CandidateList* list)
{
    if (list)
    {
        free(list);
    }
}



/*=======================================================================================================*/
/* Rainbow Table */

/* Initialisation de la Rainbow Table */
RainbowTable* init_rainbow_table(void) 
{
    // Allouer de la mémoire pour la Rainbow Table
    RainbowTable* rt = (RainbowTable*)malloc(sizeof(RainbowTable));

    if (!rt) 
    {
        return NULL; // Échec de l'allocation
    }
    
    // Initialiser les pointeurs de tables à NULL
    for (int i = 0; i < R; i++) 
    {
        rt->tables[i] = NULL;  // Pas encore créées
    }
    
    // Initialiser les fonctions de réduction et de hachage
    rt->reductions = reduction;  
    rt->hash = target_hash_function; 

    // Initialiser l'index de la prochaine table à insérer
    rt->idx = 0;
    return rt;
}

/* Insertion d'une table à la fin du tableau tables */
int rainbowtable_insert(RainbowTable* rt, Table* table)
{
    if (!rt || !table) 
    {
        return -1;  // Erreur de paramètres
    }
    
    // Vérifier si l'index est déjà occupé
    if (rt->tables[rt->idx])  //  != NULL
    {
        return -2;  // Index déjà utilisé
    }
    
    // Insérer la table et incrémenter l'index
    rt->tables[rt->idx] = table;
    rt->idx++;

    return 0;
}

/* Libération de la Rainbow Table */
void free_rainbow_table(RainbowTable* rt) 
{
    if (!rt) 
    {
        return; // Rien à libérer
    }
    
    // Libérer seulement les tables qui ont été créées de 0 a idx-1
    for (int i = 0; i < rt->idx; i++) 
    {
        free_table(&(rt->tables[i]));
    }
    
    // Libérer la Rainbow Table elle-même
    free(rt);
}


/* Recherche dans la Rainbow Table, retourne tous les candidats possibles */
CandidateList* rainbow_find(RainbowTable* rt, Password passL)
{
    if (!rt || !passL) 
    {
        return NULL;
    }

    CandidateList* list_candidates = init_candidate_list();
    if (!list_candidates)
    {
        return NULL; // Échec de l'allocation
    }

    // parcourt toutes les tables pour collecter tous les candidats
    for (int i = 0; i < rt->idx && list_candidates->count < MAX_CANDIDATES; i++)
    {
        Table* current_table = rt->tables[i];


        Node* node = table_find(current_table, passL);

        if (node)
        {
            list_candidates_add(list_candidates, node->pass0);
        }

        // pwhash index = target_hash_function(passL) % current_table->capacity;
        // Node* current = current_table->buckets[index];
        
        // // parcourt toute la liste chaînée du bucket
        // while (current && list->count < MAX_CANDIDATES)
        // {
        //     if (strcmp(current->passL, passL) == 0) 
        //     {
        //         strcpy(list->candidates[list->count], current->pass0);
        //         list->count++;
        //     }
        //     current = current->next;
        // }
    }

    return list_candidates;
}
