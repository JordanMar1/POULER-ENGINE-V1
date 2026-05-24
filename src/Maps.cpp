/*
** EPITECH PROJECT, 2026
** MY_GZDOOM
** File description:
** Maps
*/

#include "Maps.hpp"
#include "GameData.hpp"
#include <iostream>
#include <cstring>
#include "../include/array_lib.hpp"

Maps::Maps(char ***parsed_file, bool debug)
{
    if (debug)
        std::cout << "Parsing maps from game data..." << std::endl;
    Maps *current = this;
    for (int i = 0; parsed_file[i] != NULL; i++) {
        if (strcmp(parsed_file[i][0], "map") == 0) {
            for (int j = 1; parsed_file[i][j] != NULL; j++) {
                if (strcmp(parsed_file[i][j], "name") == 0 && parsed_file[i][j + 1] != NULL) {
                    current->name = strdup(parsed_file[i][j + 1]);
                }
                if (strcmp(parsed_file[i][j], "music") == 0 && parsed_file[i][j + 1] != NULL) {
                    current->music = sfMusic_createFromFile(parsed_file[i][j + 1]);
                    if (debug && !current->music)
                        std::cout << "Warning: error on music";
                }
                if (strcmp(parsed_file[i][j], "content") == 0 && parsed_file[i][j + 1] != NULL) {
                    current->map_array = my_str_to_word_array(parsed_file[i][j + 1], (char *)"\n");
                }
                if (strcmp(parsed_file[i][j], "heatmap") == 0 && parsed_file[i][j + 1] != NULL) {
                    char **lines = my_str_to_word_array(parsed_file[i][j + 1], (char *)"\n");
                    int line_count = 0;
                    while (lines[line_count] != NULL) line_count++;
                    current->heatmap_rows = line_count;
                    current->heatmap = new int *[line_count + 1];
                    for (int l = 0; l < line_count; l++) {
                        int len = (int)strlen(lines[l]);
                        if (len > current->heatmap_cols) current->heatmap_cols = len;
                        current->heatmap[l] = new int[len + 1];
                        for (int m = 0; m < len; m++)
                            current->heatmap[l][m] = lines[l][m] - '0';
                        current->heatmap[l][len] = -1;
                        free(lines[l]);
                    }
                    current->heatmap[line_count] = nullptr;
                    free(lines);
                }
            }
            bool has_next = false;
            for (int k = i + 1; parsed_file[k] != NULL; k++) {
                if (strcmp(parsed_file[k][0], "map") == 0) {
                    has_next = true;
                    break;
                }
            }
            if (has_next) {
                current->next = new Maps();
                current->next->prev = current;
                current = current->next;
            }
        }
    }
}

Maps::~Maps()
{
    if (name) free(name);
    if (map_array) {
        for (int i = 0; map_array[i] != nullptr; i++) free(map_array[i]);
        free(map_array);
    }
    if (heatmap) {
        for (int i = 0; heatmap[i] != nullptr; i++) delete[] heatmap[i];
        delete[] heatmap;
    }
    if (next) delete next;
}
