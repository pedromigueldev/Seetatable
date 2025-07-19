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

int main (void) {
    char* file_buffer = NULL;
    size_t file_buffer_size = read_file("data", &file_buffer);





    create_file("datato", &file_buffer, file_buffer_size);
    printf("%s", file_buffer);
    return 0;
}
