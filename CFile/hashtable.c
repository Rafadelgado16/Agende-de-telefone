#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

// Cria uma nova tabela hash com todos os buckets iniciados como NULL
HashTable* create_hashtable() {
    HashTable *table = (HashTable*)malloc(sizeof(HashTable));
    if (!table) return NULL;

    // Aloca espaço para os buckets e zera todos
    table->buckets = (HashNode**)calloc(TABLE_SIZE, sizeof(HashNode*));
    if (!table->buckets) {
        free(table);
        return NULL;
    }

    table->size = TABLE_SIZE;
    return table;
}

// Liberta todos os contactos e nós da tabela hash
void free_hashtable(HashTable *table) {
    if (table) {
        // Percorre todos os buckets
        for (int i = 0; i < table->size; i++) {
            HashNode *current = table->buckets[i];
            // Liberta todos os nós da lista encadeada do bucket
            while (current) {
                HashNode *temp = current;
                current = current->next;
                free_contact(temp->contact); // Liberta o contacto
                free(temp);                 // Liberta o nó
            }
        }
        free(table->buckets); // Liberta array de buckets
        free(table);         // Liberta a estrutura da tabela
    }
}

// Função de hash simples baseada no algoritmo DJB2 (muito comum para strings)
int hash_function(const char *key) {
    unsigned long hash = 5381;
    int c;

    // Processa cada caractere da string chave
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash % TABLE_SIZE;
}

// Insere um novo contacto na tabela hash
void hashtable_insert(HashTable *table, Contact *contact) {
    if (!table || !contact) return;

    // Calcula o índice a partir do nome do contacto
    int index = hash_function(contact->name);
    // Cria novo nó da lista
    HashNode *new_node = (HashNode*)malloc(sizeof(HashNode));
    if (!new_node) return;

    
    new_node->contact = contact;
    // Inserção no início da lista (colisão = encadeamento)
    new_node->next = table->buckets[index];
    table->buckets[index] = new_node;
}

// Procura por um contacto pelo nome
Contact* hashtable_search(HashTable *table, const char *name) {
    if (!table || !name) return NULL;

    int index = hash_function(name);
    HashNode *current = table->buckets[index];

    // Percorre a lista do bucket correspondente
    while (current) {
        if (strcmp(current->contact->name, name) == 0) {
            return current->contact;
        }
        current = current->next;
    }

    return NULL;
}

// Remove um contacto da tabela hash
void hashtable_remove(HashTable *table, const char *name) {
    if (!table || !name) return;

    int index = hash_function(name);
    HashNode *current = table->buckets[index];
    HashNode *prev = NULL;

    // Procura o contacto na lista do bucket
    while (current) {
        if (strcmp(current->contact->name, name) == 0) {
            // Atualiza ponteiros para remover o nó da lista
            if (prev) {
                prev->next = current->next;
            } else {
                table->buckets[index] = current->next;
            }
            free_contact(current->contact);  // Liberta o contacto
            free(current);                  // Liberta o nó
            return;
        }
        prev = current;
        current = current->next;
    }
}

// Imprime todos os contactos da tabela
void hashtable_print(HashTable *table) {
    if (!table) return;

    // Percorre todos os buckets
    for (int i = 0; i < table->size; i++) {
        HashNode *current = table->buckets[i];
        while (current) {
            print_contact(current->contact);  // Usa função definida em contact.c
            current = current->next;
        }
    }
}