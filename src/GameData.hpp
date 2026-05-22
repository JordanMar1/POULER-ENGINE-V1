/*
** EPITECH PROJECT, 2026
** POULER OR NOT
** File description:
** GameData
*/

#pragma once

#include "Settings.hpp"
#include "Window.hpp"
#include "Ray.hpp"
#include <string>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define MOVE_SPEED 4
#define FOV 60
#define RUN_SPEED 6
#define TILE_SIZE 64
#define M_PI 3.14159265358979323846
#define PLAYER_HEIGHT 32
#define MAX_DEPTH 1000

class GameData {
    public:
        GameData(char *filepath);
        ~GameData();
        char ***game_array;
        int is_err;
    private:

};
