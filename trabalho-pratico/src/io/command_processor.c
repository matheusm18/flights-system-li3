#include "io/command_processor.h"
#include "queries/query1.h"
#include "queries/query2.h" 
#include "queries/query3.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void process_commands(const char* commands_file, CatalogManager* catalog_manager, int* command_counter) {
    
    FILE* file = fopen(commands_file, "r");
    if (!file) {
        perror("Erro ao abrir ficheiro de comandos");
        return;
    }
    
    char line[100];
    
    // fgets vai ler até encontrar o \n e vai incluir no buffer de line
    // fgets retornar null quando chega no EOF (end of file)
    while (fgets(line, sizeof(line), file) != NULL) {
        int query;
        char airport_code[10];
        char manufacturer[100];
        int N;
        char start_date[20], end_date[20];


        // remover o '\n' no final da string
        line[strcspn(line, "\r\n")] = '\0';

        // criar caminho do output file
        char output_path[100];
        snprintf(output_path, sizeof(output_path), "resultados/command%d_output.txt", *command_counter);

        // primeiro extraimos o n da query
        if (sscanf(line, "%d", &query) != 1) {
            (*command_counter)++;
            continue; // pular para a próxima iteração do while (ler proxima linha do input)
        }

        switch(query) {

            case 1:

                if (sscanf(line, "%d %s", &query, airport_code) == 2) {
                    execute_query1(get_airports_from_catalog_manager(catalog_manager), airport_code, output_path);
                }
                break;
                
            case 2:
                
                // tentar com fabricante primeiro
                if (sscanf(line, "%d %d %99s", &query, &N, manufacturer) == 3) {
                    execute_query2(get_flights_from_catalog_manager(catalog_manager), get_aircrafts_from_catalog_manager(catalog_manager), N, manufacturer, output_path);
                } 

                // tentar sem fabricante
                else if (sscanf(line, "%d %d", &query, &N) == 2) {
                    execute_query2(get_flights_from_catalog_manager(catalog_manager), get_aircrafts_from_catalog_manager(catalog_manager), N, NULL, output_path);
                }
                break;

            case 3:

                if (sscanf(line, "%d %19s %19s", &query, start_date, end_date) == 3) {
                    execute_query3(get_flights_from_catalog_manager(catalog_manager), get_airports_from_catalog_manager(catalog_manager), start_date, end_date, output_path);
                }
                break;

            default:
                printf("Input inválido");
                break;
        }
        
        (*command_counter)++;
    }
    
    fclose(file);
}