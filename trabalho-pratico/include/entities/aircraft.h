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
 * 
 * Esta função aloca memória e inicializa um novo objeto Aircraft com os
 * dados fornecidos. O identificador é copiado com limite de 8 caracteres,
 * enquanto o fabricante e modelo são duplicados dinamicamente. O contador
 * de voos é inicializado a zero.
 * 
 * @param identifier String que contêm o identificador da aeronave (máximo 8 caracteres).
 * @param manufacturer String que contêm o nome do fabricante.
 * @param model String que contêm o modelo da aeronave.
 * @param year Ano de fabricação da aeronave.
 * @param capacity Capacidade de passageiros 
 * @param range Alcance da aeronave 
 * 
 * @return Ponteiro para o objeto Aircraft criado, ou NULL se a alocação falhar.
 * 
 * @note Os campos capacity e range estão atualmente comentados (implementação futura).
 *       O identificador é limitado a 8 caracteres mais o terminador '\0'.
 * 
 * @see destroy_aircraft
 */
Aircraft* create_aircraft(const char* identifier, const char* manufacturer, const char* model, unsigned short year, unsigned short capacity, unsigned short range);

/**
 * @brief Destrói um objeto Aircraft e liberta toda a memória associada.
 * 
 * Esta função liberta toda a memória alocada para um objeto Aircraft,
 * incluindo as strings alocadas dinamicamente (manufacturer e model)
 * e a própria estrutura.
 * 
 * @param a Ponteiro para o objeto Aircraft a ser destruído.
 * 
 * @return void
 * 
 * @note Se a for NULL, a função não realiza nenhuma operação.
 * 
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