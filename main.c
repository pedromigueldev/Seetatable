#include <asm-generic/errno.h>
#include <errno.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

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

size_t table_biggest_word_size (char* table_raw, size_t table_raw_size) {
    int current_word_size = 0;
    int biggest_word_size = 0;
    int index = 0;

    if (table_raw == NULL) {
        raise(EADDRNOTAVAIL);
        return -1;
    }

    for (int i = 0; i < table_raw_size; i++) {
        if (table_raw[i] != ',' && table_raw[i] != '\n' && table_raw[i] != EOF)
            current_word_size += 1;

        if (table_raw[i] == ',') {
            if(current_word_size >= biggest_word_size) {
                biggest_word_size = current_word_size;
                index = i - current_word_size;
            }
            current_word_size = 0;
        }
    }

    return biggest_word_size;
}

size_t table_biggest_wordline_num (char* table, char* tbwln);
size_t table_create_file (char* table, char* table_name);

int main (void) {
    char* file_buffer = NULL;
    size_t file_buffer_size = read_file("data", &file_buffer);
    if (file_buffer_size == -1)
        return 1;

    char* table;
    int index;
    int current_word = 0;
    int current_line = 0;
    int biggest_word_size = 0;
    int biggest_line_size = 0;
    int line_count = 0;
    for (int i = 0; i < file_buffer_size; i++) {
        current_line++;
        if (file_buffer[i] == '\n') {
            line_count++;
            if (current_line > biggest_line_size) {
                biggest_line_size = current_line - 1;
            }
            current_line = 0;
        }

        if (file_buffer[i] != ',' && file_buffer[i] != '\n' && file_buffer[i] != EOF && file_buffer[i] != ',') {
            current_word += 1;
        } else if (file_buffer[i] == ',') {
            if(current_word >= biggest_word_size) {
                biggest_word_size = current_word;
                index = i - current_word;
            }
            current_word = 0;
        }
    }

    int in_word_line_count = 0;
    int out_word_line_count = 0;
    for (int i = 0; i < file_buffer_size; i++) {
        if (file_buffer[i] != ',' && file_buffer[i] != '\n' && file_buffer[i] != EOF && file_buffer[i] != ',') {
            current_word += 1;
            printf("%c", file_buffer[i]);
        } else if (file_buffer[i] == ',') {
            in_word_line_count++;
            out_word_line_count++;
            for (int j = 0; j < biggest_word_size - (current_word) ; j++) {
                printf(" ");
            }
            printf("|");
            current_word = 0;
        } else if (file_buffer[i] == '\n') {
            for (int j = 0; j < biggest_word_size - (current_word) ; j++) {
                printf(" ");
            }
            printf("|");
        }

        if (file_buffer[i] == '\n') {
            printf("%c", file_buffer[i]);
            for (int line = 0; line < (biggest_word_size * (in_word_line_count + 1) + in_word_line_count); line++) {
                printf("-");
            }
            printf("\n");
            in_word_line_count = 0;
        }
    }

    create_file("datato", &file_buffer, file_buffer_size);
    return 0;
}
