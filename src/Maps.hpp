/*
** EPITECH PROJECT, 2026
** MY_GZDOOM
** File description:
** Maps
*/

#pragma once

class Maps {
public:
    Maps() {};
    Maps(char ***parsed_file, bool debug);
    ~Maps();
    char **getMapArray() const { return map_array; }
    int **getHeatmap() const { return heatmap; }
    int getHeatmapRows() const { return heatmap_rows; }
    int getHeatmapCols() const { return heatmap_cols; }
    char *getName() const { return name; }
    Maps *next = nullptr;
    Maps *prev = nullptr;

private:
    char **map_array = nullptr;
    char *name      = nullptr;
    int **heatmap   = nullptr;
    int  heatmap_rows = 0;
    int  heatmap_cols = 0;
};