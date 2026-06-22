/*
** EPITECH PROJECT, 2026
** filename
** File description:
** description
*/

#pragma once
#include "Enemy.hpp"
#include "Player.hpp"
class Core;

struct ActiveEnemy {
    Enemy* templateData = nullptr;
    double x;
    double y;
    int hp;
    bool isAlive = false;
    float attackTimer;
    int state;
    float animTimer;
    int ammo;
    float stuckTimer;
    bool wanderRight;

    ActiveEnemy(Enemy* data, double startX, double startY, int startHp)
        : templateData(data), x(startX), y(startY), hp(startHp), isAlive(true),
          attackTimer(0.0f), state(0), animTimer(0.0f),
          ammo(data ? data->getAmmo() : 0),
          stuckTimer(0.0f), wanderRight(true) {}
};

std::vector<ActiveEnemy> InitEnemies(Core *core, char **mapArray, int map_rows);
void UpdateEnemies(std::vector<ActiveEnemy> &enemies, Player &p, float dt);
