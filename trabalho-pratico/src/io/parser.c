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

void load_csv_file(const char* path, int num_fields,
                   void (*process_line)(char **fields, int num_fields, void* user_data, FILE* errors_file),
                   CatalogManager* catalog_manager,
                   const char* errors_file_name,
                   void (*init_errors_file)()) {

    init_errors_file();
    FILE *errors_file = fopen(errors_file_name, "a");
    if (!errors_file) { perror("Erro ao abrir arquivo de erros"); return; }

    read_csv(num_fields, path, process_line, catalog_manager, errors_file);
    fclose(errors_file);
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
  
    load_csv_file(airports_file, 8, process_valid_line_airports, catalog_manager,
                  "resultados/airports_errors.csv", init_airports_errors_file);

    load_csv_file(aircrafts_file, 6, process_valid_line_aircrafts, catalog_manager,
                  "resultados/aircrafts_errors.csv", init_aircrafts_errors_file);

    load_csv_file(flights_file, 12, process_valid_line_flights, catalog_manager,
                  "resultados/flights_errors.csv", init_flights_errors_file);
    airport_catalog_sort_all_flights(get_airports_from_catalog_manager(catalog_manager));

    load_csv_file(passengers_file, 10, process_valid_line_passengers, catalog_manager,
                  "resultados/passengers_errors.csv", init_passengers_errors_file);

    load_csv_file(reservations_file, 8, process_valid_line_reservations, catalog_manager,
                  "resultados/reservations_errors.csv", init_reservations_errors_file);

    passenger_catalog_prepare_query4(get_passengers_from_catalog_manager(catalog_manager)); // preparar a query4

}