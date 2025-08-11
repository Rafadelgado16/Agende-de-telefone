#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "callhistory.h"

// Cria e inicia a estrutura de histórico de chamadas
CallHistory* create_callhistory() {
    CallHistory *history = (CallHistory*)malloc(sizeof(CallHistory));
    if (!history) return NULL;

    history->head = NULL; // Início da lista
    history->tail = NULL; // Fim da lista
    history->current = NULL; // Posição atual para navegação
    history->size = 0; // Tamanho da lista
    return history;
}

// Liberta toda a memória alocada para o histórico
void free_callhistory(CallHistory *history) {
    if (!history) return;

    CallNode *current = history->head;
    while (current) {
        CallNode *temp = current;
        current = current->next;
        free(temp); // Liberta cada nó da lista
    }
    free(history); // Liberta a estrutura principal
}

// Adiciona uma nova chamada ao início da lista (mais recente primeiro)
void add_call(CallHistory *history, CallRecord call) {
    if (!history) return;

     // Cria um novo nó de chamada
    CallNode *new_node = (CallNode*)malloc(sizeof(CallNode));
    if (!new_node) return;

    new_node->call = call;
    new_node->next = history->head;
    new_node->prev = NULL;

    // Atualiza ponteiros se a lista não estiver vazia
    if (history->head) {
        history->head->prev = new_node;
    } else {
        history->tail = new_node;
    }

    // Atualiza o início da lista e a posição atual
    history->head = new_node;
    history->current = new_node;
    history->size++;
}


// Navega pelo histórico de chamadas
// direction > 0: próximo; direction < 0: anterior
CallRecord* navigate_history(CallHistory *history, int direction) {
    if (!history || !history->current) return NULL;

    CallNode *new_current = NULL;
    if (direction > 0) { // Avançar na lista
        new_current = history->current->next;
        if (!new_current) new_current = history->head;
    } else { // Retroceder na lista
        new_current = history->current->prev;
        if (!new_current) new_current = history->tail;
    }

    history->current = new_current;
    return new_current ? &(new_current->call) : NULL;
}

// Imprime as N chamadas mais recentes
void print_recent_calls(CallHistory *history, int n) {
    if (!history || n <= 0) return;

    CallNode *current = history->head;
    int count = 0;

     // Percorre a lista do mais recente até atingir n chamadas ou o fim
    while (current && count < n) {
        // Formata data e hora da chamada
        char date_str[20];
        strftime(date_str, sizeof(date_str), "%Y-%m-%d %H:%M:%S", localtime(&current->call.date));

        // Exibe os dados da chamada
        printf("Chamada %d:\n", count + 1);
        printf("  De:      %s (%s)\n", current->call.caller ? current->call.caller->name : "UNKNOWN", 
                                      current->call.caller ? current->call.caller->phone : "N/A");
        printf("  Para:    %s (%s)\n", current->call.receiver ? current->call.receiver->name : "UNKNOWN", 
                                      current->call.receiver ? current->call.receiver->phone : "N/A");
        printf("  Data:    %s\n", date_str);
        printf("  Duração: %d segundos\n\n", current->call.duration);

        current = current->next;
        count++;
    }
}