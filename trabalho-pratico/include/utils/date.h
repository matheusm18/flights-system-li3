#ifndef DATE_H
#define DATE_H

/**
 * @brief Converte uma string de data no formato YYYY-MM-DD para um inteiro numérico.
 * 
 * Esta função converte uma data no formato YYYY-MM-DD para um
 * inteiro no formato YYYYMMDD, facilitando comparações numéricas entre datas.
 * A string "N/A" é tratada como um caso especial e retorna -1.
 * 
 * @param str String que contêm a data no formato YYYY-MM-DD ou "N/A".
 * 
 * @return Um inteiro no formato YYYYMMDD que representa a data (ex: 20250315
 *         para 2025-03-15), ou -1 se a string for NULL ou "N/A".
 */
int string_to_date(const char* str);

/**
 * @brief Converte uma string de data e hora no formato YYYY-MM-DD HH:MM para um inteiro longo.
 * 
 * Esta função converte uma data e hora no formato "YYYY-MM-DD HH:MM" para um
 * inteiro longo no formato YYYYMMDDHHmm, facilitando comparações numéricas entre
 * datas e horas. A string "N/A" é tratada como um caso especial e retorna -1.
 * 
 * @param str String que contêm a data e hora no formato "YYYY-MM-DD HH:MM" ou "N/A".
 * 
 * @return Um long no formato YYYYMMDDHHmm que representa a data e hora (ex: 202503151430
 *         para 2025-03-15 14:30), ou -1 se a string for NULL ou "N/A".
 */
long string_to_datetime(const char* str);

/**
 * @brief Extrai a parte da data de um datetime no formato numérico YYYYMMDDHHmm.
 * 
 * Esta função extrai apenas a componente de data (YYYYMMDD) de um valor datetime
 * longo que inclui data e hora (YYYYMMDDHHmm), descartando as componentes de
 * hora e minuto através de divisão inteira.
 * 
 * @param datetime Valor long que representa a data e hora no formato YYYYMMDDHHmm
 *                 (ex: 202503151430 para 2025-03-15 14:30).
 * 
 * @return Um inteiro no formato YYYYMMDD que representa apenas a data
 *         (ex: 20250315 para 2025-03-15).
 * 
 */
int get_date_part(long datetime);

/**
 * @brief Compara duas datas no formato numérico YYYYMMDD.
 * 
 * Esta função compara duas datas representadas como inteiros no formato YYYYMMDD
 * e retorna a diferença entre elas. O resultado pode ser usado para determinar
 * qual data é a mais recente ou se são iguais.
 * 
 * @param date1 Primeira data no formato numérico YYYYMMDD.
 * @param date2 Segunda data no formato numérico YYYYMMDD.
 * 
 * @return Um valor negativo se date1 é anterior a date2,
 *         zero se as datas são iguais,
 *         ou um valor positivo se date1 é posterior a date2.
 * 
 */
int compare_dates(int date1, int date2);

/**
 * @brief Compara duas datas e horas no formato numérico YYYYMMDDHHmm.
 * 
 * Esta função compara duas datas e horas representadas como inteiros longos
 * no formato YYYYMMDDHHmm e retorna um valor indicando qual é mais recente
 * ou se são iguais.
 * 
 * @param dt1 Primeira data e hora no formato numérico YYYYMMDDHHmm.
 * @param dt2 Segunda data e hora no formato numérico YYYYMMDDHHmm.
 * 
 * @return -1 se dt1 é anterior a dt2,
 *         0 se as datas e horas são iguais,
 *         1 se dt1 é posterior a dt2.
 * 
 * @see string_to_datetime
 */
int compare_datetimes(long dt1, long dt2);

#endif