#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "include/contact.h"
#include "include/hashtable.h"
#include "include/avl.h"
#include "include/callhistory.h"

//Função para exibir o menu principal da app
void display_menu() {
    printf("\n=== Agenda Telefónica ===\n");
    printf("1. Adicionar contacto\n");
    printf("2. Editar contacto\n");
    printf("3. Remover contacto\n");
    printf("4. Pesquisar contacto\n");
    printf("5. Listar contactos (ordem alfabética)\n");
    printf("6. Registrar chamada\n");
    printf("7. Ver histórico de chamadas\n");
    printf("8. Navegar no histórico\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
}

// Recolhe os dados submetidos e cria um novo contacto
Contact* input_contact() {
    char name[100], phone[20];
    
    printf("Nome: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';
    
    printf("Telefone: ");
    fgets(phone, sizeof(phone), stdin);
    phone[strcspn(phone, "\n")] = '\0';
    
    return create_contact(name, phone); // cria e guarda o contacto
}

// Recolhe dados de uma chamada e preenche um CallRecord
CallRecord input_call(HashTable *contacts) {
    CallRecord call;
    char phone[20];
    
    // Recolhe o número do chamador
    printf("Número do chamador (ou 'oculto' para número oculto): ");
    fgets(phone, sizeof(phone), stdin);
    phone[strcspn(phone, "\n")] = '\0';
    
    // Trata o chamador oculto ou desconhecido
    if (strcmp(phone, "oculto") == 0) {
        call.caller = NULL;
    } else {
        call.caller = hashtable_search(contacts, phone);
        if (!call.caller) {
            // Criar um contacto temporário para números desconhecidos
            char unknown_name[50];
            sprintf(unknown_name, "Desconhecido (%s)", phone);
            call.caller = create_contact(unknown_name, phone);
        }
    }
    
    // Recolhe o número do destinatário
    printf("Número do destinatário: ");
    fgets(phone, sizeof(phone), stdin);
    phone[strcspn(phone, "\n")] = '\0';
    
    call.receiver = hashtable_search(contacts, phone);
    if (!call.receiver) {
        // Criar um contacto temporário para números desconhecidos
        char unknown_name[50];
        sprintf(unknown_name, "Desconhecido (%s)", phone);
        call.receiver = create_contact(unknown_name, phone);
    }
    
    // Recolhe a duração da chamada
    printf("Duração da chamada (segundos): ");
    scanf("%d", &call.duration);
    while (getchar() != '\n'); // Limpar buffer
    
    call.date = time(NULL); // Regista a data e hora atual
    
    return call;
}

// Função Principal
int main() {

    // Inicia estruturas de dados
    HashTable *contacts = create_hashtable();
    AVLTree *sorted_contacts = create_avltree();
    CallHistory *call_history = create_callhistory();
    
    int option;
    do {
        display_menu(); // Exibe o menu
        scanf("%d", &option);
        while (getchar() != '\n'); // Limpar buffer
        
        switch (option) {
            case 1: { // Adicionar contacto
                Contact *new_contact = input_contact();
                if (new_contact) {
                    hashtable_insert(contacts, new_contact);
                    avltree_insert(sorted_contacts, new_contact);
                    printf("Contacto adicionado com sucesso!\n");
                } else {
                    printf("Erro ao adicionar contacto.\n");
                }
                break;
            }
            
            case 2: { // Editar contacto
                char name[100];
                printf("Nome do contacto a editar: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = '\0';
                
                Contact *contact = hashtable_search(contacts, name);
                if (contact) {
                    Contact *new_data = input_contact();
                    if (new_data) {
                        // Atualizar na tabela hash (remoção e inserção)
                        hashtable_remove(contacts, contact->name);
                        free_contact(contact);
                        
                        // Atualizar na árvore AVL (remoção e inserção)
                        // Nota: Implementação simplificada - na prática seria melhor ter uma função de atualização
                        hashtable_insert(contacts, new_data);
                        avltree_insert(sorted_contacts, new_data);
                        printf("Contacto atualizado com sucesso!\n");
                    }
                } else {
                    printf("Contacto não encontrado.\n");
                }
                break;
            }
            
            case 3: { // Remover contacto
                char name[100];
                printf("Nome do contacto a remover: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = '\0';
                
                Contact *contact = hashtable_search(contacts, name);
                if (contact) {
                    hashtable_remove(contacts, name);
                    // Nota: Remoção da AVL não implementada por simplicidade
                    printf("Contacto removido com sucesso!\n");
                } else {
                    printf("Contacto não encontrado.\n");
                }
                break;
            }
            
            case 4: { // Pesquisar contacto
                char name[100];
                printf("Nome (ou parte) a pesquisar: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = '\0';
                
                // Implementação simplificada - pesquisa exata apenas
                Contact *contact = hashtable_search(contacts, name);
                if (contact) {
                    printf("\nContacto encontrado:\n");
                    print_contact(contact);
                } else {
                    printf("Nenhum contacto encontrado.\n");
                }
                break;
            }
            
            case 5: { // Listar contactos (ordem alfabética)
                printf("\nLista de Contactos (ordem alfabética):\n");
                printf("%-20s %s\n", "Nome", "Telefone");
                printf("-------------------- ------------\n");
                avltree_inorder(sorted_contacts, print_contact);
                break;
            }
            
            case 6: { // Registrar chamada
                CallRecord call = input_call(contacts);
                add_call(call_history, call);
                printf("Chamada registrada no histórico.\n");
                break;
            }
            
            case 7: { // Ver histórico de chamadas
                int n;
                printf("Quantas chamadas recentes deseja ver? ");
                scanf("%d", &n);
                while (getchar() != '\n'); // Limpar buffer
                
                print_recent_calls(call_history, n);
                break;
            }
            
            case 8: { // Navegar no histórico
                printf("Navegar no histórico:\n");
                printf("1. Chamada anterior\n");
                printf("2. Próxima chamada\n");
                printf("Escolha: ");
                int dir;
                scanf("%d", &dir);
                while (getchar() != '\n'); // Limpar buffer
                
                CallRecord *call = navigate_history(call_history, (dir == 2) ? 1 : -1);
                if (call) {
                    char date_str[20];
                    strftime(date_str, sizeof(date_str), "%Y-%m-%d %H:%M:%S", localtime(&call->date));
                    
                    printf("\nChamada:\n");
                    printf("  De:      %s (%s)\n", call->caller ? call->caller->name : "UNKNOWN", 
                                                  call->caller ? call->caller->phone : "N/A");
                    printf("  Para:    %s (%s)\n", call->receiver ? call->receiver->name : "UNKNOWN", 
                                                  call->receiver ? call->receiver->phone : "N/A");
                    printf("  Data:    %s\n", date_str);
                    printf("  Duração: %d segundos\n", call->duration);
                } else {
                    printf("Nenhuma chamada no histórico.\n");
                }
                break;
            }
            
            case 0:
                printf("A sair...\n");
                break;
                
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (option != 0);
    
    // Libertação de memória
    free_callhistory(call_history);
    free_avltree(sorted_contacts);
    free_hashtable(contacts);
    
    return 0;
}