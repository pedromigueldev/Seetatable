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
      perror("An error ocurrend while writing file\n");
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
        printf("Could not open the file to read contents\n");
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

int main (int argc, char* argv[]) {
    printf("Reading file: %s\n", argv[1]);

    char* raw_table = NULL;
    size_t raw_table_size = read_file(argv[1], &raw_table);
    TableAscii table = {0};
    if (raw_table_size == -1)
        return 1;

    size_t line_number =  table_line_number(raw_table, raw_table_size);
    size_t biggest_word = table_biggest_word_size(raw_table, raw_table_size);
    size_t biggest_word_line = table_biggest_wordline_num(raw_table, raw_table_size);

    int bars = (biggest_word_line+1) * line_number;
    size_t chars = (line_number * (biggest_word_line * biggest_word)) + bars;
    table = (TableAscii) {
        .buffer = calloc(chars, sizeof(char)),
        .size = chars,
        .biggest_word = biggest_word,
        .num_words = biggest_word_line
    };

    table_transform_data(&table, raw_table);
    printf("writing to table: %s----------\n%s\n", argv[2], table.buffer);
    create_file(argv[2], &table.buffer, table.size);

    free(raw_table);
    free(table.buffer);
    return 0;
}
