#include "io/command_processor.h"
#include "queries/query1.h"
#include "queries/query2.h" 
#include "queries/query3.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void process_commands(const char* commands_file, AirportCatalog* airport_catalog, FlightCatalog* flight_catalog, AircraftCatalog* aircraft_catalog) {
    
    FILE* file = fopen(commands_file, "r");
    if (!file) {
        perror("Erro ao abrir ficheiro de comandos");
        return;
    }
    
    char line[100];
    int command_counter = 1; // contador para saber qual o numero do comando
    
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

        if (sscanf(line, "%d %d %s", &query, &N, manufacturer) == 3 && query == 2) {
            char output_path[100];
            snprintf(output_path, sizeof(output_path), "resultados/command%d_output.txt", command_counter);
            execute_query2(flight_catalog, aircraft_catalog, N, manufacturer, output_path);
            printf("Executada Query %d: %d %s -> %s\n", command_counter, N, manufacturer, output_path);
        }
        else if (sscanf(line, "%s %s", start_date, end_date) == 2) {
                char output_path[100];
                snprintf(output_path, sizeof(output_path), "resultados/command%d_output.txt", command_counter);
                
                execute_query3(flight_catalog, airport_catalog, start_date, end_date, output_path);
                printf("Executada Query %d: %s -> %s\n", command_counter,airport_code ,output_path);
        }  
        else if (sscanf(line, "%d %d", &query, &N) == 2) {
                char output_path[100];
                snprintf(output_path, sizeof(output_path), "resultados/command%d_output.txt", command_counter);
                
                if (query == 2) {
                    execute_query2(flight_catalog,aircraft_catalog, N, NULL, output_path);
                    printf("Executada Query %d: %d -> %s\n", command_counter, N, output_path);
                }
        }      
        else if (sscanf(line, "%d %s", &query, airport_code) == 2) {
                char output_path[100];
                snprintf(output_path, sizeof(output_path), "resultados/command%d_output.txt", command_counter);
                
                if (query == 1) {
                    execute_query1(airport_catalog, airport_code, output_path);
                    printf("Executada Query %d: %s -> %s\n", command_counter, airport_code, output_path);
                }
        }
        
        command_counter++;
    }
    
    fclose(file);
}