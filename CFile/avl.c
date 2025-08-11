#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "avl.h"

// Função auxiliar para obter o maior valor entre dois inteiros
static int max(int a, int b) {
    return (a > b) ? a : b;
}

// Função auxiliar para obter a altura de um nó (0 se for NULL)
static int height(AVLNode *node) {
    if (!node) return 0;
    return node->height;
}

// Cria um novo nó AVL com o contacto fornecido
static AVLNode* create_avlnode(Contact *contact) {
    AVLNode *node = (AVLNode*)malloc(sizeof(AVLNode));
    if (!node) return NULL;

    node->contact = contact; // Associa o contacto ao nó
    node->left = NULL;
    node->right = NULL;
    node->height = 1;  // Altura inicial de um novo nó é 1
    return node;
}

// Rotação simples à direita (para reequilibrar a árvore)
static AVLNode* right_rotate(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    // Realiza rotação
    x->right = y;
    y->left = T2;

    // Atualiza alturas dos nós
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;


    // Novo topo da subárvore
    return x;
}

// Rotação simples à esquerda
static AVLNode* left_rotate(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    // Realiza rotação
    y->left = x;
    x->right = T2;

    // Atualiza alturas
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// Calcula o fator de balanceamento do nó
static int get_balance(AVLNode *node) {
    if (!node) return 0;
    return height(node->left) - height(node->right);
}

// Insere um contacto recursivamente na subárvore e aplica balanceamentos
static AVLNode* avlnode_insert(AVLNode *node, Contact *contact) {
    if (!node) return create_avlnode(contact);

     // Insere recursivamente na esquerda ou direita, conforme a ordem alfabética
    if (compare_contacts(contact, node->contact) < 0) {
        node->left = avlnode_insert(node->left, contact);
    } else {
        node->right = avlnode_insert(node->right, contact);
    }

    // Atualiza a altura do nó atual
    node->height = 1 + max(height(node->left), height(node->right));

    // Obtém o fator de balanceamento
    int balance = get_balance(node);

    // Casos de desbalanceamento:

    // Left Left Case
    if (balance > 1 && compare_contacts(contact, node->left->contact) < 0) {
        return right_rotate(node);
    }

    // Right Right Case
    if (balance < -1 && compare_contacts(contact, node->right->contact) > 0) {
        return left_rotate(node);
    }

    // Left Right Case
    if (balance > 1 && compare_contacts(contact, node->left->contact) > 0) {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }

    // Right Left Case
    if (balance < -1 && compare_contacts(contact, node->right->contact) < 0) {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }

    return node;
}

// Liberta a memória de todos os nós da árvore (em pós-ordem)
static void free_avlnode(AVLNode *node) {
    if (!node) return;
    free_avlnode(node->left);
    free_avlnode(node->right);
    free(node); // Liberta o nó atual
}

// Percorre a árvore em ordem (esquerda, raiz, direita) e aplica uma função
static void inorder_traversal(AVLNode *node, void (*func)(const Contact*)) {
    if (!node) return;
    inorder_traversal(node->left, func);
    func(node->contact); // Executa a função no contacto do nó atual
    inorder_traversal(node->right, func);
}

// Cria e inicia uma nova árvore AVL
AVLTree* create_avltree() {
    AVLTree *tree = (AVLTree*)malloc(sizeof(AVLTree));
    if (!tree) return NULL;
    tree->root = NULL;
    return tree;
}

// Liberta todos os nós e depois a própria estrutura da árvore
void free_avltree(AVLTree *tree) {
    if (!tree) return;
    free_avlnode(tree->root);
    free(tree);
}

// Insere um novo contacto na árvore AVL
void avltree_insert(AVLTree *tree, Contact *contact) {
    if (!tree || !contact) return;
    tree->root = avlnode_insert(tree->root, contact);
}

// Percorre e imprime a árvore em ordem (alfabeticamente)
void avltree_inorder(AVLTree *tree, void (*func)(const Contact*)) {
    if (!tree || !func) return;
    inorder_traversal(tree->root, func);
}