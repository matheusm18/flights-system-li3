#ifndef AIRCRAFT_CATALOG_H
#define AIRCRAFT_CATALOG_H

#include <glib.h>
#include "entities/aircraft.h"

typedef struct aircraft_catalog AircraftCatalog;

/**
 * @brief Cria e inicializa um catálogo de aeronaves.
 * 
 * Esta função aloca memória para um novo catálogo de aeronaves e inicializa
 * uma tabela hash (GHashTable) para armazenar as aeronaves indexadas pelo
 * seu identificador. A tabela hash é configurada para libertar automaticamente
 * a memória das chaves (strings) e dos valores (objetos Aircraft) quando
 * os elementos são removidos ou a tabela é destruída.
 * 
 * @return Ponteiro para o catálogo de aeronaves criado, ou NULL se a alocação
 *         de memória falhar.
 * 
 * @note A tabela hash utiliza g_str_hash para função de hash e g_str_equal
 *       para comparação de chaves. As chaves são libertadas com g_free e os
 *       valores com destroy_aircraft.
 * 
 * @see destroy_aircraft
 */
AircraftCatalog* aircraft_catalog_create();

/**
 * @brief Destrói um catálogo de aeronaves e liberta toda a memória associada.
 * 
 * Esta função liberta toda a memória alocada para o catálogo de aeronaves,
 * incluindo a tabela hash e todos os objetos Aircraft nela contidos. A função
 * g_hash_table_destroy invoca automaticamente as funções de destruição
 * configuradas (g_free para as chaves e destroy_aircraft para os valores).
 * 
 * @param manager Ponteiro para o catálogo de aeronaves a ser destruído.
 * 
 * @return void
 * 
 * @note Se manager for NULL, a função não realiza nenhuma operação.
 *       Todos os objetos Aircraft no catálogo são automaticamente destruídos
 *       devido à configuração da tabela hash com destroy_aircraft.
 * 
 */
void aircraft_catalog_destroy(AircraftCatalog* manager);

/**
 * @brief Adiciona uma aeronave ao catálogo.
 * 
 * Esta função adiciona um objeto Aircraft ao catálogo, indexando-o pelo seu
 * identificador. O identificador é duplicado (g_strdup) para servir como chave
 * na tabela hash. Se já existir uma aeronave com o mesmo identificador, ela
 * será substituída e a anterior será automaticamente destruída.
 * 
 * @param manager Ponteiro para o catálogo de aeronaves.
 * @param aircraft Ponteiro para o objeto Aircraft a ser adicionado.
 * 
 * @return void
 * 
 * @note Se manager, aircraft ou o identificador da aeronave forem NULL,
 *       a função não realiza nenhuma operação. O identificador é duplicado
 *       para garantir que a tabela hash tenha sua própria cópia da chave.
 * 
 */
void aircraft_catalog_add(AircraftCatalog* manager, Aircraft* aircraft);

/**
 * @brief Inicializa um iterador para percorrer o catálogo de aeronaves.
 * 
 * Esta função configura um iterador (GHashTableIter) para permitir a iteração
 * sobre todas as aeronaves no catálogo. O iterador pode ser usado com
 * g_hash_table_iter_next() para aceder sequencialmente a cada aeronave.
 * 
 * @param catalog Ponteiro para o catálogo de aeronaves a ser percorrido.
 * @param iter Ponteiro para a estrutura GHashTableIter a ser inicializada.
 * 
 * @return void
 * 
 * @note Se catalog ou iter forem NULL, a função não realiza nenhuma operação.
 *       Após a inicialização, use g_hash_table_iter_next() para iterar sobre
 *       os elementos do catálogo.
 * 
 */
void aircraft_catalog_iter_init(const AircraftCatalog* catalog, GHashTableIter* iter);

/**
 * @brief Obtém a próxima aeronave do iterador do catálogo.
 * 
 * Esta função avança o iterador para o próximo elemento da tabela hash e
 * retorna a aeronave correspondente. Deve ser usada em conjunto com
 * aircraft_catalog_iter_init() para percorrer todas as aeronaves do catálogo.
 * 
 * @param iter Ponteiro para o iterador GHashTableIter previamente inicializado.
 * 
 * @return Ponteiro constante para a próxima aeronave (Aircraft) no catálogo,
 *         ou NULL se não houver mais elementos ou se iter for NULL.
 * 
 * @note A aeronave retornada não deve ser modificada (ponteiro constante).
 *       Esta função deve ser chamada repetidamente até retornar NULL para
 *       percorrer todo o catálogo.
 * 
 */
const Aircraft* aircraft_catalog_iter_next(GHashTableIter* iter);

/**
 * @brief Obtém uma aeronave do catálogo pelo seu identificador.
 * 
 * Esta função procura e retorna uma aeronave no catálogo usando o seu
 * identificador como chave de pesquisa. A pesquisa é realizada na tabela
 * hash do catálogo, garantindo acesso eficiente em tempo O(1) médio.
 * 
 * @param manager Ponteiro para o catálogo de aeronaves.
 * @param identifier String contendo o identificador da aeronave a procurar.
 * 
 * @return Ponteiro para o objeto Aircraft se encontrado, ou NULL se a aeronave
 *         não existir no catálogo, ou se manager ou identifier forem NULL.
 * 
 * @note A aeronave retornada pode ser modificada pelo chamador. Para acesso
 *       apenas de leitura, considere usar uma versão que retorne const Aircraft*.
 * 
 */
Aircraft* get_aircraft_by_identifier(AircraftCatalog* manager, const char* identifier);

/**
 * @brief Incrementa o contador de voos de uma aeronave específica.
 * 
 * Esta função procura uma aeronave no catálogo pelo seu identificador e
 * incrementa o seu contador de voos. É utilizada para registar cada vez
 * que uma aeronave realiza um voo (não cancelado).
 * 
 * @param aircraft_id String contendo o identificador da aeronave.
 * @param manager Ponteiro para o catálogo de aeronaves.
 * 
 * @return void
 * 
 * @note Se manager ou aircraft_id forem NULL, ou se aircraft_id for uma string
 *       vazia, ou se a aeronave não existir no catálogo, a função não realiza
 *       nenhuma operação.
 * 
 * @see get_aircraft_by_identifier
 * @see aircraft_increment_flight_count
 */
void aircrafts_counter_increment(const char* aircraft_id, AircraftCatalog* manager);

/**
 * @brief Obtém o número total de aeronaves no catálogo.
 * 
 * Esta função retorna a quantidade total de aeronaves armazenadas no catálogo,
 * consultando o tamanho da tabela hash que as contém.
 * 
 * @param catalog Ponteiro para o catálogo de aeronaves.
 * 
 * @return Número total de aeronaves no catálogo, ou 0 se catalog for NULL.
 * 
 */
int get_total_aircrafts_in_catalog(AircraftCatalog* catalog);

#endif
