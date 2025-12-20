#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

#define MIN(x, y) ((x) < (y)) ? (x) : (y)
#define MAX(x, y) ((x) > (y)) ? (x) : (y)

uint32_t calculate_paper(uint32_t height, uint32_t width, uint32_t length);
uint32_t calculate_ribbon(uint32_t height, uint32_t width, uint32_t length);

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
    uint64_t paper_sum = 0;
    uint64_t ribbon_sum = 0;
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

        paper_sum += calculate_paper(height, width, length);
        ribbon_sum += calculate_ribbon(height, width, length);

        token = strtok(NULL, delimeter);
    }

    printf("The sum of the area is: %lu\n", paper_sum);
    printf("The sum of the ribbon is: %lu\n", ribbon_sum);

    free(file_content_buffer);
    free(content_copy);
    return 0;
}

uint32_t calculate_paper(uint32_t height, uint32_t width, uint32_t length){

    uint32_t side_1 = length * height;
    uint32_t side_2 = length * width;
    uint32_t side_3 = width * height;

    uint32_t min_side = MIN(side_1, MIN(side_2, side_3));

    return ((2 * side_1)
        + (2 * side_2)
        + (2 * side_3))
        + min_side;
}

// This is probably an unoptimzied solition
// TODO: Look for how to get two of the smallest element effectively
uint32_t calculate_ribbon(uint32_t height, uint32_t width, uint32_t length){
    uint32_t wrap_len = 2 * (height + width + length);
    wrap_len -= 2 * (MAX(height, MAX(width, length)));

    wrap_len += height * width * length;
    return wrap_len;
}
