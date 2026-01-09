#ifndef FLIGHT_VALIDATION_H
#define FLIGHT_VALIDATION_H

#include "catalog/aircraft_catalog.h"
#include <stdbool.h>

/**
 * @brief Valida se uma string representa um identificador de voo válido.
 * 
 * Esta função verifica se a string fornecida corresponde a um ID de voo válido,
 * que deve seguir o formato padrão de códigos de voo: duas letras maiúsculas
 * seguidas de cinco dígitos numéricos (ex: "AB12345").
 * 
 * @param flight_id String contendo o identificador de voo a ser validado.
 * 
 * @return true se o ID é válido (exatamente 7 caracteres, sendo 2 letras
 *         maiúsculas seguidas de 5 dígitos).
 * @return false se o ID é NULL, não tem 7 caracteres, ou não segue o formato
 *         esperado (2 letras maiúsculas + 5 dígitos).
 * 
 */
bool validate_flight_id_flight(const char *flight_id);

/**
 * @brief Valida se os códigos de origem e destino de um voo são válidos e diferentes.
 * 
 * Esta função verifica se os códigos de aeroporto de origem e destino são válidos
 * e se são diferentes entre si, garantindo que um voo não tenha o mesmo aeroporto como origem e destino.
 * 
 * @param origin String contendo o código do aeroporto de origem.
 * @param destination String contendo o código do aeroporto de destino.
 * 
 * @return true se ambos os códigos são válidos e diferentes entre si.
 * @return false se origin ou destination são NULL, se algum dos códigos não é válido,
 *         ou se origin e destination são iguais.
 * 
 * @see validate_code_airport
 */
bool validate_destination_origin(const char *origin, const char *destination);

/**
 * @brief Valida a consistência temporal entre datas de partida e chegada de um voo.
 * 
 * Esta função verifica se as datas e horas de um voo respeitam a ordem temporal
 * lógica, garantindo que a chegada seja sempre posterior à partida, tanto para
 * os horários programados quanto para os horários reais. Voos cancelados são
 * automaticamente considerados válidos.
 * 
 * @param status String que representa o status do voo (ex: "Cancelled").
 * @param departure Data e hora programada de partida no formato YYYYMMDDHHmm.
 * @param arrival Data e hora programada de chegada no formato YYYYMMDDHHmm.
 * @param actual_departure Data e hora real de partida no formato YYYYMMDDHHmm.
 * @param actual_arrival Data e hora real de chegada no formato YYYYMMDDHHmm.
 * 
 * @return true se o voo está cancelado ou se todas as chegadas são posteriores
 *         às respetivas partidas.
 * @return false se a chegada programada é anterior à partida programada, ou se
 *         a chegada real é anterior à partida real.
 * 
 * @see compare_datetimes
 */
bool validate_temporal_consistency(const char *status, long departure, long arrival, long actual_departure, long actual_arrival);

/**
 * @brief Valida se um voo cancelado tem horários reais corretamente marcados como "N/A".
 * 
 * Esta função verifica a consistência dos dados de voos cancelados, garantindo
 * que voos com status "Cancelled" tenham os horários de partida e chegada reais
 * marcados como "N/A". Se o voo não estiver cancelado, a validação é automaticamente
 * bem-sucedida.
 * 
 * @param status String contendo o status do voo.
 * @param actual_departure_str String contendo o horário real de partida.
 * @param actual_arrival_str String contendo o horário real de chegada.
 * 
 * @return true se o voo não está cancelado, ou se está cancelado e ambos os horários
 *         reais são "N/A".
 * @return false se o voo está cancelado mas algum dos horários reais não é "N/A" ou
 *         se as strings são NULL.
 */
bool validate_cancelled_status(const char *status, const char *actual_departure_str, const char *actual_arrival_str);

/**
 * @brief Valida se um voo atrasado tem horários reais posteriores aos programados.
 * 
 * Esta função verifica a consistência dos dados de voos atrasados, garantindo
 * que voos com status "Delayed" tenham os horários reais de partida e chegada
 * iguais ou posteriores aos horários programados. Se o voo não estiver atrasado,
 * a validação é automaticamente bem-sucedida.
 * 
 * @param status String que contêm o status do voo.
 * @param scheduled_departure Data e hora programada de partida no formato YYYYMMDDHHmm.
 * @param scheduled_arrival Data e hora programada de chegada no formato YYYYMMDDHHmm.
 * @param actual_departure Data e hora real de partida no formato YYYYMMDDHHmm.
 * @param actual_arrival Data e hora real de chegada no formato YYYYMMDDHHmm.
 * 
 * @return true se o voo não está atrasado, ou se está atrasado e os horários reais
 *         são iguais ou posteriores aos horários programados.
 * @return false se o voo está atrasado mas a partida real é anterior à programada,
 *         ou se a chegada real é anterior à programada.
 * 
 * @see compare_datetimes
 */
bool validate_delayed_status(const char *status, long scheduled_departure, long scheduled_arrival, long actual_departure, long actual_arrival);

/**
 * @brief Valida o status de um voo e a consistência dos seus horários.
 * 
 * Esta função realiza uma validação completa do status de um voo, verificando
 * se os dados associados ao status são consistentes. Os status aceites são
 * "On Time", "Delayed" e "Cancelled", cada um com suas regras específicas de
 * validação. Voos "On Time" são sempre válidos, voos "Delayed" devem ter
 * horários reais posteriores aos programados, e voos "Cancelled" devem ter
 * horários reais marcados como "N/A".
 * 
 * @param status String contendo o status do voo ("On Time", "Delayed" ou "Cancelled").
 * @param scheduled_departure Data e hora programada de partida no formato YYYYMMDDHHmm.
 * @param scheduled_arrival Data e hora programada de chegada no formato YYYYMMDDHHmm.
 * @param actual_departure Data e hora real de partida no formato YYYYMMDDHHmm.
 * @param actual_arrival Data e hora real de chegada no formato YYYYMMDDHHmm.
 * @param actual_departure_str String do horário real de partida (para validação de "N/A").
 * @param actual_arrival_str String do horário real de chegada (para validação de "N/A").
 * 
 * @return true se o status é válido e os dados são consistentes com o status.
 * @return false se o status é NULL, não é reconhecido, ou se os dados são
 *         inconsistentes com o status declarado.
 * 
 * @see validate_delayed_status
 * @see validate_cancelled_status
 */
bool validate_status_flight_new(const char *status, long scheduled_departure, long scheduled_arrival, long actual_departure, long actual_arrival,
                                const char *actual_departure_str, const char *actual_arrival_str);

/**
 * @brief Valida se um identificador de aeronave existe no catálogo.
 * 
 * Esta função verifica se o identificador de aeronave fornecido está registado
 * no catálogo de aeronaves, garantindo que apenas aeronaves conhecidas sejam
 * associadas a voos.
 * 
 * @param aircraft String contendo o identificador da aeronave a ser validado.
 * @param aircraft_catalog Ponteiro para o catálogo de aeronaves onde será
 *                         realizada a pesquisa.
 * 
 * @return true se a aeronave existe no catálogo.
 * @return false se a aeronave não existe no catálogo ou se não foi encontrada.
 * 
 * @see get_aircraft_by_identifier
 */
bool validate_aircraft_flight(const char *aircraft, AircraftCatalog* aircraft_catalog);

/**
 * @brief Realiza a validação lógica completa de um voo.
 * 
 * Esta função executa todas as validações lógicas necessárias para garantir
 * a consistência e integridade dos dados de um voo. As validações incluem:
 * verificar se origem e destino são diferentes, validar a consistência temporal
 * dos horários, confirmar a existência da aeronave no catálogo, e validar o
 * status do voo com suas restrições específicas.
 * 
 * @param origin String contendo o código do aeroporto de origem.
 * @param destination String contendo o código do aeroporto de destino.
 * @param departure Data e hora programada de partida no formato YYYYMMDDHHmm.
 * @param arrival Data e hora programada de chegada no formato YYYYMMDDHHmm.
 * @param actual_departure Data e hora real de partida no formato YYYYMMDDHHmm.
 * @param actual_arrival Data e hora real de chegada no formato YYYYMMDDHHmm.
 * @param status String contendo o status do voo ("On Time", "Delayed" ou "Cancelled").
 * @param aircraft String contendo o identificador da aeronave.
 * @param aircraft_catalog Ponteiro para o catálogo de aeronaves.
 * @param actual_departure_str String do horário real de partida (formato original).
 * @param actual_arrival_str String do horário real de chegada (formato original).
 * 
 * @return true se todas as validações lógicas são bem-sucedidas.
 * @return false se qualquer uma das validações falhar (origem igual ao destino,
 *         inconsistência temporal, aeronave inexistente, ou status inválido).
 * 
 * @see validate_destination_origin
 * @see validate_temporal_consistency
 * @see validate_aircraft_flight
 * @see validate_status_flight_new
 */
bool validate_flight_logical(const char *origin, const char *destination, long departure, long arrival, long actual_departure, long actual_arrival,
                             const char *status, const char *aircraft, AircraftCatalog* aircraft_catalog, const char *actual_departure_str,
                             const char *actual_arrival_str);

/**
 * @brief Verifica se o status do voo é "Cancelled".
 *
 * Compara a string de status com o valor "Cancelled".
 *
 * @param status String que representa o status do voo.
 *
 * @return true se o status for "Cancelled".
 * @return false caso contrário.
 */
bool is_canceled(char *status);

/**
 * @brief Verifica se o status do voo é "Delayed".
 *
 * Compara a string de status com o valor "Delayed".
 *
 * @param status String que representa o status do voo.
 *
 * @return true se o status for "Delayed".
 * @return false caso contrário.
 */
bool is_delayed(char *status);

#endif