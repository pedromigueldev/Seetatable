#include <stdio.h>
#include <signal.h>
#include <asm-generic/errno.h>
#include <stddef.h>

typedef struct {
    size_t size;
    char* buffer;
    size_t biggest_word;
    size_t num_words
} TableAscii;

size_t table_create_file (char* table, char* table_name);
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
size_t table_biggest_wordline_num (char* table_raw, size_t table_raw_size) {
    int wprln = 0;
    int current_count = 0;

    if (table_raw == NULL) {
        raise(EADDRNOTAVAIL);
        return -1;
    }

    for (int i = 0; i < table_raw_size; i++) {
        if (table_raw[i] == '\n') {
            if (current_count > wprln) {
                wprln = current_count;
            }
            current_count = 0;
            continue;
        }

        if (table_raw[i] == ',') {
            current_count++;
        }
    }

    return wprln + 1;
};
size_t table_line_number(char* table_raw, size_t table_raw_size) {
    int line_number = 0;

    if (table_raw == NULL) {
        raise(EADDRNOTAVAIL);
        return -1;
    }

    for (int i = 0; i < table_raw_size; i++) {
        if (table_raw[i] == '\n') {
            line_number++;
        }
    }

    return line_number;
}
int table_transform_data(TableAscii* table, char* raw_table) {
    int raw_index = 0;
    int current_word_size = 0;

    for (int i = 0; i < table->size; i++) {
        if (raw_table[raw_index] != ',' && raw_table[raw_index] != '\n')
            table->buffer[i] = raw_table[raw_index];

        if (raw_table[raw_index] == ',') {
            int remain = table->biggest_word - current_word_size+1;
            for (int k = 0; k < remain; k++) {
                table->buffer[i + k] = ' ';
            }
            i += remain;
            table->buffer[i] = '|';
            current_word_size = 0;
        }
        if (raw_table[raw_index] == '\n') {
            int remain = table->biggest_word - current_word_size+1;
            for (int k = 0; k < remain; k++) {
                table->buffer[i + k] = ' ';
            }
            i += remain;
            table->buffer[i] = '|';
            i++;
            table->buffer[i] = '\n';
            current_word_size = 0;
            raw_index++;
            continue;
        }
        current_word_size++;
        raw_index++;
    }
    return 0;
}
