#ifndef AVL_H
#define AVL_H

#include "contact.h"  // Inclui definição da estrutura Contact usada nos nós da AVL

// Estrutura de um nó da árvore AVL
typedef struct AVLNode {
    Contact *contact;
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
} AVLNode;

// Estrutura principal da árvore AVL
typedef struct {
    AVLNode *root;  // Raiz da árvore
} AVLTree;

// Cria e inicializa uma árvore AVL vazia
AVLTree* create_avltree();

// Libera toda a memória da árvore AVL
void free_avltree(AVLTree *tree);

// Insere um novo contacto na árvore AVL
void avltree_insert(AVLTree *tree, Contact *contact);

// Percorre a árvore em ordem (esquerda, raiz, direita) e aplica uma função a cada contacto
void avltree_inorder(AVLTree *tree, void (*func)(const Contact*));

#endif