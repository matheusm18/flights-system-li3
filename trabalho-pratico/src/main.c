#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "catalog/flight_catalog.h"
#include "catalog/airport_catalog.h" 
#include "catalog/aircraft_catalog.h"
#include "catalog/passenger_catalog.h"
#include "catalog/reservation_catalog.h"

#include "io/parser.h"
#include "io/command_processor.h"
#include "queries/query1.h"
#include "queries/query2.h"

int main(int argc, char **argv) {
    
    // tres args visto que ele considera o nome do programa como primeiro arg
    if (argc != 3) {
        fprintf(stderr, "Usage format: %s <dataset_path> <command_list_path>\n", argv[0]);
        return 1;
    }

    const char* dataset_path = argv[1];
    const char* commands_path = argv[2];

    // criar catalogs
    AirportCatalog* airport_catalog = airport_catalog_create();
    AircraftCatalog* aircraft_catalog = aircraft_catalog_create();
    FlightCatalog* flight_catalog = flight_catalog_create();
    // PassengerCatalog* passenger_catalog = passenger_catalog_create();
    // ReservationCatalog* reservation_catalog = reservation_catalog_create();

    // carregar datasets
    printf("\nA carregar datasets de: %s\n", dataset_path);
    load_datasets(dataset_path, airport_catalog, aircraft_catalog, flight_catalog, NULL, NULL);

    // processar comandos do ficheiro
    printf("\nA processar comandos de: %s\n", commands_path);
    process_commands(commands_path, airport_catalog, flight_catalog, aircraft_catalog);

    printf("\nProcessamento concluído! Verifique a pasta 'resultados/'\n");

    // liberar memória
    airport_catalog_destroy(airport_catalog);
    aircraft_catalog_destroy(aircraft_catalog);
    flight_catalog_destroy(flight_catalog);

    return 0;
}