#include "io/command_processor.h"
#include "io/output_writer.h"
#include "queries/query_result.h"

#include "queries/query1.h"
#include "queries/query2.h" 
#include "queries/query3.h"
#include "queries/query4.h"
#include "queries/query5.h"
#include "queries/query6.h"

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
    
    char line[256];
    
    // fgets vai ler até encontrar o \n e vai incluir no buffer de line
    // fgets retornar null quando chega no EOF (end of file)
    while (fgets(line, sizeof(line), file) != NULL) {
        
        int query_id;
        char query_type_str[16];
        char delimiter = ';';

        // variaveis auxiliares para argumentos
        char arg_str1[100], arg_str2[100]; 
        int arg_int1;

        QueryResult* result = NULL;

        // remover o '\n' no final da string
        line[strcspn(line, "\r\n")] = '\0';

        // criar caminho do output file
        char output_path[100];
        snprintf(output_path, sizeof(output_path), "resultados/command%d_output.txt", *command_counter);

        // primeiro extraimos o n da query
        if (sscanf(line, "%s", query_type_str) != 1) {
            (*command_counter)++;
            continue; // pular para a próxima iteração do while (ler proxima linha do input)
        }

        // verificar se tem o 'S' após o nº da querie para mudar o delimitador para '='
        if (strchr(query_type_str, 'S') != NULL) delimiter = '='; // se encontrar o char 'S' retorna o ponteiro para ele, se não retorna NULL

        query_id = atoi(query_type_str); // converte "1S" ou "1" para o int 1

        switch(query_id) {

            case 1: // Aeroporto (arg: code)

                if (sscanf(line, "%*s %s", arg_str1) == 1) { // %*s diz para deitar o valor fora (não guardar) visto que guardamos em cima
                    result = execute_query1(get_airports_from_catalog_manager(catalog_manager), arg_str1);
                }
                break;
                
            case 2: // Top N Aeronaves (args: N [manufacturer])
                
                // tentar com fabricante primeiro
                if (sscanf(line, "%*s %d %99s", &arg_int1, arg_str1) == 2) { // sscanf retorna o nº de variagem preenchidas (por isso == 2)
                    result = execute_query2(get_aircrafts_from_catalog_manager(catalog_manager), arg_int1, arg_str1);
                } 

                // tentar sem fabricante
                else if (sscanf(line, "%*s %d", &arg_int1) == 1) {
                    result = execute_query2(get_aircrafts_from_catalog_manager(catalog_manager), arg_int1, NULL);
                }
                break;

            case 3: // Top Aeroporto (args: data_inicio data_fim)

                if (sscanf(line, "%*s %99s %99s", arg_str1, arg_str2) == 2) {
                    result = execute_query3(get_airports_from_catalog_manager(catalog_manager), arg_str1, arg_str2);
                }
                break;
            
            case 4: // Top Passageiro (args: [data_inicio data_fim])

                // com intervalo de datas
                if (sscanf(line, "%*s %s %s", arg_str1, arg_str2) == 2) {
                    //
                }
                // sem argumentos (Top Global)
                else {
                    //
                }
                break;
            
            case 5: // Top N Companhias Aéreas (arg: N)

                if (sscanf(line, "%*s %d", &arg_int1) == 1) {
                    //
                }
                break;
            
            case 6: // Aeroporto Destino por Nacionalidade (arg: nacionalidade)

                if (sscanf(line, "%*s %s", arg_str1) == 1) {
                    //
                }
                break;

            default:
                printf("Query ID %d desconhecido ou input inválido na linha: %s\n", query_id, line);
                break;
        }
        
        write_result(result, output_path, delimiter);
        if (result != NULL) {
            destroy_query_result(result); // libertar a memória da struct de resultados
        } 
        
        (*command_counter)++;
    }
    
    fclose(file);
}