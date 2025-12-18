#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

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

    // printf("The text content of the file: %s", file_content_buffer);

    // ALGORITHM
    int floor = 0;
    int character_pos_when_negative_one_floor = -1;
    int buffer_len = strlen(file_content_buffer);

    for(int i = 0; i < buffer_len; ++i){
        char buffer_char = file_content_buffer[i];
        switch (buffer_char) {
            case '(':
                floor += 1;
                break;
            case ')':
                floor -= 1;
                break;
            case '\n':
            case '\r':
                // Ignore whitespace
                break;
            default:
                fprintf(stderr, "Unknown char in file content: %c", buffer_char);
                return -1;
        }

        if (floor == -1 && character_pos_when_negative_one_floor == -1){
            character_pos_when_negative_one_floor = i + 1;
        }
    }

    printf("The final floor: %d\n", floor);
    printf("Character when the santa enters the negative one floor: %d\n", character_pos_when_negative_one_floor);

    free(file_content_buffer);
    return 0;
}
