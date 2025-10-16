#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 1024
#define MAX_FIELDS 8

void remover_aspas(char *s){
    if (!s) return;
    size_t len = strlen(s);
    if (len >= 2 && s[0] == '"' && s[len - 1] == '"') {
        memmove(s, s + 1, len - 2);
        s[len - 2] = '\0';
    }
}

void ler_csv(const char *filename, void (*callback)(char **campos, int num_campos)){
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Erro ao abrir ficheiro");
        return;
    }
    char linha[MAX_LINE_LENGTH];
    int primeira_linha = 1;
    while (fgets(linha, sizeof(linha), fp)){
        if (primeira_linha) {
            primeira_linha = 0;
            continue;
        }
        linha[strcspn(linha, "\r\n")] = '\0';
        char *campos[MAX_FIELDS];
        int num_campos = 0;
        char *token = strtok(linha, ",");
        while (token && num_campos < MAX_FIELDS){
            remover_aspas(token);
            campos[num_campos++] = token;
            token = strtok(NULL, ",");
        }
        if (callback)
            callback(campos, num_campos);
    }
    fclose(fp);
}

bool valida_code_airport(const char *codigo){
    if (!codigo || strlen(codigo) != 3) return false;
    for (int i = 0; i < 3; i++) {
        if (codigo[i] < 'A' || codigo[i] > 'Z') return false;
    }
    return true;
}

bool valida_name_city_airport(const char *nome){
    if (!nome) return false;
    for (int i = 0; nome[i]; i++) {
        char c = nome[i];
        if (!isalpha(c) && c != ' ' && c != '-' && c != '\'') return false;
    }
    return true;
}

bool valida_country_airport(const char *pais){
    if (!pais || strlen(pais) != 2) return false;
    for (int i = 0; i < 2; i++) {
        if (pais[i] < 'A' || pais[i] > 'Z') return false;
    }
    return true;
}

bool valida_latitude_airport(double latitude){
    return latitude >= -90 && latitude <= 90;
}

bool valida_longitude_airport(double longitude){
    return longitude >= -180 && longitude <= 180;
}

bool valida_icao_airport(const char *icao){
    if (!icao || strlen(icao) != 4) return false;
    for (int i = 0; i < 4; i++) {
        if (icao[i] < 'A' || icao[i] > 'Z') return false;
    }
    return true;
}

bool valida_type_airport(const char *type){
    if (!type) return false;
    return (strcmp(type, "small_airport") == 0 ||
            strcmp(type, "medium_airport") == 0 ||
            strcmp(type, "large_airport") == 0);
}


void processar_linha_valida(char **campos, int num_campos){
    char *codigo = campos[0];
    char *nome = campos[1];
    char *cidade = campos[2];
    char *pais = campos[3];
    double latitude = atof(campos[4]);
    double longitude = atof(campos[5]);
    char *icao = campos[6];
    char *type = campos[7];

    if (!valida_code_airport(codigo)){
        printf("Aeroporto descartado: código '%s' inválido\n", codigo);
        return;
    }
    if (!valida_name_city_airport(nome)){
        printf("Aeroporto descartado: nome '%s' inválido\n", nome);
        return;
    }
    if (!valida_name_city_airport(cidade)){
        printf("Aeroporto descartado: cidade '%s' inválida\n", cidade);
        return;
    }
    if (!valida_country_airport(pais)){
        printf("Aeroporto descartado: país '%s' inválido\n", pais);
        return;
    }
    if (!valida_latitude_airport(latitude)){
        printf("Aeroporto descartado: latitude '%f' inválida\n", latitude);
        return;
    }
    if (!valida_longitude_airport(longitude)){
        printf("Aeroporto descartado: longitude '%f' inválida\n", longitude);
        return;
    }
    if (!valida_icao_airport(icao)){
        printf("Aeroporto descartado: icao '%s' inválida\n", icao);
        return;
    }
    if (!valida_type_airport(type)){
        printf("Aeroporto descartado: type '%s' inválida\n", type);
        return;
    }
}

