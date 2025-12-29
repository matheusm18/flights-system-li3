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
 * Constrói dinamicamente uma estrutura Passenger a partir dos dados fornecidos.
 *
 * @param document_number Número de documento do passageiro como string.
 * @param first_name Nome próprio do passageiro.
 * @param last_name Apelido do passageiro.
 * @param dob Data de nascimento do passageiro (formato int).
 * @param nationality Nacionalidade do passageiro.
 * @param gender Género do passageiro como string (primeiro carácter usado).
 *
 * @return Ponteiro para a estrutura Passenger criada, ou NULL em caso de falha.
 */
Passenger* create_passenger(
    char* document_number,
    char* first_name,
    char* last_name,
    int dob,
    char* nationality,
    char* gender
);

/**
 * @brief Liberta a memória de um passageiro.
 *
 * Liberta todos os campos dinamicamente alocados e a própria estrutura Passenger.
 *
 * @param p Ponteiro para o passageiro a destruir.
 *
 * @return void
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
