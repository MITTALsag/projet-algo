#include <rainbow_table.h>


/*=======================================================================================================*/

// Initialisation d'un nœud
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

// Libération d'un nœud
void free_node(Node* node) 
{
    if (node) 
    {
        free(node);
    }
}


// Insertion d'un nœud au début d'une liste chaînée
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
    } 
    else 
    {
        // Insertion au début
        new_node->next = *head;
        *head = new_node;
    }

    return 0; // Succès
}


// Libération de toute une liste chaînée
void free_list(Node* head) 
{
    Node* current = head;

    while (current) 
    {
        Node* next = current->next;
        free_node(current);
        current = next;
    }
}

/*=======================================================================================================*/



// Initialisation de la table
Table* init_table(void) 
{
    Table* table = (Table*)malloc(sizeof(Table));
    
    if (!table) 
    {
        return NULL;
    }

    for (size_t i = 0; i < N; i++) 
    {
        table->buckets[i] = NULL;  // Initialiser à NULL
    }

    table->size = 0;
    table->capacity = N;
    
    return table;
}

// Recherche dans la table
Node* table_find(const Table* table, const Password passL) 
{
    pwhash index = target_hash_function(passL) % table->capacity;

    Node* current = table->buckets[index];
    
    while (current)
    {
        if (strcmp(current->passL, passL) == 0) 
        {
            return current;
        }
        current = current->next;
    }
    return NULL;  // Non trouvé
}


// Insertion dans la table
int table_insert(Table* table, const Password pass0, const Password passL) 
{
    if (!table || !pass0 || !passL)
    {
        return -3; //Erreur de paramètres
    }

    // Si passL n'est pas deja dans la table
    if (!table_find(table, passL))
    {
        pwhash index = target_hash_function(passL) % table->capacity;
        
        // Créer et insérer le nouveau nœud
        Node* new_node = init_node(pass0, passL);
        if (!new_node) 
        {
            return -2;  // Échec de l'allocation
        }
        
        return list_insert(&(table->buckets[index]), new_node);

    }

    return -1; // Collision detecte
}


// Libération de la table
void free_table(Table** table_ptr) 
{
    if (!table_ptr || !*table_ptr) 
    {
        return;
    }
    
    Table* table = *table_ptr;
    

    for (size_t i = 0; i < N; i++) 
    {
        free_list(table->buckets[i]);
    }

    
    free(table);
    *table_ptr = NULL; 
}

// Affichage de la table
void print_table(const Table* table) 
{
    printf("Table (%lu/%u éléments):\n", table->size, N);

    for (size_t i = 0; i < N; i++) 
    {
        if (table->buckets[i]) 
        {
            printf("  Bucket[%zu]: ", i);
            Node* current = table->buckets[i];
            while (current) 
            {
                printf("(%s -> %s) ", current->pass0, current->passL);
                current = current->next;
            }
            printf("\n");
        }
    }
}


Table* create_table(char* file)
{
    FILE* fi = fopen(file, "r");
    char pass0[M + 1], passL[M + 1];  

    Table* new_table = init_table();

    while (fscanf(fi, "%s %s", pass0, passL) == 2) 
    { 
        table_insert(new_table, pass0, passL);
    }
    fclose(fi);

    return new_table;
}



/*=======================================================================================================*/


// Initialisation de la Rainbow Table
RainbowTable* init_rainbow_table(void) 
{
    RainbowTable* rt = (RainbowTable*)malloc(sizeof(RainbowTable));
    if (!rt) 
    {
        return NULL;
    }
    
    // Initialiser les pointeurs de tables à NULL
    for (int i = 0; i < R; i++) 
    {
        rt->tables[i] = NULL;  // Pas encore créées
    }
    
    // Initialiser les fonctions de réduction et de hachage
    for (int i = 0; i < L; i++) 
    {

        rt->reductions = reduction;  
        rt->hash = target_hash_function;       
    }
    rt->idx = 0;
    return rt;
}

// Insertion d'une table à un index spécifique
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
    
    rt->tables[rt->idx] = table;
    rt->idx++;
    return 0;  // Succès
}

// Libération de la Rainbow Table (version corrigée)
void free_rainbow_table(RainbowTable* rt) 
{
    if (!rt) 
    {
        return;
    }
    
    // Libérer seulement les tables qui ont été créées
    for (int i = 0; i < R; i++) 
    {
        if (rt->tables[i])  //  != NULL
        {
            free_table(&(rt->tables[i]));
        }
    }
    
    free(rt);
}

void apply(const Password pass0, Password result, const RainbowTable* rt, int i, int k)
{
    Password current_pass;
    strcpy(current_pass, pass0);

    for (int j = i; j <= k; j++)
    {
        // Vérifier que les fonctions sont initialisées
        if (rt->hash == NULL || rt->reductions == NULL) 
        {
            printf("la fonction de hashage ou de reduction n'est pas dans la rainbowtable.\n");
            EXIT_FAILURE;
        }
        
        pwhash hash_value = rt->hash(current_pass);
        rt->reductions(hash_value, j, 26, "abcdefghijklmnopqrstuvwxyz", current_pass);
    }
    
    strcpy(result, current_pass);
}


char* rainbow_find(RainbowTable* rt, Password passL)
{
    Node* candidate;
    for (int i = 0; i<rt->idx; i++)
    {
        Table* current_table = rt->tables[i];

        candidate = table_find(current_table, passL);

        if (candidate)
        {
            return candidate->pass0;
        }

    }

    return NULL;
}
