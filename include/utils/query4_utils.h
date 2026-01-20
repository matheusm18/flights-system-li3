#ifndef QUERY4_UTILS_H
#define QUERY4_UTILS_H

#include <glib.h>

/**
 * @brief Verifica se um determinado ano é bissexto.
 * 
 * Um ano é bissexto se for divisível por 4, exceto os múltiplos de 100,
 * a menos que também seja múltiplo de 400.
 * 
 * @param y Ano (ex: 2024)
 * @return 1 se o ano é bissexto, 0 caso contrário
 */
int is_leap(int y);

/**
 * @brief Retorna o número de dias de um mês específico, ajustando para anos bissextos.
 * 
 * @param m Mês (1 = Janeiro, 2 = Fevereiro, ..., 12 = Dezembro)
 * @param y Ano (ex: 2024)
 * @return Número de dias no mês especificado
 */
int get_days_in_month_adj(int m, int y);

/**
 * @brief Calcula o dia da semana para uma dada data usando a fórmula de Zeller.
 * 
 * 0 = domingo, 1 = segunda-feira, ..., 6 = sábado
 * 
 * @param y Ano (ex: 2024)
 * @param m Mês (1..12)
 * @param d Dia do mês (1..31)
 * @return Inteiro representando o dia da semana
 */
int weekday(int y, int m, int d);

/**
 * @brief Converte uma data no formato YYYYMMDD para uma string representando
 *        o domingo da mesma semana (chave semanal).
 * 
 * @param date Data no formato YYYYMMDD (ex: 20241225)
 * @param buf Buffer de pelo menos 20 bytes onde será armazenada a string "YYYY-MM-DD"
 */
void date_to_week_key_buf(int date, char buf[20]);

#endif
