#include "interactive/interactive_mode.h"
#include "io/command_processor.h"
#include "io/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void start_interactive_ui(CatalogManager* manager) {
    char dataset_path[512];
    
    printf("Introduza o caminho dos ficheiros de dados: ");
    if (!fgets(dataset_path, sizeof(dataset_path), stdin)) return;
    
    dataset_path[strcspn(dataset_path, "\n")] = '\0';
    if (strlen(dataset_path) == 0) strcpy(dataset_path, "dataset-fase-2/sem_erros/");

    printf("A carregar dados de '%s'.\n", dataset_path);
    load_datasets(dataset_path, manager); 
    printf("Dataset carregado com sucesso!\n");

    run_menu_loop(manager);
}

void run_menu_loop(CatalogManager* manager) {
    char line[1024];
    while (1) {
        printf("\nQue query deseja executar? (ex: '5 10' ou '1S LIS', ou 'sair'): ");
        if (!fgets(line, sizeof(line), stdin)) break;

        // Limpar o \n para o strcmp e o sscanf funcionarem bem
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "sair") == 0) break;

        if (strlen(line) > 0) {
            execute_single_line(line, manager, 0, 1);
        }
    }
}