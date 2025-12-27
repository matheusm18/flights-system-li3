#include "queries/query4.h"
#include "catalog/reservation_catalog.h"
#include "catalog/passenger_catalog.h"
#include "entities/passenger.h"
#include "utils/utils_validation.h"
#include "utils/query4_utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//==== Determinar o passageiro que esteve mais tempo no Top 10 semanal durante um período
QueryResult* execute_query4(ReservationCatalog* reservation_catalog, PassengerCatalog* passenger_catalog, char* begin_date, char* end_date) {
    QueryResult* res = create_query_result();
    
    if (!reservation_catalog || !passenger_catalog) return res;

    // retorna o document_number do passageiro vencedor.
    int count = 0;
    char* winner_id = reservation_catalog_get_top_passenger_in_period(reservation_catalog, begin_date, end_date, &count);

    if (winner_id != NULL) {
        const Passenger* p = get_passenger_by_dnumber(passenger_catalog, atoi(winner_id));

        if (p != NULL) {
            char** tokens = malloc(6 * sizeof(char*));
            
            tokens[0] = strdup(winner_id);
            tokens[1] = get_passenger_first_name(p);
            tokens[2] = get_passenger_last_name(p);
            tokens[3] = date_to_string(get_passenger_dob(p));
            tokens[4] = get_passenger_nationality(p);
            tokens[5] = int_to_string(count);
            
            add_line_to_result(res, tokens, 6);
        }
        
        free(winner_id); // Libertar a string retornada pelo catálogo
    }

    return res;
}