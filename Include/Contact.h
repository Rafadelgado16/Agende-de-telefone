#ifndef CONTACT_H
#define CONTACT_H

// Estrutura que representa um contacto telefónico
typedef struct {
    char *name;
    char *phone;
} Contact;

// Cria um novo contacto com os dados fornecidos (nome e telefone)
Contact* create_contact(const char *name, const char *phone);

// Liberta toda a memória associada a um contacto (nome, telefone e estrutura)
void free_contact(Contact *contact);

// Imprime o nome e telefone de um contacto em formato amigável
void print_contact(const Contact *contact);

// Compara dois contactos com base no nome (ordem alfabética)
// Retorna <0 se a < b, 0 se iguais, >0 se a > b
int compare_contacts(const Contact *a, const Contact *b);

#endif