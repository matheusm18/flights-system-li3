#include <utils/date.h>
#include <stdio.h>
#include <stdlib.h>

struct date {
    int year;
    int month;
    int day;
};

struct datetime {
    struct date date_part;
    int hour;
    int minute;
    int second;
};

Date* date_create(int year, int month, int day) {
    Date* date = malloc(sizeof(Date));
    date->year = year;
    date->month = month; 
    date->day = day;
    
    return date;
}

void date_destroy(Date* date) {
    free(date);
}

int date_compare(const Date* date1, const Date* date2) {
}