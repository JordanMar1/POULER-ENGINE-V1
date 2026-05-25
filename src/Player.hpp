/*
** EPITECH PROJECT, 2026
** POULER-ENGINE-V1.1
** File description:
** Player
*/

#pragma once

class Player {
    public:
        Player(char ***parsedMap, bool debug);
        ~Player();
        double x, y = 2.5;
        double dirX, dirY;
        double planeX, planeY;
        double height = 1.0;
        double defaultHeight = height;
        double crouchHeight = height * 0.5;
        double eyeHeight;
        bool crouching = false;
        float lean = 0.0f;
        float pitch = 0.0f;
        int hp;
        int maxHp;
        int stamina = 100;
        int maxStamina = stamina;
};
