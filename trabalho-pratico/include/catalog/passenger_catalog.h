#ifndef PASSENGER_CATALOG_H
#define PASSENGER_CATALOG_H

#include <glib.h>
#include "entities/passenger.h"

typedef struct passenger_catalog PassengerCatalog;

/**
 * @brief Cria e inicializa um catálogo de passageiros.
 * 
 * Esta função aloca memória para um novo catálogo de passageiros e inicializa
 * uma tabela hash (GHashTable) para armazenar números de documento. Atualmente,
 * apenas os números de documento são armazenados (implementação simplificada),
 * funcionando como um conjunto (set) para validação rápida de existência.
 * 
 * @return Ponteiro para o catálogo de passageiros criado, ou NULL se a alocação
 *         de memória falhar.
 * 
 * @note A tabela hash utiliza g_direct_hash e g_direct_equal para trabalhar
 *       com inteiros como chaves. Não há funções de destruição configuradas
 *       pois apenas números inteiros são armazenados (não há alocação dinâmica).
 * 
 */
PassengerCatalog* passenger_catalog_create();

/**
 * @brief Destrói um catálogo de passageiros e liberta toda a memória associada.
 * 
 * Esta função liberta toda a memória alocada para o catálogo de passageiros,
 * incluindo a tabela hash. Como apenas números inteiros são armazenados,
 * não há necessidade de libertar objetos complexos.
 * 
 * @param manager Ponteiro para o catálogo de passageiros a ser destruído.
 * 
 * @return void
 * 
 * @note Se manager for NULL, a função não realiza nenhuma operação.
 * 
 */
void passenger_catalog_destroy(PassengerCatalog* manager);

/**
 * @brief Adiciona um número de documento ao catálogo de passageiros.
 * 
 * Esta função adiciona um número de documento à tabela hash do catálogo,
 * funcionando como um conjunto (set) para permitir verificação rápida de
 * existência. Nesta fase simplificada, apenas o número de documento é
 * armazenado, sem objeto Passenger associado.
 * 
 * @param manager Ponteiro para o catálogo de passageiros.
 * @param document_number Número de documento do passageiro a ser adicionado.
 * 
 * @return void
 * 
 * @note Se manager for NULL, a função não realiza nenhuma operação.
 *       O valor associado à chave é NULL, pois apenas queremos um conjunto
 *       de números de documento válidos. Esta é uma implementação simplificada
 *       para validação de reservas.
 * 
 */
void passenger_catalog_add(PassengerCatalog* manager, int document_number);

/**
 * @brief Verifica se um número de documento existe no catálogo.
 * 
 * Esta função verifica se um determinado número de documento está registado
 * no catálogo de passageiros, permitindo validar rapidamente se um passageiro
 * existe no sistema antes de criar uma reserva.
 * 
 * @param manager Ponteiro para o catálogo de passageiros.
 * @param document_number Número de documento a verificar.
 * 
 * @return 1 (true) se o número de documento existe no catálogo, 0 (false) caso contrário.
 * 
 * @note Usa g_hash_table_contains em vez de g_hash_table_lookup porque atualmente apenas
 *       armazenamos chaves (conjunto), sem valores associados.

 */
int passenger_catalog_dnumber_exists(PassengerCatalog* manager, int document_number);

#endif