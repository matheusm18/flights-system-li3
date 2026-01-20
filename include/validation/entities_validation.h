#ifndef VALIDATION_H
#define VALIDATION_H

#include <stdio.h>

/**
 * @brief Inicializa o ficheiro de erros de aeroportos.
 * 
 * Esta função cria (ou limpa se já existir) o ficheiro "airports_errors.csv"
 * no diretório "resultados/" e escreve o cabeçalho com os nomes das colunas.
 * O ficheiro será usado para registar aeroportos que apresentem erros de validação
 * durante o processamento.
 * 
 * @return void
 * 
 * @note Se ocorrer erro ao criar o ficheiro, uma mensagem de erro é impressa
 *       em stderr através de perror().
 */
void init_airports_errors_file();

/**
 * @brief Processa uma linha válida do ficheiro de aeroportos.
 * 
 * Esta função realiza validações (código, latitude, longitude e tipo), cria um objeto Airport e adiciona-o ao catálogo.
 * Se houver erros de validação, a linha é escrita no ficheiro de erros.
 * 
 * @param fields Array de strings que contêm os campos da linha CSV:
 *               [0] code, [1] name, [2] city, [3] country, [4] latitude,
 *               [5] longitude, [6] icao, [7] type.
 * @param num_fields Número de campos no array (não utilizado).
 * @param user_data Ponteiro para o CatalogManager que contém os catálogos.
 * @param errors_file Ponteiro para o ficheiro onde serão escritas as linhas
 *                    com erros de validação.
 * 
 * @return void
 * 
 * @see validate_code_airport
 * @see validate_latitude_airport
 * @see validate_longitude_airport
 * @see validate_type_airport
 * @see create_airport
 * @see airport_catalog_add
 */
void process_valid_line_airports(char **fields, int num_fields, void* user_data, FILE *errors_file);

/**
 * @brief Inicializa o ficheiro de erros de aeronaves.
 * 
 * Esta função cria (ou limpa se já existir) o ficheiro "aircrafts_errors.csv"
 * no diretório "resultados/" e escreve o cabeçalho com os nomes das colunas.
 * O ficheiro será usado para registar aeronaves que apresentem erros de validação
 * durante o processamento.
 * 
 * @return void
 * 
 * @note Se ocorrer erro ao criar o ficheiro, uma mensagem de erro é impressa
 *       em stderr através de perror().
 */
void init_aircrafts_errors_file();

/**
 * @brief Processa uma linha válida do ficheiro de aeronaves.
 * 
 * Esta função realiza validações, converte os campos para os tipos apropriados, cria um objeto Aircraft e
 * adiciona-o ao catálogo. Se houver erros de validação, a linha é escrita no ficheiro de erros.
 * 
 * @param fields Array de strings que contêm os campos da linha CSV:
 *               [0] identifier, [1] manufacturer, [2] model, [3] year,
 *               [4] capacity, [5] range.
 * @param num_fields Número de campos no array (não utilizado).
 * @param user_data Ponteiro para o CatalogManager que contém os catálogos.
 * @param errors_file Ponteiro para o ficheiro onde serão escritas as linhas
 *                    com erros de validação.
 * 
 * @return void
 * 
 * @see validate_year_aircraft
 * @see create_aircraft
 * @see aircraft_catalog_add
 */
void process_valid_line_aircrafts(char **fields, int num_fields, void* user_data, FILE *errors_file);

/**
 * @brief Inicializa o ficheiro de erros de voos.
 * 
 * Esta função cria (ou limpa se já existir) o ficheiro "flights_errors.csv"
 * no diretório "resultados/" e escreve o cabeçalho com os nomes das colunas.
 * O ficheiro será usado para registar voos que apresentem erros de validação
 * durante o processamento.
 * 
 * @return void
 * 
 * @note Se ocorrer erro ao criar o ficheiro, uma mensagem de erro é impressa
 *       em stderr através de perror().
 */
void init_flights_errors_file();

/**
 * @brief Processa uma linha válida do ficheiro de voos.
 * 
 * Esta função processa uma linha do ficheiro CSV de voos. Realiza validações sintáticas (formato de campos)
 * e lógicas (consistência de dados), converte datas para formato numérico, cria
 * um objeto Flight e adiciona-o ao catálogo. Se houver erros de validação,
 * a linha é escrita no ficheiro de erros.
 * 
 * @param fields Array de strings que contêm os campos da linha CSV:
 *               [0] flight_id, [1] departure, [2] actual_departure, [3] arrival,
 *               [4] actual_arrival, [5] gate, [6] status, [7] origin,
 *               [8] destination, [9] aircraft, [10] airline, [11] tracking_url.
 * @param num_fields Número de campos no array (não utilizado).
 * @param user_data Ponteiro para o CatalogManager que contém os catálogos.
 * @param errors_file Ponteiro para o ficheiro onde serão escritas as linhas
 *                    com erros de validação.
 * 
 * @return void
 * 
 * @see validate_flight_id_flight
 * @see validate_datetime
 * @see string_to_datetime
 * @see validate_flight_logical
 * @see create_flight
 * @see flight_catalog_add
 * @see aircrafts_counter_increment
 * @see airport_add_departing_flight
 */
// falta explicar a parte da logica que ajuda nas queries
void process_valid_line_flights(char **fields, int num_fields, void* user_data, FILE *errors_file);

/**
 * @brief Inicializa o ficheiro de erros de passageiros.
 * 
 * Esta função cria (ou limpa se já existir) o ficheiro "passengers_errors.csv"
 * no diretório "resultados/" e escreve o cabeçalho com os nomes das colunas.
 * O ficheiro será usado para registar passageiros que apresentem erros de validação
 * durante o processamento.
 * 
 * @return void
 * 
 * @note Se ocorrer erro ao criar o ficheiro, uma mensagem de erro é impressa
 *       em stderr através de perror().
 */
void init_passengers_errors_file();

/**
 * @brief Processa uma linha válida do ficheiro de passageiros.
 * 
 * Esta função processa uma linha do ficheiro CSV de passageiros que já passou
 * pela validação de formato básico. Realiza validações de número de documento,
 * email, género e data de nascimento. Atualmente, apenas o número de documento
 * é armazenado no catálogo para validação de reservas (implementação simplificada).
 * Se houver erros de validação, a linha é escrita no ficheiro de erros.
 * 
 * @param fields Array de strings que contêm os campos da linha CSV:
 *               [0] document_number, [1] first_name, [2] last_name, [3] dob,
 *               [4] nationality, [5] gender, [6] email, [7] phone,
 *               [8] address, [9] photo.
 * @param num_fields Número de campos no array (não utilizado).
 * @param user_data Ponteiro para o CatalogManager que contém os catálogos.
 * @param errors_file Ponteiro para o ficheiro onde serão escritas as linhas
 *                    com erros de validação.
 * 
 * @return void
 * 
 * @note Atualmente, apenas o número de documento é armazenado no catálogo.
 * 
 * @see validate_passenger_document_number
 * @see validate_passenger_email
 * @see validate_passenger_gender
 * @see validate_passenger_birth_date
 * @see passenger_catalog_add
 */
void process_valid_line_passengers(char **fields, int num_fields, void* user_data, FILE *errors_file);

/**
 * @brief Inicializa o ficheiro de erros de reservas.
 * 
 * Esta função cria (ou limpa se já existir) o ficheiro "reservations_errors.csv"
 * no diretório "resultados/" e escreve o cabeçalho com os nomes das colunas.
 * O ficheiro será usado para registar reservas que apresentem erros de validação
 * durante o processamento.
 * 
 * @return void
 * 
 * @note Se ocorrer erro ao criar o ficheiro, uma mensagem de erro é impressa
 *       em stderr através de perror().
 */
void init_reservations_errors_file();

/**
 * @brief Processa uma linha válida do ficheiro de reservas.
 * 
 * Esta função processa uma linha do ficheiro CSV de reservas que já passou
 * pela validação de formato básico. Realiza validações do ID da reserva,
 * dos IDs de voos (formato e existência no catálogo), do número de documento
 * (formato e existência no catálogo de passageiros). Se houver erros de validação,
 * a linha é escrita no ficheiro de erros. Atualmente, a criação do objeto Reservation
 * está comentada (implementação futura).
 * 
 * @param fields Array de strings contendo os campos da linha CSV:
 *               [0] reservation_id, [1] flight_ids, [2] document_number, [3] seat,
 *               [4] price, [5] extra_luggage, [6] priority_boarding, [7] qr_code.
 * @param num_fields Número de campos no array (não utilizado).
 * @param user_data Ponteiro para o CatalogManager que contém os catálogos.
 * @param errors_file Ponteiro para o ficheiro onde serão escritas as linhas
 *                    com erros de validação.
 * 
 * @return void
 * 
 * @note Atualmente, apenas as validações são realizadas. 
 * 
 * @see validate_reservation_id
 * @see validate_flight_ids_reservation
 * @see validate_passenger_document_number
 * @see validate_document_number_reservation
 */
void process_valid_line_reservations(char **fields, int num_fields, void* user_data, FILE *errors_file);

#endif