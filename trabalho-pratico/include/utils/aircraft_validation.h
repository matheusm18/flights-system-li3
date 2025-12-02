#ifndef AIRCRAFT_VALIDATION_H
#define AIRCRAFT_VALIDATION_H

#include <stdbool.h>

/**
 * @brief Valida se uma string representa um ano válido de uma aeronave.
 * 
 * A validação inclui verificar se a string tem exatamente
 * 4 dígitos numéricos e se o ano não é posterior a 2025.
 * 
 * @param string String contendo o ano a ser validado.
 * 
 * @return true se a string representa um ano válido (4 dígitos numéricos,
 *         e não superior a 2025).
 * @return false se a string é NULL, não tem 4 caracteres, contém caracteres
 *         não numéricos, ou representa um ano posterior a 2025.
 */
bool validate_year_aircraft(const char *string);

#endif