#ifndef CALLHISTORY_H
#define CALLHISTORY_H

#include "contact.h"
#include <time.h>

// Estrutura que representa uma chamada telefónica
typedef struct {
    Contact *caller;
    Contact *receiver;
    time_t date;
    int duration; // in seconds
} CallRecord;

// Nó de uma lista duplamente ligada, usado para armazenar o histórico
typedef struct CallNode {
    CallRecord call;
    struct CallNode *next;
    struct CallNode *prev;
} CallNode;

// Estrutura principal do histórico de chamadas
typedef struct {
    CallNode *head;
    CallNode *tail;
    CallNode *current;
    int size;
} CallHistory;

// Cria um novo histórico de chamadas vazio
CallHistory* create_callhistory();

// Liberta toda a memória associada ao histórico de chamadas
void free_callhistory(CallHistory *history);

// Adiciona uma nova chamada ao histórico
void add_call(CallHistory *history, CallRecord call);

// Navega pelo histórico de chamadas
CallRecord* navigate_history(CallHistory *history, int direction);

// Imprime as N chamadas mais recentes
void print_recent_calls(CallHistory *history, int n);

#endif