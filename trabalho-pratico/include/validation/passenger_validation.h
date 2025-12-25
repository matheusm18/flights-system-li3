#ifndef PASSENGER_VALIDATION_H
#define PASSENGER_VALIDATION_H

#include <stdbool.h>

/**
 * @brief Valida se uma string representa um número de documento de passageiro válido.
 * 
 * Esta função verifica se o número de documento fornecido é válido, seguindo
 * o formato esperado de exatamente 9 dígitos numéricos. Todos os caracteres
 * devem ser dígitos entre 0 e 9.
 * 
 * @param document_number String contendo o número de documento a ser validado.
 * 
 * @return true se o número de documento é válido (exatamente 9 dígitos numéricos).
 * @return false se o número de documento é NULL, não tem 9 caracteres, ou contém
 *         caracteres não numéricos.
 */
bool validate_passenger_document_number(const char *document_number);

/**
 * @brief Valida se uma string representa um género válido de passageiro.
 * 
 * Esta função verifica se o género fornecido corresponde a um dos valores
 * aceites pelo sistema: "M" (Masculino), "F" (Feminino) ou "O" (Outro).
 * 
 * @param gender String contendo o género a ser validado.
 * 
 * @return true se o género é válido ("M", "F" ou "O").
 * @return false se o género é NULL ou não corresponde a nenhum dos valores aceites.
 */
bool validate_passenger_gender(const char *gender);

/**
 * @brief Valida se uma string representa um endereço de email válido.
 * 
 * Esta função verifica se o email fornecido segue um formato válido simplificado.
 * O formato aceite é: username@domain, onde:
 * - username: corresponde a um conjunto de carateres do intervalo [a-z0-9] podendo incluir o carácter '.'
 * - domain: com formato <lstring>.<rstring>, onde lstring corresponde a um conjunto de 1 ou mais caracteres, e 
 * onde rstring corresponde a um conjunto de 2 ou 3 caracteres.Todos os carateres de lstring e rstring deverão pertencer ao intervalo
 * [a-z].
 * 
 * @param email String que contêm o endereço de email a ser validado.
 * 
 * @return true se o email é válido (contém @, username válido, domínio com ponto,
 *         e extensão de 2-3 caracteres).
 * @return false se o email é NULL, não contém @, tem username ou domínio inválidos,
 *         não tem ponto no domínio, ou a extensão tem tamanho incorreto.
 */
bool validate_passenger_email(const char *email);

/**
 * @brief Valida se uma string representa uma data de nascimento válida de passageiro.
 * 
 * Esta função verifica se a data de nascimento fornecida é válida, delegando
 * a validação para a função validate_date que verifica o formato YYYY-MM-DD
 * e a validade da data.
 * 
 * @param dob String que contêm a data de nascimento a ser validada no formato YYYY-MM-DD.
 * 
 * @return true se a data de nascimento é válida (formato e componentes corretos).
 * @return false se a data de nascimento é inválida.
 * 
 * @see validate_date
 */
bool validate_passenger_birth_date(const char *dob);

#endif

