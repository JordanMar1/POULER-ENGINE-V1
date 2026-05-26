/*
** EPITECH PROJECT, 2026
** MY_GZDOOM
** File description:
** Head
*/
#pragma once
#include <SFML/Graphics.h>
#include <vector>
struct HeadThreshold {
    int hp;
    int x1, y1;
    int x2, y2;
};

class Head {
public:
    int             x, y;
    Head           *next;
    Head           *prev;
    sfSprite        *sprite;
    sfTexture       *texture;
    float scale;
    std::vector<std::pair<int, HeadThreshold>> thresholds;
    Head() : x(0), y(0), next(nullptr), prev(nullptr), sprite(nullptr), texture(nullptr), scale(1.0f), thresholds {} {}
    ~Head() {
        if (sprite)
            sfSprite_destroy(sprite);
        if (texture)
            sfTexture_destroy(texture);
    }
    HeadThreshold *get_threshold(int current_hp) {
        for (auto &[hp, t] : thresholds) {
            if (current_hp >= hp)
                return &t;
        }
        return thresholds.empty() ? nullptr : &thresholds.back().second;
    }
};
Head *parse_heads(char ***parsed_file, bool debug);