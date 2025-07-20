#include <asm-generic/errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "./table.c"

int create_file(char* file_name, char** contents, size_t size) {
    FILE *file_handle;

    file_handle = fopen(file_name, "w+");
    size_t witten = fwrite(*contents, sizeof(char), size, file_handle);

    if (witten != size) {
      perror("Erro ao escrever no arquivo");
      return 1;
    }

    fclose(file_handle);
    return 0;
}
size_t read_file(char* file_name, char** file_buffer) {
    FILE *file_handle;
    size_t file_size;

    file_handle = fopen(file_name, "r");
    if(file_handle == NULL) {
        printf("Could not open the file to read contents");
        return -1;
    }

    fseek(file_handle, 0, SEEK_END);
    file_size = ftell(file_handle);
    fseek(file_handle, 0, SEEK_SET);
    *file_buffer = malloc(file_size);

    size_t n = 0;
    int c;
    while ((c = fgetc(file_handle)) != EOF) {
        (*file_buffer)[n] = (char)c;
        n++;
    }

    (*file_buffer)[n] = '\0';
    fclose(file_handle);
    return file_size;
}

int main (void) {
    char* raw_table = NULL;
    size_t raw_table_size = read_file("data", &raw_table);
    TableAscii table = {0};
    if (raw_table_size == -1)
        return 1;

    size_t line_number =  table_line_number(raw_table, raw_table_size);
    size_t biggest_word = table_biggest_word_size(raw_table, raw_table_size);
    size_t biggest_word_line = table_biggest_wordline_num(raw_table, raw_table_size);

    int l = ((line_number * 2) + 1);
    size_t chars = l * (biggest_word * biggest_word_line);
    table = (TableAscii) {
        .buffer = malloc(sizeof(char) * chars),
        .size = chars,
        .biggest_word = biggest_word,
        .num_words = biggest_word_line
    };

    table_transform_data(&table, raw_table);
    printf("table:\n%s\n", table.buffer);

    return 0;
    int index;
    int current_word = 0;
    int current_line = 0;
    int biggest_word_size = 0;
    int biggest_line_size = 0;
    int line_count = 0;
    for (int i = 0; i < raw_table_size; i++) {
        current_line++;
        if (raw_table[i] == '\n') {
            line_count++;
            if (current_line > biggest_line_size) {
                biggest_line_size = current_line - 1;
            }
            current_line = 0;
        }

        if (raw_table[i] != ',' && raw_table[i] != '\n' && raw_table[i] != EOF && raw_table[i] != ',') {
            current_word += 1;
        } else if (raw_table[i] == ',') {
            if(current_word >= biggest_word_size) {
                biggest_word_size = current_word;
                index = i - current_word;
            }
            current_word = 0;
        }
    }

    int in_word_line_count = 0;
    int out_word_line_count = 0;
    for (int i = 0; i < raw_table_size; i++) {
        if (raw_table[i] != ',' && raw_table[i] != '\n' && raw_table[i] != EOF && raw_table[i] != ',') {
            current_word += 1;
            printf("%c", raw_table[i]);
        } else if (raw_table[i] == ',') {
            in_word_line_count++;
            out_word_line_count++;
            for (int j = 0; j < biggest_word_size - (current_word) ; j++) {
                printf(" ");
            }
            printf("|");
            current_word = 0;
        } else if (raw_table[i] == '\n') {
            for (int j = 0; j < biggest_word_size - (current_word) ; j++) {
                printf(" ");
            }
            printf("|");
        }

        if (raw_table[i] == '\n') {
            printf("%c", raw_table[i]);
            for (int line = 0; line < (biggest_word_size * (in_word_line_count + 1) + in_word_line_count); line++) {
                printf("-");
            }
            printf("\n");
            in_word_line_count = 0;
        }
    }

    create_file("datato", &raw_table, raw_table_size);
    return 0;
}
