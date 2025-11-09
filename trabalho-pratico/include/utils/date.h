#ifndef DATE_H
#define DATE_H

int string_to_date(const char* str);
long string_to_datetime(const char* str);
int get_date_part(long datetime);
int compare_dates(int date1, int date2);
int compare_datetimes(long dt1, long dt2);

#endif