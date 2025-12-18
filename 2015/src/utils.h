#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int get_input(const char *file_name, char **buffer);
int get_file_size(FILE *file_ptr, int *file_size);
int read_file_content(FILE *file_ptr, char *buffer, int read_size);

inline int get_input(const char* file_name, char** buffer){

    if (file_name == NULL){
        fprintf(stderr, "File name should not be NULL\n");
        return -1;
    }

    if (buffer == NULL){
        fprintf(stderr, "Buffer reference should not be null\n");
        return -1;
    }

    FILE* file_ptr = fopen(file_name, "r");

    if (file_ptr == NULL){
        int error_code = errno;
        fprintf(stderr, "File (%s) could not open: %s (%d) \n",
            file_name, strerror(error_code), error_code);
        return -1;
    }

    int file_size;
    if(get_file_size(file_ptr, &file_size) != 0){
        fclose(file_ptr);
        return -1;
    }

    char* file_content_buffer = (char*) malloc(file_size + 1);

    if (file_content_buffer == NULL){
        fclose(file_ptr);
        fprintf(stderr, "Malloc failed :D\n");
        return -1;
    }

    memset(file_content_buffer, 0, file_size + 1);
    if (read_file_content(file_ptr, file_content_buffer, file_size) != 0){
        fclose(file_ptr);
        return -1;
    }

    *buffer = file_content_buffer;
    fclose(file_ptr);
    return 0;
}

inline int get_file_size(FILE* file_ptr, int* file_size){
    if (file_ptr == NULL){
        fprintf(stderr, "File ptr should not be NULL\n");
        return -1;
    }

    long original_file_offset = ftell(file_ptr);

    if (fseek(file_ptr, 0, SEEK_END) != 0){
        int error_code = errno;
        fprintf(stderr, "fseek failed when trying to get to the end: %s (%d)\n",
            strerror(error_code), error_code);
        return error_code;
    }

    long file_ending_offset =  ftell(file_ptr);
    *file_size = file_ending_offset;

    if (fseek(file_ptr, original_file_offset, SEEK_SET) != 0){
        int error_code = errno;
        fprintf(stderr, "fseek failed when trying to reset the position: %s (%d)\n",
            strerror(error_code), error_code);
        return error_code;
    }

    return 0;
}

inline int read_file_content(FILE* file_ptr, char* buffer, int read_size){
    if (file_ptr == NULL){
        fprintf(stderr, "File ptr should not be NULL\n");
        return -1;
    }

    if (buffer == NULL){
        fprintf(stderr, "buffer should not be NULL\n");
        return -1;
    }

    // Note buffer should be read_size + 1
    size_t read_count = fread(buffer, sizeof *buffer, read_size, file_ptr);

    if(read_count != read_size){
        if(feof(file_ptr)){
            fprintf(stderr, "EOF reached while reading from the file\n");
            return -1;
        }
        if (ferror(file_ptr)){
            int error_code = errno;
            fprintf(stderr, "fread failed when trying to read from the file: %s (%d)\n",
                strerror(error_code), error_code);
            return error_code;
        }
    }

    return 0;
}
