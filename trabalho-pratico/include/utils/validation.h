#ifndef VALIDATION_H
#define VALIDATION_H

#include <stdbool.h>

bool valid_date(char* date);
bool valida_code_airport(const char *codigo);
bool valida_name_city_airport(const char *nome);
bool valida_country_airport(const char *pais);
bool valida_latitude_airport(double latitude);
bool valida_longitude_airport(double longitude);
bool valida_icao_airport(const char *icao);
bool valida_type_airport(const char *type);
void processar_linha_valida(char **campos, int num_campos);

#endif