#ifndef MIN_HEAP_H
#define MIN_HEAP_H

/**
 * @struct aircraftcount
 * @brief Estrutura que guarda o identificador de uma aeronave e o número de voos associados.
 * 
 * Campos:
 *  - aircraft_id: Identificador único da aeronave (string alocada dinamicamente).
 *  - flight_count: Número de voos realizados por esta aeronave.
 */
typedef struct aircraftcount AircraftCount;

/**
 * @struct MinHeap
 * @brief Estrutura de heap mínima para armazenar as N aeronaves com mais voos.
 * 
 * Campos internos:
 *  - heap: Array de AircraftCount.
 *  - size: Número atual de elementos na heap.
 *  - capacity: Capacidade máxima da heap.
 */
typedef struct MinHeap MinHeap;

/**
 * @brief Cria uma MinHeap com capacidade especificada.
 * 
 * @param capacity Capacidade máxima da heap.
 * @return Ponteiro para a MinHeap criada ou NULL em caso de falha.
 */
MinHeap* create_min_heap(int capacity);

/**
 * @brief Liberta a memória associada a uma MinHeap.
 * 
 * @param h Ponteiro para a heap a destruir.
 */
void destroy_min_heap(MinHeap* h);

/**
 * @brief Insere um novo elemento (aeronave + número de voos) na MinHeap.
 * 
 * Se a heap estiver cheia, substitui o elemento mínimo se o novo tiver mais voos 
 * ou, em caso de empate, um identificador lexicograficamente menor.
 * 
 * @param h Ponteiro para a MinHeap.
 * @param aircraft_id Identificador da aeronave (string).
 * @param flight_count Número de voos.
 */
void insert_min_heap(MinHeap* h, const char* aircraft_id, int flight_count);

/**
 * @brief Retorna o número de elementos atualmente armazenados na MinHeap.
 * 
 * @param h Ponteiro para a MinHeap.
 * @return Número de elementos.
 */
int min_heap_size(const MinHeap* h);

/**
 * @brief Retorna o identificador da aeronave armazenado na posição especificada da MinHeap.
 * 
 * @param h Ponteiro para a MinHeap.
 * @param index Índice do elemento (0-based).
 * @return String com o identificador ou NULL se inválido.
 */
const char* get_aicraft_id_min_heap(const MinHeap* h, int index);

/**
 * @brief Retorna o número de voos da aeronave na posição especificada da MinHeap.
 * 
 * @param h Ponteiro para a MinHeap.
 * @param index Índice do elemento (0-based).
 * @return Número de voos ou 0 se inválido.
 */
int get_flight_count_min_heap(const MinHeap* h, int index);

/**
 * @brief Ordena os elementos da MinHeap em ordem decrescente de número de voos.
 * 
 * Em caso de empate, usa o identificador lexicográfico crescente.
 * 
 * @param h Ponteiro para a MinHeap.
 */
void sort_desc_min_heap(MinHeap* h);

#endif
