#ifndef FLIGHT_CATALOG_H
#define FLIGHT_CATALOG_H

#include <glib.h>
#include "entities/flight.h"

typedef struct flight_catalog FlightCatalog;

/**
 * @brief Cria e inicializa um catálogo de voos.
 * 
 * Esta função aloca memória para um novo catálogo de voos e inicializa
 * uma tabela hash (GHashTable) para armazenar os voos indexados pelo
 * seu identificador (flight ID). A tabela hash é configurada para liberar
 * automaticamente a memória das chaves (strings) e dos valores (objetos Flight)
 * quando os elementos são removidos ou a tabela é destruída.
 * 
 * @return Ponteiro para o catálogo de voos criado, ou NULL se a alocação
 *         de memória falhar.
 * 
 * @note A tabela hash utiliza g_str_hash para função de hash e g_str_equal
 *       para comparação de chaves. As chaves são libertadas com g_free e os
 *       valores com destroy_flight.
 * 
 * @see destroy_flight
 */
FlightCatalog* flight_catalog_create(void);

/**
 * @brief Destrói um catálogo de voos e liberta toda a memória associada.
 * 
 * Esta função liberta toda a memória alocada para o catálogo de voos,
 * incluindo a tabela hash e todos os objetos Flight nela contidos. A função
 * g_hash_table_destroy invoca automaticamente as funções de destruição
 * configuradas (g_free para as chaves e destroy_flight para os valores).
 * 
 * @param manager Ponteiro para o catálogo de voos a ser destruído.
 * 
 * @return void
 * 
 * @note Se manager for NULL, a função não realiza nenhuma operação.
 *       Todos os objetos Flight no catálogo são automaticamente destruídos
 *       devido à configuração da tabela hash com destroy_flight.
 * 
 */
void flight_catalog_destroy(FlightCatalog* manager);

/**
 * @brief Adiciona um voo ao catálogo.
 * 
 * Esta função adiciona um objeto Flight ao catálogo, indexando-o pelo seu
 * identificador (flight ID). O identificador é duplicado (g_strdup) para
 * servir como chave na tabela hash. Se já existir um voo com o mesmo
 * identificador, ele será substituído e o anterior será automaticamente destruído.
 * 
 * @param manager Ponteiro para o catálogo de voos.
 * @param flight Ponteiro para o objeto Flight a ser adicionado.
 * 
 * @return void
 * 
 * @note Se manager, flight ou o identificador do voo forem NULL, a função
 *       não realiza nenhuma operação. O identificador é duplicado para garantir
 *       que a tabela hash tenha sua própria cópia da chave.
 */
void flight_catalog_add(FlightCatalog* manager, Flight* flight);

/**
 * @brief Obtém um voo do catálogo pelo seu identificador.
 * 
 * Esta função procura e retorna um voo no catálogo usando o seu
 * identificador como chave de pesquisa. A pesquisa é realizada na tabela
 * hash do catálogo, garantindo acesso eficiente em tempo O(1) médio.
 * 
 * @param manager Ponteiro para o catálogo de voos.
 * @param flight_id String contendo o identificador do voo a procurar.
 * 
 * @return Ponteiro para o objeto Flight se encontrado, ou NULL se o voo
 *         não existir no catálogo, ou se manager ou flight_id forem NULL.
 * 
 */
Flight* get_flight_by_flight_id_from_catalog(FlightCatalog* manager, const char* flight_id);

/**
 * @brief Obtém a origem de um voo através do seu identificador.
 * 
 * Esta função procura um voo no catálogo pelo seu identificador e retorna
 * o código do aeroporto de origem desse voo.
 * 
 * @param manager Ponteiro para o catálogo de voos.
 * @param flight_id String contendo o identificador do voo.
 * 
 * @return String contendo o código do aeroporto de origem, ou NULL se o voo
 *         não for encontrado, ou se manager ou flight_id forem NULL.
 * 
 * @see get_flight_origin
 */
const char* flight_catalog_get_origin(const FlightCatalog* manager, const char* flight_id);

/**
 * @brief Obtém o destino de um voo através do seu identificador.
 * 
 * Esta função procura um voo no catálogo pelo seu identificador e retorna
 * o código do aeroporto de destino desse voo.
 * 
 * @param manager Ponteiro para o catálogo de voos.
 * @param flight_id String contendo o identificador do voo.
 * 
 * @return String contendo o código do aeroporto de destino, ou NULL se o voo
 *         não for encontrado, ou se manager ou flight_id forem NULL.
 * 
 * @see get_flight_destination
 */
const char* flight_catalog_get_destination(const FlightCatalog* manager, const char* flight_id);

/**
 * @brief Obtém o número total de voos no catálogo.
 * 
 * Esta função retorna a quantidade total de voos armazenados no catálogo,
 * consultando o tamanho da tabela hash que os contém.
 * 
 * @param manager Ponteiro para o catálogo de voos.
 * 
 * @return Número total de voos no catálogo, ou 0 se manager for NULL.
 * 
 */
int flight_catalog_get_count(FlightCatalog* manager);

/**
 * @brief Obtém a tabela hash interna do catálogo de voos.
 * 
 * Esta função retorna o ponteiro direto para a GHashTable que armazena
 * os voos. Deve ser usada com cuidado, pois permite acesso direto à
 * estrutura interna do catálogo.
 * 
 * @param manager Ponteiro para o catálogo de voos.
 * 
 * @return Ponteiro para a GHashTable interna, ou NULL se manager for NULL.
 * 
 * @note Esta função expõe a implementação interna. Modificações diretas
 *       na tabela hash devem ser feitas com cuidado para não comprometer
 *       a integridade do catálogo.
 * 
 */
GHashTable* get_flight_catalog(FlightCatalog* manager);

#endif
