#include "io/output_writer.h"
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>


void write_result(const QueryResult* res, const char* output_path, char delimiter, bool in_ncurses, WINDOW* win) {
    if (res == NULL) return;

    FILE* f = NULL;
    if(!in_ncurses){
        if (output_path == NULL) {
            f = stdout;
        }
        else {
            f = fopen(output_path, "w");
            if (f == NULL) {
                perror("Erro ao abrir ficheiro de output");
                return;
            }
        }

    } else if (win != NULL){
        werase(win);   
        wmove(win, 2, 2);   
        wrefresh(win);
    }

    int total_lines = get_result_num_lines(res);
    
    if (total_lines == 0) {
        if(in_ncurses  && win!= NULL){
            wprintw(win, "\n");
        }
        else if (f != NULL) {
            fprintf(f, "\n");
        }
    }
    else {
        for (int i = 0; i < total_lines; i++) {
            int num_tokens = get_line_num_tokens(res, i);

            if(in_ncurses && win != NULL && num_tokens > 0){
                int current_line, current_col;
                getyx(win, current_line, current_col);
                (void)current_col;
                wmove(win, current_line, 2);  // voltar à coluna 2
            }
            
            if(num_tokens > 0) {
                for (int j = 0; j < num_tokens; j++) {
                    const char* token = get_result_token(res, i, j);
                    if (token != NULL) {
                        if(in_ncurses  && win!= NULL){
                            wprintw(win, "%s", token);
                        }
                        else if (f != NULL) {
                            fprintf(f, "%s", token);
                        }
                    }
                    // coloca o delimitador se NÃO for o ultimo token
                    if (j < num_tokens - 1) {
                        if(in_ncurses  && win!= NULL){
                            wprintw(win, "%c", delimiter);
                        }
                        else if (f != NULL) {
                            fprintf(f, "%c", delimiter);
                        }
                    }
                }
            }
            if(in_ncurses){
                    wprintw(win, "\n");
            }
            else if (f != NULL) {
                fprintf(f, "\n");
            } 
        }
    }
    if(in_ncurses && win!= NULL) {
        wrefresh(win);
    }
    else {
        if (f != NULL && output_path != NULL) {
        fclose(f);
    }

    }
}