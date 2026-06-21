/*
** EPITECH PROJECT, 2026
** MY_GZDOOM
** File description:
** Head
*/
#include "Head.hpp"
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>

Head *parse_heads(char ***parsed_file, bool debug)
{
    Head *root    = nullptr;
    Head *current = nullptr;
    for (int i = 0; parsed_file[i] != nullptr; i++) {
        if (strcmp(parsed_file[i][0], "head") != 0)
            continue;
        Head *h = new Head();
        for (int j = 1; parsed_file[i][j] != nullptr; j++) {
            if (strcmp(parsed_file[i][j], "x") == 0
            && parsed_file[i][j + 1] != nullptr) {
                h->x = atoi(parsed_file[i][++j]);
                continue;
            }
            if (strcmp(parsed_file[i][j], "y") == 0
            && parsed_file[i][j + 1] != nullptr) {
                h->y = atoi(parsed_file[i][++j]);
                continue;
            }
            if (strcmp(parsed_file[i][j], "scale") == 0
            && parsed_file[i][j + 1] != nullptr) {
                h->scale = std::stof(parsed_file[i][++j]);
                continue;
            }
            if (strcmp(parsed_file[i][j], "image") == 0
            && parsed_file[i][j + 1] != nullptr) {
                h->texture = sfTexture_createFromFile(parsed_file[i][j + 1], nullptr);
                if (h->texture) {
                    h->sprite = sfSprite_create();
                    sfSprite_setTexture(h->sprite, h->texture, sfTrue);
                } else {
                    std::cerr << "Failed to load head texture: "
                              << parsed_file[i][j + 1] << std::endl;
                }
                j++;
                continue;
            }
            char *endptr = nullptr;
            long hp_val = strtol(parsed_file[i][j], &endptr, 10);
            if (endptr != parsed_file[i][j] && *endptr == '\0'
            && parsed_file[i][j + 1] != nullptr) {
                HeadThreshold t = {0, 0, 0, 0, 0};
                int parsed_count = sscanf(parsed_file[i][j + 1], "%d,%d,%d,%d",
                    &t.x1, &t.y1, &t.x2, &t.y2);
                if (parsed_count == 4) {
                    h->thresholds.push_back({(int)hp_val, t});
                    j++;
                }
            }
        }
        if (debug) {
            std::cout << "[head] x=" << h->x << " y=" << h->y
                      << " scale=" << h->scale
                      << " thresholds=" << h->thresholds.size() << std::endl;
            for (int k = 0; k < (int)h->thresholds.size(); k++) {
                std::cout << "  [" << h->thresholds[k].first << "%] "
                    << h->thresholds[k].second.x1 << ","
                    << h->thresholds[k].second.y1 << " -> "
                    << h->thresholds[k].second.x2 << ","
                    << h->thresholds[k].second.y2 << std::endl;
            }
        }
        if (!root) {
            root    = h;
            current = h;
        } else {
            current->next = h;
            h->prev       = current;
            current       = h;
        }
    }
    return root;
}
