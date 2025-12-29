#ifndef PASSENGER_CATALOG_H
#define PASSENGER_CATALOG_H

#include <glib.h>
#include "entities/passenger.h"

/**
 * @typedef PassengerCatalog
 * @brief Estrutura opaca que representa um catálogo de passageiros.
 *
 * O catálogo de passageiros encapsula uma tabela hash que associa
 * números de documento a estruturas Passenger, permitindo acesso
 * rápido, verificação de existência e gestão centralizada dos passageiros.
 *
 * A definição concreta da estrutura encontra-se no ficheiro de implementação
 * (.c), garantindo encapsulamento e abstração da implementação interna.
 */
typedef struct passenger_catalog PassengerCatalog;

/**
 * @brief Cria e inicializa um catálogo de passageiros.
 *
 * Esta função aloca memória para um novo catálogo de passageiros e inicializa
 * uma tabela hash (GHashTable) que associa números de documento a ponteiros
 * para estruturas Passenger.
 *
 * A tabela hash utiliza os números de documento como chave, permitindo
 * operações eficientes de inserção, pesquisa e verificação de existência.
 *
 * @return Ponteiro para o catálogo de passageiros criado, ou NULL caso
 *         a alocação de memória falhe.
 *
 * @note A tabela hash utiliza g_direct_hash e g_direct_equal para permitir
 *       o uso de inteiros como chaves. Os valores armazenados são Passenger*,
 *       cuja destruição é tratada automaticamente quando o catálogo é destruído.
 */
PassengerCatalog* passenger_catalog_create();

/**
 * @brief Destrói um catálogo de passageiros e liberta toda a memória associada.
 *
 * Esta função destrói a tabela hash interna do catálogo e liberta toda a
 * memória alocada para os passageiros armazenados, bem como a própria
 * estrutura do catálogo.
 *
 * A destruição dos objetos Passenger é realizada através da função de
 * destruição configurada na tabela hash.
 *
 * @param manager Ponteiro para o catálogo de passageiros a ser destruído.
 *
 * @return void
 *
 * @note Se o ponteiro manager for NULL, a função não realiza nenhuma operação.
 */
void passenger_catalog_destroy(PassengerCatalog* manager);

/**
 * @brief Adiciona um passageiro ao catálogo.
 *
 * Esta função insere um passageiro no catálogo, associando o seu número
 * de documento ao ponteiro da estrutura Passenger correspondente.
 *
 * Caso já exista um passageiro com o mesmo número de documento, o valor
 * anterior é substituído de acordo com o comportamento padrão da GHashTable,
 * sendo o objeto antigo libertado automaticamente.
 *
 * @param manager Ponteiro para o catálogo de passageiros.
 * @param passenger Ponteiro para a estrutura Passenger a adicionar.
 *
 * @return void
 *
 * @note Se manager ou passenger forem NULL, a função não realiza nenhuma operação.
 */
void passenger_catalog_add(PassengerCatalog* manager, Passenger* passenger);

/**
 * @brief Verifica se um número de documento existe no catálogo.
 *
 * Esta função verifica se existe um passageiro registado no catálogo com
 * o número de documento fornecido.
 *
 * É utilizada, por exemplo, para validar a existência de um passageiro
 * antes da criação de reservas ou outras operações dependentes.
 *
 * @param manager Ponteiro para o catálogo de passageiros.
 * @param document_number Número de documento a verificar.
 *
 * @return 1 se o número de documento existe no catálogo, 0 caso contrário.
 *
 * @note A verificação é realizada utilizando g_hash_table_contains,
 *       garantindo eficiência na pesquisa.
 */
int passenger_catalog_dnumber_exists(PassengerCatalog* manager, int document_number);

/**
 * @brief Obtém um passageiro a partir do número de documento.
 *
 * Esta função procura no catálogo um passageiro associado ao número
 * de documento fornecido e devolve um ponteiro para a estrutura Passenger
 * correspondente.
 *
 * @param manager Ponteiro para o catálogo de passageiros.
 * @param document_number Número de documento do passageiro a procurar.
 *
 * @return Ponteiro para a estrutura Passenger se o passageiro existir,
 *         ou NULL caso o número de documento não esteja registado ou
 *         se manager for NULL.
 */
Passenger* get_passenger_by_dnumber(PassengerCatalog* manager, int document_number);

#endif
