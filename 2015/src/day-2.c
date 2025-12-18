#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

#define MIN(x, y) ((x) < (y)) ? (x) : (y)

int main(int argc, const char** argv){

    // SETUP
    if (argc != 2){
        fprintf(stderr, "Argument count is wrong (%d), should be 2\n", argc);
        return -1;
    }

    const char* file_name = argv[1];
    char* file_content_buffer = NULL;

    if(get_input(file_name, &file_content_buffer) != 0){
        free(file_content_buffer);
        return -1;
    }

    size_t content_len = strlen(file_content_buffer);

    char* content_copy = malloc(content_len + 1);
    if (content_copy == NULL){
        fprintf(stderr, "Malloc failed :D\n");
        return -1;
    }

    // I want to preserve the original string (I don't know why!)
    memcpy(content_copy, file_content_buffer, content_len + 1);

    // ALGORITHM
    uint64_t sum = 0;
    const char* delimeter = "\n";
    char* token = strtok(content_copy, delimeter);
    while (token != NULL) {
        int length, width, height;
        if (sscanf(token, "%dx%dx%d", &length, &width, &height) != 3){
            fprintf(stderr, "Scanf was not succesfull!\n");
            free(file_content_buffer);
            free(content_copy);
            return -1;
        }

        // printf("%dx%dx%d\n", length, width, height);
        //

        int side_1 = length * height;
        int side_2 = length * width;
        int side_3 = width * height;

        int min_side = MIN(side_1, MIN(side_2, side_3));

        sum += ((2 * side_1)
            + (2 * side_2)
            + (2 * side_3))
            + min_side;

        token = strtok(NULL, delimeter);
    }

    printf("The sum of the area is: %lu\n", sum);

    free(file_content_buffer);
    free(content_copy);
    return 0;
}
