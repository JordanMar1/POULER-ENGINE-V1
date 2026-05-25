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
            if (strcmp(parsed_file[i][j], "image") == 0
                && parsed_file[i][j + 1] != nullptr) {
                h->texture = sfTexture_createFromFile(parsed_file[i][j + 1], nullptr);
                if (h->texture) {
                    h->sprite = sfSprite_create();
                    sfSprite_setTexture(h->sprite, h->texture, sfTrue);
                } else {
                    std::cerr << "Failed to load head texture: " << parsed_file[i][j + 1] << std::endl;
                }
            }
            if (strcmp(parsed_file[i][j], "y") == 0
                && parsed_file[i][j + 1] != nullptr) {
                h->y = atoi(parsed_file[i][++j]);
                continue;
            }
            char *endptr = nullptr;
            long hp_val = strtol(parsed_file[i][j], &endptr, 10);
            if (endptr != parsed_file[i][j] && *endptr == '\0'
                && parsed_file[i][j + 1] != nullptr
                && h->threshold_count < 6)
            {
                HeadThreshold &t = h->thresholds[h->threshold_count++];
                t.hp = (int)hp_val;
                sscanf(parsed_file[i][j + 1], "%d,%d,%d,%d",
                       &t.x1, &t.y1, &t.x2, &t.y2);
                j++;
            }
        }

        if (debug) {
            std::cout << "[head] x=" << h->x << " y=" << h->y
                      << " thresholds=" << h->threshold_count << std::endl;
            for (int k = 0; k < h->threshold_count; k++) {
                std::cout << "  [" << h->thresholds[k].hp << "%] "
                          << h->thresholds[k].x1 << ","
                          << h->thresholds[k].y1 << " -> "
                          << h->thresholds[k].x2 << ","
                          << h->thresholds[k].y2 << std::endl;
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

void renderHead(sfRenderWindow *window, sfSprite *head_sprite, sfTexture *head_texture, Head *head, int current_hp, int max_hp)
{
    if (!head || !head_texture || !head_sprite)
        return;

    HeadThreshold *t = head->get_threshold(current_hp, max_hp);
    if (!t)
        return;

    sfIntRect rect;
    rect.left   = t->x1;
    rect.top    = t->y1;
    rect.width  = t->x2 - t->x1;
    rect.height = t->y2 - t->y1;

    sfSprite_setTexture(head_sprite, head_texture, sfTrue);
    sfSprite_setTextureRect(head_sprite, rect);
    sfSprite_setPosition(head_sprite, (sfVector2f){(float)head->x, (float)head->y});

    sfRenderWindow_drawSprite(window, head_sprite, nullptr);
}