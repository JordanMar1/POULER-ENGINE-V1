/*
** EPITECH PROJECT, 2026
** filename
** File description:
** description
*/

#include "Enemy.hpp"
#include "Core.hpp"

#pragma once

struct ActiveEnemy {
    Enemy* templateData;
    double x;
    double y;
    int hp;
    bool isAlive;
    float attackTimer;
    int state;
    float animTimer;

    ActiveEnemy(Enemy* data, double startX, double startY, int startHp)
        : templateData(data), x(startX), y(startY), hp(startHp), isAlive(true),
          attackTimer(0.0f), state(0), animTimer(0.0f) {}
};

std::vector<ActiveEnemy> InitEnemies(Core *core, char **mapArray, int map_rows);
void UpdateEnemies(std::vector<ActiveEnemy> &enemies, Player &p, float dt);
