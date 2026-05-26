/*
** EPITECH PROJECT, 2026
** MY_GZDOOM
** File description:
** Head
*/
#pragma once
#include <SFML/Graphics.h>

struct HeadThreshold {
    int hp;
    int x1, y1;
    int x2, y2;
};

class Head {
public:
    int             x, y;
    HeadThreshold   thresholds[6];
    int             threshold_count;
    Head           *next;
    Head           *prev;
    sfSprite        *sprite;
    sfTexture       *texture;

    Head() : x(0), y(0), threshold_count(0), next(nullptr), prev(nullptr), sprite(nullptr), texture(nullptr) {}
    
    ~Head() {
        if (sprite)
            sfSprite_destroy(sprite);
        if (texture)
            sfTexture_destroy(texture);
    }

    HeadThreshold *get_threshold(int current_hp)
    {
        for (int i = 0; i < threshold_count; i++) {
            if (current_hp >= thresholds[i].hp)
                return &thresholds[i];
        }
        return (threshold_count > 0) ? &thresholds[threshold_count - 1] : nullptr;
    }
};
Head *parse_heads(char ***parsed_file, bool debug);
void renderHead(sfRenderWindow *window, sfSprite *head_sprite, sfTexture *head_texture, Head *head, int current_hp);