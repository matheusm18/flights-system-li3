#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv) {
    
    // tres args visto que ele considera o nome do programa como primeiro arg
    if (argc != 4) {
        fprintf(stderr, "Usage format: %s <csv_files_path> <input_path> <expected_output_path>\n", argv[0]);
        return 1;
    }

    const char* csv_files_path = argv[1];
    const char* input_path = argv[2];
    const char* expected_output_path = argv[3];

    return 0;
}