#ifndef QUERY_DEFS_H
#define QUERY_DEFS_H

#include <stdlib.h>

/**
 * @brief Estrutura que representa um argumento de uma Query.
 *
 * Cada argumento tem:
 *  - Um nome simbólico (ex.: "DATA_INICIO")
 *  - Um indicador se é obrigatório ou opcional
 */
typedef struct query_arg QueryArg;

/**
 * @brief Estrutura que representa uma Query disponível no sistema.
 *
 * Cada Query contém:
 *  - Um identificador numérico
 *  - Um nome curto
 *  - Uma descrição textual
 *  - Um conjunto de argumentos associados
 *  - Informação sobre se permite o modificador 'S'
 *
 * Esta estrutura é usada apenas para leitura fora deste módulo.
 */
typedef struct Query Query;

/**
 * @brief Estrutura para a gestão dinâmica de queries.
 * * Atua como um contentor que armazena todas as definições de queries 
 * disponíveis no sistema. O uso deste tipo permite que o resto do 
 * programa interaja com as queries sem precisar de conhecer a 
 * estrutura interna do array ou do contador.
 */
typedef struct query_manager QueryManager;

/**
 * @brief Cria e inicializa um novo gestor de queries.
 * * Aloca memória para a estrutura QueryManager e para o array interno de queries
 * com a capacidade inicial especificada.
 *
 * @param initial_capacity Capacidade inicial do array de queries (número de slots).
 * @return Ponteiro para o QueryManager criado ou NULL em caso de falha na alocação.
 */
QueryManager* create_query_manager(int initial_capacity);

/**
 * @brief Adiciona uma nova query ao gestor de forma dinâmica.
 * * Esta função regista uma query no sistema, duplicando as strings de nome e 
 * descrição para garantir a integridade dos dados (evitando problemas com ponteiros const).
 * Se a capacidade do gestor for excedida, o array interno é automaticamente redimensionado.
 *
 * @param qm Ponteiro para o QueryManager.
 * @param id Identificador numérico da query (ex: 1, 2, 0 para sair).
 * @param nome Nome curto da query exibido no menu.
 * @param desc Descrição detalhada da funcionalidade da query.
 * @param args Array de estruturas QueryArg com a definição dos argumentos.
 * @param n_args Número de argumentos definidos no array args.
 * @param permite_S Booleano (1/0) que indica se a query suporta o modo de resultados separados por =.
 */
void add_query(QueryManager* qm, int id, const char* nome, const char* desc, QueryArg* args, int n_args, int permite_S);


/**
 * @brief Regista todas as queries do sistema no gestor de queries.
 * 
 * Esta função atua como o configurador central da aplicação. Ela define
 * o ID, nome, descrição, argumentos e permissões de cada query (incluindo
 * a opção de saída do programa), inserindo-as dinamicamente no QueryManager.
 * 
 * * @note Esta é a única função que deve ser modificada caso se pretenda 
 * adicionar, remover ou alterar a definição de uma query no menu interativo.
 * * @param qm Ponteiro para o QueryManager onde as queries serão registadas.
 */
void init_all_queries(QueryManager* qm);

/**
 * @brief Liberta toda a memória associada ao gestor de queries.
 * * Realiza a limpeza completa: liberta as strings duplicadas de cada query (nome e descrição),
 * o array de queries e a própria estrutura do gestor.
 * * @note Não liberta os arrays de argumentos (QueryArg*).
 *
 * @param qm Ponteiro para o QueryManager a ser destruído.
 */
void free_query_manager(QueryManager* qm);

/**
 * @brief Obtém o número total de queries registadas no gestor.
 *
 * Esta função consulta o estado atual do gestor para determinar quantas
 * queries foram adicionadas dinamicamente até ao momento.
 *
 * @param qm Ponteiro para a estrutura QueryManager.
 * @return Número total de queries disponíveis ou 0 se o gestor for NULL.
 */
int get_total_queries(const QueryManager* qm);

/**
 * @brief Obtém uma query pelo seu índice no array interno do gestor.
 *
 * O índice corresponde à ordem em que as queries foram registadas via add_query,
 * sendo utilizado para mapear a seleção do utilizador no menu.
 *
 * @param qm Ponteiro para a estrutura QueryManager.
 * @param index Índice da query (0 <= index < num_queries).
 *
 * @return Ponteiro constante para a Query correspondente ou NULL se o 
 * gestor for inválido ou o índice estiver fora dos limites.
 */
const Query* get_query_at_index(const QueryManager* qm, int index);

/**
 * @brief Obtém o identificador numérico de uma query.
 *
 * @param q Ponteiro para a Query.
 *
 * @return Identificador da query (ex.: 1, 2, 3, ...).
 */
int get_query_id(const Query* q);

/**
 * @brief Obtém o nome curto de uma query.
 *
 * @param q Ponteiro para a Query.
 *
 * @return String com o nome da query.
 */
const char* get_query_nome(const Query* q);

/**
 * @brief Obtém a descrição textual de uma query.
 *
 * @param q Ponteiro para a Query.
 *
 * @return String com a descrição da query.
 */
const char* get_query_descricao(const Query* q);

/**
 * @brief Indica se a query permite o modificador 'S'.
 *
 * O modificador 'S' altera o formato do output da query.
 *
 * @param q Ponteiro para a Query.
 *
 * @return 1 se permitir 'S', 0 caso contrário.
 */
int get_query_permite_s(const Query* q);

/**
 * @brief Obtém o número de argumentos associados a uma query.
 *
 * @param q Ponteiro para a Query.
 *
 * @return Número de argumentos da query.
 */
int get_query_num_args(const Query* q);

/**
 * @brief Obtém um argumento específico de uma query.
 *
 * @param q Ponteiro para a Query.
 * @param index Índice do argumento (0 <= index < número de argumentos).
 *
 * @return Ponteiro para o QueryArg correspondente ou NULL se inválido.
 */
const QueryArg* get_query_arg_at(const Query* q, int index);

/**
 * @brief Obtém o nome simbólico de um argumento.
 *
 * @param arg Ponteiro para o argumento da query.
 *
 * @return String com o nome do argumento.
 */
const char* get_arg_nome(const QueryArg* arg);

/**
 * @brief Indica se um argumento é obrigatório.
 *
 * @param arg Ponteiro para o argumento da query.
 *
 * @return 1 se for obrigatório, 0 se for opcional.
 */
int get_arg_obrigatorio(const QueryArg* arg);

#endif
