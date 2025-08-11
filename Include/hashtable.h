#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "contact.h"

#define TABLE_SIZE 100  // Tamanho fixo da tabela hash (número de buckets)

// Estrutura de um nó da tabela hash (usada em listas encadeadas para colisões)
typedef struct HashNode {
    Contact *contact;
    struct HashNode *next;
} HashNode;

// Estrutura da tabela hash completa
typedef struct {
    HashNode **buckets;
    int size;
} HashTable;

// Cria e inicializa uma nova tabela hash
HashTable* create_hashtable();

// Liberta todos os contactos e memória associada à tabela
void free_hashtable(HashTable *table);

// Função de hash: converte uma string (nome) em um índice para a tabela
int hash_function(const char *key);

// Insere um contacto na tabela hash
void hashtable_insert(HashTable *table, Contact *contact);

// Procura um contacto pelo nome na tabela
Contact* hashtable_search(HashTable *table, const char *name);

// Remove um contacto pelo nome
void hashtable_remove(HashTable *table, const char *name);

// Imprime todos os contactos da tabela
void hashtable_print(HashTable *table);

#endif