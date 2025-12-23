#define _POSIX_C_SOURCE 199309L // para clock_gettime
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>
#include <unistd.h>
#include "tests/main_tests.h"
#include "io/parser.h"
#include "io/command_processor.h"

#define NUM_QUERIES 3

struct discrepancy {
    int query;
    int command_id;
    int line_error;
    char file_name[256];
};

struct query_stats {
    int total_tests;
    int passed_tests;
    double total_time_ms;
};

// função que da load dos datasets e retorna o tempo em ms
int get_load_dataset_time (const char* csv_files_path, CatalogManager* catalog_manager) {

    struct timespec start, end;
    double elapsed;

    clock_gettime(CLOCK_REALTIME, &start);
    
    load_datasets(csv_files_path, catalog_manager);
    
    clock_gettime(CLOCK_REALTIME, &end);
    
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    return (int)(elapsed); // retornar em milissegundos
}

// função para criar ficheiros agrupados por query e contar comandos
void create_query_files(const char* input_path, int *query_counts) {
    FILE* input = fopen(input_path, "r");
    if (!input) {
        printf("Erro ao abrir ficheiro de input: %s\n", input_path);
        return;
    }
    
    // abrir todos os ficheiros de output
    FILE* query_files[NUM_QUERIES+1]; // +1 para indexar de 1 a NUM_QUERIES
    char filename[50];
    
    for (int i = 1; i <= NUM_QUERIES; i++) {
        snprintf(filename, sizeof(filename), "q%d_commands.txt", i);
        query_files[i] = fopen(filename, "w");
        query_counts[i] = 0;
    }
    
    char line[100];
    while (fgets(line, sizeof(line), input)) {
        line[strcspn(line, "\r\n")] = '\0';
        
        if (strlen(line) > 0) {
            int q;
            char first_token[16];
            if (sscanf(line, "%15s", first_token) == 1) {
                q = atoi(first_token);
                
                if (q >= 1 && q <= NUM_QUERIES) {
                    if (query_files[q]) {
                        fprintf(query_files[q], "%s\n", line);
                        query_counts[q]++;
                    }
                }
            }
        }
    }
    
    // fechar todos os ficheiros
    fclose(input);
    for (int i = 1; i <= NUM_QUERIES; i++) {
        if (query_files[i]) fclose(query_files[i]);
    }
}

// Retorna 1 se ficheiros iguais, 0 se diferentes e guarda a linha da 1ª diferença
int compare_files(const char* actual_file, const char* expected_file, int *line_error) {
    FILE *f1 = fopen(actual_file, "r");
    FILE *f2 = fopen(expected_file, "r");
    if (!f1 || !f2) {
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        return 0;
    }

    char line1[1024], line2[1024];
    int line = 1;

    while (1) {
        char *res1 = fgets(line1, sizeof(line1), f1);
        char *res2 = fgets(line2, sizeof(line2), f2);

        // se chegamos ao fim de ambos, tudo ok
        if (!res1 && !res2) break;

        // se actual terminou mas expected tem apenas uma linha vazia, ignora (no resultado-esperado tem uma linha extra vazia)
        if (!res1 && res2) {
            // remover \r\n
            line2[strcspn(line2, "\r\n")] = 0;
            if (strlen(line2) == 0) break; // linha vazia extra, ignora
            *line_error = line;
            fclose(f1);
            fclose(f2);
            return 0;
        }

        // se expected terminou mas actual não, diferença real
        if (!res2 && res1) {
            *line_error = line;
            fclose(f1);
            fclose(f2);
            return 0;
        }

        // remover \r\n
        line1[strcspn(line1, "\r\n")] = 0;
        line2[strcspn(line2, "\r\n")] = 0;

        if (strcmp(line1, line2) != 0) {
            *line_error = line;
            fclose(f1);
            fclose(f2);
            return 0;
        }

        line++;
    }

    fclose(f1);
    fclose(f2);
    return 1;
}

// executar grupos de queries e medir tempos
void execute_query_groups(const char* csv_files_path, int *query_counts, QueryStats *stats, CatalogManager* catalog_manager) {
    (void) csv_files_path;
    
    int command_counter = 1;

    for (int i = 1; i <= NUM_QUERIES; i++) {
        if (query_counts[i] > 0) {
            char command_file[50];
            snprintf(command_file, sizeof(command_file), "q%d_commands.txt", i);
            
            // Start time
            struct timespec start, end;
            double elapsed;
            clock_gettime(CLOCK_REALTIME, &start);
            
            // Execute work
            process_commands(command_file, catalog_manager, &command_counter);
            
            // End time
            clock_gettime(CLOCK_REALTIME, &end);
            
            // Elapsed time
            elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
            stats[i].total_time_ms = elapsed * 1000;
            stats[i].total_tests = query_counts[i];
        }
    }
}

void compare_all_outputs(const char* expected_output_path, int *query_counts, QueryStats *stats, Discrepancy *discrepancies, int *discrepancy_count) {

    int current_command = 1;
    for (int q = 1; q <= NUM_QUERIES; q++) {
        for (int i = 0; i < query_counts[q]; i++) {
            char actual[256], expected[256];
            snprintf(actual, sizeof(actual), "resultados/command%d_output.txt", current_command);
            snprintf(expected, sizeof(expected), "%s/command%d_output.txt", expected_output_path, current_command);

            int line_error = -1;
            if (compare_files(actual, expected, &line_error)) {
                stats[q].passed_tests++;
            } else if (*discrepancy_count < 500) {
                Discrepancy *d = &discrepancies[*discrepancy_count];
                d->query = q;
                d->command_id = current_command;
                d->line_error = line_error;
                snprintf(d->file_name, sizeof(d->file_name), "resultados/command%d_output.txt", current_command);
                (*discrepancy_count)++;
            }

            current_command++;
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <csv_files_path> <input_path> <expected_output_path>\n", argv[0]);
        return 1;
    }

    const char* csv_files_path = argv[1];
    const char* input_path = argv[2];
    const char* expected_output_path = argv[3];

    CatalogManager* catalog_manager = catalog_manager_create(); // criar o catalog

    // carregar datasets e medir tempo ( em segundos )
    int load_dataset_time = get_load_dataset_time(csv_files_path, catalog_manager);

    // inicializar estatísticas (1-index: indices 1..NUM_QUERIES)
    QueryStats stats[NUM_QUERIES + 1];
    for (int i = 1; i <= NUM_QUERIES; i++) {
        stats[i].total_tests = 0;
        stats[i].passed_tests = 0;
        stats[i].total_time_ms = 0.0;
    }
    
    // criar ficheiros agrupados por query e contar comandos
    int query_counts[NUM_QUERIES + 1]; // +1 para 1-index
    create_query_files(input_path, query_counts);

    // executar cada grupo de queries e medir tempos
    execute_query_groups(csv_files_path, query_counts, stats, catalog_manager);

    Discrepancy discrepancies[500];
    int discrepancy_count = 0;

    // comparar resultados

    compare_all_outputs(expected_output_path, query_counts, stats, discrepancies, &discrepancy_count);

    catalog_manager_destroy(catalog_manager);

    // memória final
    struct rusage r_usage;
    getrusage(RUSAGE_SELF, &r_usage);
    double memory_used_mb = r_usage.ru_maxrss / 1024.0;

    // estatísticas por query
    for (int i = 1; i <= NUM_QUERIES; i++) {
        if (stats[i].total_tests > 0) {
            
        printf("Q%d: %d de %d testes ok!\n", i, stats[i].passed_tests, stats[i].total_tests);

        // imprimir discrepâncias desta query
        for (int d = 0; d < discrepancy_count; d++) {
            if (discrepancies[d].query == i) {
                printf("    Discrepância no input %d: linha %d de \"%s\"\n", discrepancies[d].command_id, discrepancies[d].line_error, discrepancies[d].file_name);
            }
        }
    }
}
    printf("Memória utilizada: %.0f MB\n", memory_used_mb);

    // tempos de execução
    printf("Tempos de execução por query:\n");
    double total_query_time_seconds = 0.0;
    for (int i = 1; i <= NUM_QUERIES; i++) {
        if (stats[i].total_tests > 0) {
            printf("    Q%d: %.1f ms\n", i, stats[i].total_time_ms);
            total_query_time_seconds += (stats[i].total_time_ms / 1000.0);
        }
    }

    printf("Tempo total: %.1f s\n", total_query_time_seconds + load_dataset_time);

    // limpar ficheiros temporários
    for (int i = 1; i <= NUM_QUERIES; i++) {
        char temp_file[50];
        snprintf(temp_file, sizeof(temp_file), "q%d_commands.txt", i);
        remove(temp_file);
    }
    
    return 0;
}