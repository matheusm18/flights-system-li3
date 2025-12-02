#ifndef DATE_VALIDATION_H
#define DATE_VALIDATION_H

#include <stdbool.h>

/**
 * @brief Valida se uma string representa uma data válida no formato YYYY-MM-DD.
 * 
 * A validação inclui verificar o formato da string, se os componentes são numéricos, se o mês está entre 1-12,
 * se o dia está entre 1-31, e se a data não é posterior a 2025-09-30.
 * 
 * @param date String contendo a data a ser validada no formato YYYY-MM-DD.
 * 
 * @return true se a data é válida (formato correto, componentes numéricos válidos,
 *         e não posterior a 2025-09-30).
 * @return false se a data é NULL, não tem 10 caracteres, não segue o formato
 *         YYYY-MM-DD, contém caracteres não numéricos, tem mês ou dia inválidos,
 *         ou é posterior a 2025-09-30.
 */
bool validate_date(const char* date);

/**
 * @brief Valida se uma string representa um horário válido no formato HH:MM.
 * 
 * A validação inclui verificar o formato da string, se os componentes são numéricos, 
 * se as horas estão entre 0-23, e se os minutos estão entre 0-59.
 * 
 * @param time String contendo o horário a ser validado no formato HH:MM.
 * 
 * @return true se o horário é válido (formato correto, horas entre 0-23,
 *         e minutos entre 0-59).
 * @return false se o horário é NULL, não tem 5 caracteres, não segue o formato
 *         HH:MM, contém caracteres não numéricos, ou tem horas/minutos fora
 *         do intervalo válido.
 */
bool validate_time(const char* time);

/**
 * @brief Valida se uma string representa uma data e hora válidas no formato YYYY-MM-DD HH:MM.
 * 
 * Esta função verifica se a string fornecida corresponde a uma data e hora válidas
 * no formato "YYYY-MM-DD HH:MM" ou se é a string especial "N/A" (que é aceita como
 * válida). A validação é feita separando os componentes de data e hora e validando
 * cada um individualmente.
 * 
 * @param datetime String contendo a data e hora a ser validada no formato 
 *                 "YYYY-MM-DD HH:MM" ou "N/A".
 * 
 * @return true se o datetime é válido (formato "YYYY-MM-DD HH:MM" correto com
 *         componentes válidos, ou a string "N/A").
 * @return false se o datetime é NULL, tem menos de 16 caracteres, não segue o
 *         formato esperado, ou se os componentes de data ou hora são inválidos.
 * 
 * @see validate_date
 * @see validate_time
 */
bool validate_datetime(const char* datetime);


#endif