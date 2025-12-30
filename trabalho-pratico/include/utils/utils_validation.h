#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

/**
 * @brief Converte um inteiro em string.
 * 
 * @param value Valor inteiro
 * @return String alocada dinamicamente contendo o número
 */
char* int_to_string(int value);

/**
 * @brief Converte um double em string com 3 casas decimais.
 * 
 * @param valor Valor double
 * @return String alocada dinamicamente representando o valor
 */
char* double_to_string(double valor);

/**
 * @brief Converte uma data no formato YYYYMMDD para string "YYYY-MM-DD".
 * 
 * @param date Data no formato YYYYMMDD
 * @return String alocada dinamicamente
 */
char* date_to_string(int date);

/**
 * @brief Valida se a string representa uma data válida no formato "YYYY-MM-DD".
 * 
 * @param date String da data
 * @return true se válida, false caso contrário
 */
bool validate_date(const char* date);

/**
 * @brief Valida se a string representa um horário válido no formato "HH:MM".
 * 
 * @param time String do horário
 * @return true se válida, false caso contrário
 */
bool validate_time(const char* time);

/**
 * @brief Valida se a string representa uma data e hora válida no formato "YYYY-MM-DD HH:MM" ou "N/A".
 * 
 * @param datetime String da data/hora
 * @return true se válida, false caso contrário
 */
bool validate_datetime(const char* datetime);

/**
 * @brief Converte uma string de data "YYYY-MM-DD" para inteiro YYYYMMDD.
 * 
 * @param str String da data
 * @return Inteiro representando a data ou -1 para "N/A"
 */
int string_to_date(const char* str);

/**
 * @brief Converte uma string de data e hora "YYYY-MM-DD HH:MM" para long.
 * 
 * @param str String de data e hora
 * @return Long representando a data e hora ou -1 para "N/A"
 */
long string_to_datetime(const char* str);

/**
 * @brief Obtém apenas a parte da data de um valor datetime longo.
 * 
 * @param datetime Valor datetime
 * @return Parte da data (YYYYMMDD)
 */
int get_date_part(long datetime);

/**
 * @brief Compara duas datas no formato YYYYMMDD.
 * 
 * @param date1 Primeira data
 * @param date2 Segunda data
 * @return Valor negativo, zero ou positivo conforme date1 <, =, > date2
 */
int compare_dates(int date1, int date2);

/**
 * @brief Compara dois datetime longs.
 * 
 * @param dt1 Primeiro datetime
 * @param dt2 Segundo datetime
 * @return -1 se dt1 < dt2, 0 se iguais, 1 se dt1 > dt2
 */
int compare_datetimes(long dt1, long dt2);

/**
 * @brief Converte um datetime longo em minutos desde o ano 0.
 * 
 * @param date Datetime longo
 * @return Total de minutos
 */
long datetime_to_minutes(long date);

/**
 * @brief Calcula o atraso de um voo em minutos.
 * 
 * @param actual_departure Horário real de partida
 * @param expected_departure Horário previsto de partida
 * @return Diferença em minutos (pode ser negativa)
 */
int get_flight_delay(long actual_departure, long expected_departure);

/**
 * @brief Converte string "true"/"false" para booleano.
 * 
 * @param str String a converter
 * @return true se "true" (case-insensitive), false caso contrário
 */
bool string_to_bool(const char *str);

/**
 * @brief Valida se a string representa um inteiro positivo (>0).
 * 
 * @param str String a validar
 * @return true se válida, false caso contrário
 */
bool validate_int_pos(const char *str);


/**
 * @brief Valida a estrutura de parênteses em uma string.
 *
 * Esta função verifica se os parênteses na string seguem algumas regras específicas:
 * - Não permite parênteses aninhados: '(' dentro de outro '(' é inválido.
 * - Não permite parênteses vazios: "()" é inválido.
 * - Cada '(' deve ter um ')' correspondente.
 * - Um '(' não pode ser o último caractere da string.
 * - Um ')' não pode aparecer sem um '(' correspondente.
 *
 * @param str Ponteiro para a string a ser validada.
 * @param len Comprimento da string.
 * @return true Se todos os parênteses estiverem corretamente balanceados e válidos.
 * @return false Se algum parêntese violar as regras acima.
 *
 */
bool validate_parentheses(const char *str, int len);

/**
 * @brief Valida caracteres especiais básicos em uma string.
 *
 * Esta função verifica se uma string atende a algumas regras básicas de formatação:
 * - Não pode começar com espaço, '&' ou ')'.
 * - Não pode terminar com espaço, '&' ou '('.
 * - Não pode conter espaços duplos consecutivos.
 * - Não pode conter '&&' consecutivos.
 *
 * @param str Ponteiro para a string a ser validada.
 * @param len Comprimento da string.
 * @return true Se a string passar todas as validações.
 * @return false Se a string violar qualquer uma das regras acima.
 *
 */
bool validate_basic_special_chars(const char *str, int len);



#endif
