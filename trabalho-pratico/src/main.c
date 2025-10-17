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

int main(int argc, char **argv) {
    
    // tres args visto que ele considera o nome do programa como primeiro arg
    if (argc != 3) {
        fprintf(stderr, "Usage format: %s <dataset_path> <command_list_path>\n", argv[0]);
        return 1;
    }

    const char* dataset_path = argv[1];
    //const char* commands_path = argv[2];

    load_datasets(dataset_path);

    return 0;
}