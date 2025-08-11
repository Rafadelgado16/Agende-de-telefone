#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"

// Cria um novo contacto a partir do nome e número fornecidos
Contact* create_contact(const char *name, const char *phone) {
    // Aloca memória para a estrutura Contact
    Contact *new_contact = (Contact*)malloc(sizeof(Contact));
    if (!new_contact) return NULL;

     // Duplica as strings fornecidas (aloca nova memória para elas)
    new_contact->name = strdup(name);
    new_contact->phone = strdup(phone);

    // Verifica se houve falha na duplicação e limpa, se necessário
    if (!new_contact->name || !new_contact->phone) {
        free_contact(new_contact);
        return NULL;
    }

    return new_contact;
}

// Liberta a memória associada a um contacto
void free_contact(Contact *contact) {
    if (contact) {
        free(contact->name);     // Liberta nome
        free(contact->phone);   // Liberta telefone
        free(contact);         // Liberta a estrutura Contact
    }
}

// Imprime os dados de um contacto formatados
void print_contact(const Contact *contact) {
    if (contact) {
         // Nome é alinhado à esquerda com 20 caracteres; telefone vem depois
        printf("%-20s %s\n", contact->name, contact->phone);
    }
}

// Compara dois contactos com base no nome (ordem alfabética)
int compare_contacts(const Contact *a, const Contact *b) {
    return strcmp(a->name, b->name);
}