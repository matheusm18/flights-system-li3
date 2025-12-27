#include "io/parser.h"
#include "validation/validate_entities.h"
#include "catalog/catalog_manager.h"
#include "catalog/airport_catalog.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

void parse_csv_line(char *line, char *fields[], int max_fields, int *num_fields) {
    int i = 0;
    char *ptr = line;

    while (*ptr && i < max_fields) {
        // SEMPRE começa com aspas
        if (*ptr != '"') {
            break;
        }
        
        ptr++; // pula a primeira aspas
        fields[i++] = ptr; // guarda o início do campo no array de campos
        
        // procura a aspas de fecho
        while (*ptr && !(*ptr == '"' && (*(ptr+1) == ',' || *(ptr+1) == '\0' || *(ptr+1) == '\r' || *(ptr+1) == '\n'))) {
            ptr++;
        }
        
        if (*ptr == '"') {
            *ptr = '\0'; // termina string
            ptr++;
            if (*ptr == ',') ptr++; // pula vírgula
        
        } else {
            // aspas não fechada
            break;
        }
    }
    *num_fields = i;
}

void read_csv(int fields_length, const char *filename, void (*callback)(char **fields, int num_fields, void* user_data, FILE* errors_file), void* user_data, FILE* errors_file) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Erro ao abrir ficheiro");
        return;
    }

    char line[100000];
    int first_line = 1;
    
    while (fgets(line, sizeof(line), fp)) { 
        if (first_line) {
            first_line = 0;
            continue;
        }
        
        line[strcspn(line, "\r\n")] = '\0'; // remove newline characters

        char *fields[fields_length];  // array de strings
        int num_fields = 0;

        parse_csv_line(line, fields, fields_length, &num_fields);

        if (num_fields == fields_length && callback) {
            callback(fields, num_fields, user_data, errors_file);
        }
    }

    fclose(fp);
}

void load_datasets(const char* dataset_path, CatalogManager* catalog_manager) {

    char airports_file[256];
    snprintf(airports_file, sizeof(airports_file), "%s/airports.csv", dataset_path); 

    char aircrafts_file[256];
    snprintf(aircrafts_file, sizeof(aircrafts_file), "%s/aircrafts.csv", dataset_path);

    char flights_file[256];
    snprintf(flights_file, sizeof(flights_file), "%s/flights.csv", dataset_path);
  
    char passengers_file[256];
    snprintf(passengers_file, sizeof(passengers_file), "%s/passengers.csv", dataset_path);

    char reservations_file[256];
    snprintf(reservations_file, sizeof(reservations_file), "%s/reservations.csv", dataset_path);
  
    //printf("\nA inicializar ficheiro de erros de aeroportos...\n");
    init_airports_errors_file();
    FILE *airports_errors = fopen("resultados/airports_errors.csv", "a");

    //printf("\nA carregar os aeroportos de: %s", airports_file);
    read_csv(8, airports_file, process_valid_line_airports, catalog_manager, airports_errors);
    fclose(airports_errors);
    //printf("\nTodos os aeroportos válidos foram carregados!\n");

    //printf("\nA inicializar ficheiro de erros de aeronaves...\n");
    init_aircrafts_errors_file();
    FILE *aircrafts_errors = fopen("resultados/aircrafts_errors.csv", "a");

    //printf("\nA carregar as aeronaves de: %s", aircrafts_file);
    read_csv(6, aircrafts_file, process_valid_line_aircrafts, catalog_manager, aircrafts_errors);
    fclose(aircrafts_errors);
    //printf("\nTodas as aeronaves válidas foram carregadas!\n");

    //printf("\nA inicializar ficheiro de erros de voos...\n");
    init_flights_errors_file();
    FILE *flights_errors = fopen("resultados/flights_errors.csv", "a");

    //printf("\nA carregar os voos de: %s", flights_file);
    read_csv(12, flights_file, process_valid_line_flights, catalog_manager, flights_errors);
    fclose(flights_errors);
    airport_catalog_sort_all_flights(get_airports_from_catalog_manager(catalog_manager)); // ordenar voos por actual_departure para query3
    //printf("\nTodos os voos válidos foram carregados!\n");

    //printf("\nA inicializar ficheiro de erros de passageiros...\n");
    init_passengers_errors_file();
    FILE *passengers_errors = fopen("resultados/passengers_errors.csv", "a");

    //printf("\nA carregar os passageiros de: %s", passengers_file);
    read_csv(10, passengers_file, process_valid_line_passengers, catalog_manager, passengers_errors);
    fclose(passengers_errors);
    //printf("\nTodos os passageiros válidos foram carregados!\n");

    //printf("\nA inicializar ficheiro de erros de reservas...\n");
    init_reservations_errors_file();
    FILE *reservations_errors = fopen("resultados/reservations_errors.csv", "a");

    //printf("\nA carregar as reservas de: %s", reservations_file);
    read_csv(8, reservations_file, process_valid_line_reservations, catalog_manager, reservations_errors);
    fclose(reservations_errors);
    reservation_catalog_prepare_metrics(get_reservations_from_catalog_manager(catalog_manager));
    //printf("\nTodos as reservas válidas foram carregadas!\n");

}