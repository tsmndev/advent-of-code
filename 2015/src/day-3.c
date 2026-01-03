#include "utils.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

struct location_point{
    int32_t x;
    int32_t y;
};

#define REGISTER_ZMAP_TYPES(X)      \
    X(struct location_point, int, locations)     \

#include <zdk/zmap.h>

uint32_t hash_location_point(const struct location_point point, uint32_t seed){
    return ZMAP_HASH_SCALAR(point, seed);
}

int comp_location_point(const struct location_point point1, const struct location_point point2){
    uint64_t point1_as_uint = *((uint64_t*) &point1);
    uint64_t point2_as_uint = *((uint64_t*) &point2);
    return (int) (point1_as_uint - point2_as_uint);
}

int move_location_point(struct location_point* current_location, char direction);

# define ADVENT_DEBUG

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

    # ifdef ADVENT_DEBUG
    puts("Initializing the visited locations");
    # endif //ADVENT_DEBUG
    zmap_locations visited_locations = zmap_init_locations(hash_location_point, comp_location_point);

    struct location_point current_santa_location = { 0 };
    // Add the current location to the hash map with the value of 1 (visited one time at the beginning)
    zmap_put(&visited_locations, current_santa_location, 1);

    char* direction = file_content_buffer;
    while(*direction != '\0'){
        // Move current location
        int move_operation_error = move_location_point(&current_santa_location, *direction);

        // New line error (We simply ignore)
        if (move_operation_error == -1){
            ++direction;
            continue;
        }

        if(move_operation_error != 0){
            fprintf(stderr, "Unknown direction: (%c) aborting", *direction);
            free(file_content_buffer);
            zmap_free(&visited_locations);
            return -1;
        }

        int* visit_count = zmap_get(&visited_locations, current_santa_location);
        if (visit_count == NULL){
            zmap_put(&visited_locations, current_santa_location, 1);
        } else {
            zmap_put(&visited_locations, current_santa_location, (*visit_count) + 1);
        }

        ++direction;
    }

    printf("Santa Solo Visited count: %zu\n", zmap_size(&visited_locations));


    current_santa_location = (struct location_point){ 0 };
    struct location_point current_robot_location = { 0 };

    zmap_clear(&visited_locations);
    zmap_put(&visited_locations, current_santa_location, 2);

    int direction_len = strlen(file_content_buffer);
    direction = file_content_buffer;
    for (int i = 0; i < direction_len; ++i) {
        struct location_point* location_selection = (i % 2 == 0)
            ? &current_santa_location
            : &current_robot_location;

        // Move current location
        int move_operation_error = move_location_point(location_selection, *direction);

        // New line error (We simply ignore)
        if (move_operation_error == -1){
            ++direction;
            continue;
        }

        if(move_operation_error != 0){
            fprintf(stderr, "Unknown direction: (%c) aborting", *direction);
            free(file_content_buffer);
            zmap_free(&visited_locations);
            return -1;
        }

        int* visit_count = zmap_get(&visited_locations, *location_selection);
        if (visit_count == NULL){
            zmap_put(&visited_locations, *location_selection, 1);
        } else {
            zmap_put(&visited_locations, *location_selection, (*visit_count) + 1);
        }

        ++direction;
    }

    printf("Combined Visited count: %zu\n", zmap_size(&visited_locations));

    free(file_content_buffer);
    zmap_free(&visited_locations);
}

int move_location_point(struct location_point* current_location, char direction){
    switch (direction) {
        case '^':
            current_location->y += 1;
            break;
        case '>':
            current_location->x += 1;
            break;
        case 'v':
            current_location->y -= 1;
            break;
        case '<':
            current_location->x -= 1;
            break;
        case '\n':
            return -1;
        default:
            return -2;
    }
    return 0;
}
