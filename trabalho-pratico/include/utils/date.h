#ifndef DATE_H
#define DATE_H

typedef struct date Date;
typedef struct datetime DateTime;

Date* date_create(int year, int month, int day);

void date_destroy(Date* date);

int date_compare(const Date* date1, const Date* date2);

#endif