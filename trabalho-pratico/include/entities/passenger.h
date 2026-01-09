#ifndef PASSENGER_H
#define PASSENGER_H

/**
 * @typedef Passenger
 * @brief Estrutura opaca que representa um passageiro.
 *
 * Contém informações pessoais básicas:
 *  - Número de documento
 *  - Nome próprio e apelido
 *  - Data de nascimento
 *  - Nacionalidade
 *  - Sexo
 *
 * A implementação concreta encontra-se no ficheiro passenger.c.
 */
typedef struct passenger Passenger;

/**
 * @brief Cria e inicializa um novo passageiro.
 *
 * Os campos de texto são armazenados por REFERÊNCIA para poupar memória.
 *
 * @param document_number Número de documento.
 * @param first_name Nome próprio (NÃO duplicado; vem da String Pool).
 * @param last_name Apelido (NÃO duplicado; vem da String Pool).
 * @param dob Data de nascimento.
 * @param nationality Nacionalidade (NÃO duplicado; vem da String Pool).
 *
 * @return Ponteiro para Passenger ou NULL.
 */
Passenger* create_passenger(const char* document_number, const char* first_name, const char* last_name, int dob, const char* nationality);

/**
 * @brief Liberta a memória de um passageiro.
 *
 * NÃO liberta `first_name`, `last_name` ou `nationality`(pertencem à StringPool).
 *
 * @param p Ponteiro para o passageiro.
 */
void destroy_passenger(Passenger* p);

/**
 * @brief Obtém o número de documento de um passageiro.
 *
 * @param passenger Ponteiro para a estrutura Passenger.
 *
 * @return Número de documento ou -1 se passenger for NULL.
 */
int get_passenger_dnumber(const Passenger* passenger);

/**
 * @brief Obtém o primeiro nome de um passageiro.
 *
 * @param passenger Ponteiro para a estrutura Passenger.
 *
 * @return String dinamicamente alocada com o primeiro nome, ou NULL se passenger for NULL.
 *
 * @note A string devolvida deve ser libertada pelo caller.
 */
char* get_passenger_first_name(const Passenger* passenger);

/**
 * @brief Obtém o apelido de um passageiro.
 *
 * @param passenger Ponteiro para a estrutura Passenger.
 *
 * @return String dinamicamente alocada com o apelido, ou NULL se passenger for NULL.
 *
 * @note A string devolvida deve ser libertada pelo caller.
 */
char* get_passenger_last_name(const Passenger* passenger);

/**
 * @brief Obtém a data de nascimento de um passageiro.
 *
 * @param passenger Ponteiro para a estrutura Passenger.
 *
 * @return Data de nascimento como int, ou -1 se passenger for NULL.
 */
int get_passenger_dob(const Passenger* passenger);

/**
 * @brief Obtém a nacionalidade de um passageiro.
 *
 * @param passenger Ponteiro para a estrutura Passenger.
 *
 * @return String dinamicamente alocada com a nacionalidade, ou NULL se passenger for NULL.
 *
 * @note A string devolvida deve ser libertada pelo caller.
 */
char* get_passenger_nationality(const Passenger* passenger);

#endif
