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
        float stamina = 100.0f;
        float maxStamina = 100.0f;
        float speed = 5.0f;
        float sprint_speed = 8.0f;
        float sprintCooldown = 100.0f;
        bool staminaCooldown = false;
        int maxWeapons = 2;
        float staminaConsumption = 30.0f;
};
