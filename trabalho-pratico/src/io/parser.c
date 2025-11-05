#include "io/parser.h"
#include "entities/airport.h"
#include "utils/validation.h"
#include "utils/passenger_validation.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

void remove_quotation_marks(char *s) {
    if (!s) return;
    
    size_t len = strlen(s);
    if (len >= 2 && s[0] == '"' && s[len - 1] == '"') {
        for (size_t i = 1; i < len - 1; i++) {
            s[i - 1] = s[i];
        }
        s[len - 2] = '\0';
    }
}

void read_csv(int fields_length, const char *filename, void (*callback)(char **fields, int num_fields, void* catalog), void* catalog) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Erro ao abrir ficheiro");
        return;
    }

    char line[1024];
    int first_line = 1;

    while (fgets(line, sizeof(line), fp)) {  // lê uma linha: a string lida de fp será armazenada em line(buffer, uma área temporária de armazenamento), lê um número máximo de caracteres
        if (first_line) {
            first_line = 0;
            continue;
        }

        line[strcspn(line, "\r\n")] = '\0'; // remover \n do final da string na posição devolvida por strcspn

        char *fields[fields_length];
        int num_fields = 0;
        char *token = strtok(line, "\""); // delimitar com aspas duplas

        while (token && num_fields < fields_length) {
            if (!(token[0] == ',' && token[1] == '\0')) { // verifica se não é a virgula entre os dados
                remove_quotation_marks(token);
                fields[num_fields++] = token;
            }
            token = strtok(NULL, "\"");
        }

        if (callback)
            callback(fields, num_fields, catalog);
    }

    fclose(fp);
}

void load_datasets(const char* dataset_path, AirportCatalog* airport_catalog, AircraftCatalog* aircraft_catalog, FlightCatalog* flight_catalog, PassengerCatalog* passenger_catalog, ReservationCatalog* reservation_catalog) {

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
  
    printf("\nA inicializar ficheiro de erros de aeroportos...\n");
    init_airports_errors_file();

    printf("\nA carregar os aeroportos de: %s", airports_file);
    read_csv(8, airports_file, process_valid_line_airports, airport_catalog);
    printf("\nTodos os aeroportos válidos foram carregados!\n");

    printf("\nA inicializar ficheiro de erros de aeronaves...\n");
    init_aircrafts_errors_file();

    printf("\nA carregar as aeronaves de: %s", aircrafts_file);
    read_csv(6, aircrafts_file, process_valid_line_aircrafts, aircraft_catalog);
    printf("\nTodas as aeronaves válidas foram carregadas!\n");

    printf("\nA inicializar ficheiro de erros de voos...\n");
    init_flights_errors_file();

    printf("\nA carregar os voos de: %s", flights_file);
    read_csv(12, flights_file, process_valid_line_flights, flight_catalog);
    printf("\nTodos os voos válidos foram carregados!\n");

    printf("\nA inicializar ficheiro de erros de passageiros...\n");
    init_passengers_errors_file();

    printf("\nA carregar os passageiros de: %s", passengers_file);
    read_csv(9, passengers_file, process_valid_line_passengers, passenger_catalog);
    printf("\nTodos os passageiros válidos foram carregados!\n");


    printf("\nA inicializar ficheiro de erros de reservas...\n");
    init_reservations_errors_file();

    printf("\nA carregar as reservas de: %s", reservations_file);
    read_csv(8, reservations_file, process_valid_line_reservations, reservation_catalog);
    printf("\nTodos as reservas válidas foram carregadas!\n");

}