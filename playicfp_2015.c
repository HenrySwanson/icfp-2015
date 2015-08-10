#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <jansson.h>

#include "gameplay.h"

void parse_unit(unit* unit_array[], json_t* json_unit, int index) {
    unit* parsed_unit = new_unit(json_array_size(json_object_get(json_unit, "members")));

    json_t* cell_array = json_object_get(json_unit, "members");
    for (int i = 0; i < parsed_unit->num_cells; i++) {
        parsed_unit->cells_x[i] = json_number_value(json_object_get(json_array_get(cell_array, i), "x"));
        parsed_unit->cells_y[i] = json_number_value(json_object_get(json_array_get(cell_array, i), "y"));
    }

    json_t* pivot = json_object_get(json_unit, "pivot");
    parsed_unit->pivot_x = json_number_value(json_object_get(pivot, "x"));
    parsed_unit->pivot_y = json_number_value(json_object_get(pivot, "y"));

    unit_array[index] = parsed_unit;
}

void fill_board(board* board, json_t* filled_array) {
    int x, y;
    for (int i = 0; i < json_array_size(filled_array); i++) {
        x = json_number_value(json_object_get(json_array_get(filled_array, i), "x"));
        y = json_number_value(json_object_get(json_array_get(filled_array, i), "y"));
        set_cell(board, x, y, 1);
    }
}

input* parse_file(char* filename) {
    /* Read in file */
    FILE * file = fopen(filename, "rb");
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* json_text = malloc(length);
    fread(json_text, 1, length, file);

    /* Extract json into input struct */
    input* new_input = (input *) malloc(sizeof(input));
    json_error_t* error = 0;
    json_t* parsed = json_loads(json_text, 0, error);
    
    new_input->id = json_number_value(json_object_get(parsed, "id"));
    
    json_t* unit_array = json_object_get(parsed, "units");
    new_input->units = malloc(sizeof(unit *) * json_array_size(unit_array));
    for (int i = 0; i < json_array_size(unit_array); i++) {
        parse_unit(new_input->units, json_array_get(unit_array, i), i);
    }

    int width = json_number_value(json_object_get(parsed, "width"));
    new_input->width = width;
    int height = json_number_value(json_object_get(parsed, "height"));
    new_input->height = height;
    json_t* filled_array = json_object_get(parsed, "filled");
    new_input->board = new_board(width, height);
    fill_board(new_input->board, filled_array);

    new_input->num_seeds = json_number_value(json_object_get(parsed, "sourceLength"));
    new_input->source_seeds = malloc(sizeof(int) * new_input->num_seeds);
    json_t* seed_array = json_object_get(parsed, "sourceSeeds");
    for (int i = 0; i < json_array_size(seed_array); i++) {
        new_input->source_seeds[i] = json_number_value(json_array_get(seed_array, i));
    }

    return new_input;
}

void print_output(output* outputs[], int num_outputs) {
    // TODO nop for now
}

int main(int argc, char* argv[]) {
    char** filenames = malloc(sizeof(char *));
    int num_files = 0;
    int time_available = 0;
    int mem_available = 0;
    int cores_available = 0;
    char** wops = malloc(sizeof(char *));
    int num_wops = 0;

    char c;

    /* Parse arguments */
    while ((c = getopt (argc, argv, "f:t:m:c:p:")) != -1) {
        switch (c) {
            case 'f':
                num_files++;
                if (num_files > 1) {
                    filenames = realloc(filenames, sizeof(char *) * num_files);
                }
                filenames[num_files - 1] = optarg;
                break;
            case 't':
                if (time_available != 0) {
                    fprintf(stderr, "Only one time argument, please!\n");
                    return 1;
                }
                time_available = atoi(optarg);
                break;
            case 'm':
                if (mem_available != 0) {
                    fprintf(stderr, "Only one memory argument, please!\n");
                    return 1;
                }
                mem_available = atoi(optarg);
                break;
            case 'c':
                if (cores_available != 0) {
                    fprintf(stderr, "Only one corenum argument, please!\n");
                    return 1;
                }
                cores_available = atoi(optarg);
                break;
            case 'p':
                num_wops++;
                if (num_wops > 1) {
                    wops = realloc(wops, sizeof(char *) * num_wops);
                }
                wops[num_wops - 1] = optarg;
                break;
            case '?':
                fprintf(stderr, "Incorrect usage! See icfp spec for correct usage\n");
                return 1;
        }
    }

    /* Parse files */
    input** inputs = malloc(sizeof(input*) * num_files);
    for (int i = 0; i < num_files; i++) {
        inputs[i] = parse_file(filenames[i]);
    }

    /* Play games */
    output** outputs = malloc(sizeof(output*) * num_files);
    
    for (int i = 0; i < num_files; i++) { 
        outputs[i] = play_game(inputs[i], time_available, mem_available,
                               cores_available, wops, num_wops);
    }

    print_output(outputs, num_files);
    
    return 0;
}




    
