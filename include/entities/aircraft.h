#ifndef AIRCRAFT_H
#define AIRCRAFT_H

/**
 * @struct aircraft
 * @brief Estrutura que representa uma aeronave.
 *
 * A estrutura aircraft armazena informação essencial sobre uma aeronave, incluindo:
 *  - Identificador único da aeronave
 *  - Fabricante da aeronave
 *  - Modelo da aeronave
 *  - Ano de fabrico
 */
typedef struct aircraft Aircraft;

/**
 * @brief Cria um novo objeto Aircraft.
 * * Aloca memória para a estrutura Aircraft.
 * - O `identifier` é COPIADO internamente (max 8 chars).
 * - `manufacturer` e `model` são armazenados por REFERÊNCIA.
 * * @param identifier Identificador da aeronave (Será copiado).
 * @param manufacturer Ponteiro para o fabricante (NÃO é duplicado; vem da String Pool).
 * @param model Ponteiro para o modelo (NÃO é duplicado; vem da String Pool).
 * @param year Ano de fabrico.
 * @param capacity Capacidade (não utilizado).
 * @param range Alcance (não utilizado).
 * * @return Ponteiro para o objeto Aircraft criado.
 * * @warning O caller não deve libertar `manufacturer` ou `model` visto que estes pertencem à String Pool.
 */
Aircraft* create_aircraft(const char* identifier, const char* manufacturer, const char* model, unsigned short year, unsigned short capacity, unsigned short range);

/**
 * @brief Destrói um objeto Aircraft.
 * * Liberta a estrutura Aircraft.
 * NÃO liberta as strings `manufacturer` e `model` (pertencem à StringPool).
 * * @param a Ponteiro para o objeto Aircraft.
 */
void destroy_aircraft(Aircraft* a);

/**
 * @brief Obtém o identificador de uma aeronave.
 * 
 * Esta função retorna o identificador de uma aeronave.
 * 
 * @param a Ponteiro para o objeto Aircraft.
 * 
 * @return String que contêm o identificador da aeronave, ou NULL se a for NULL.
 * 
 */
char* get_aircraft_identifier(const Aircraft* a);

/**
 * @brief Obtém o nome do fabricante de uma aeronave.
 * 
 * Esta função retorna o nome do fabricante de uma aeronave.
 * 
 * @param a Ponteiro para o objeto Aircraft.
 * 
 * @return String que contêm o nome do fabricante, ou NULL se a for NULL.
 * 
 */
char* get_aircraft_manufacturer(const Aircraft* a);

/**
 * @brief Obtém o modelo de uma aeronave.
 * 
 * Esta função retorna o modelo de uma aeronave.
 * 
 * @param a Ponteiro para o objeto Aircraft.
 * 
 * @return String que contêm o modelo da aeronave, ou NULL se a for NULL.
 */
char* get_aircraft_model(const Aircraft* a);

#endif