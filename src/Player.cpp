/*
** EPITECH PROJECT, 2026
** POULER-ENGINE-V1.1
** File description:
** Player
*/

#include "Player.hpp"
#include <iostream>
#include <cstring>

Player::Player(char ***parsed_file, bool debug)
{
    dirX  =  1.0;
    dirY  =  0.0;
    planeX = 0.0;
    planeY = 0.66;
    hp = 100;
    maxHp = 100;
    for (int i = 0; parsed_file[i] != NULL; i++) {
        if (strcmp(parsed_file[i][0], "player") == 0) {
            for (int j = 1; parsed_file[i][j] != NULL; j++) {
                if (strcmp(parsed_file[i][j], "hp") == 0 && parsed_file[i][j + 1] != NULL) {
                    hp = std::stoi(parsed_file[i][j + 1]);
                    if (hp < 0)
                        hp = 100;
                    if (debug)
                        std::cout << "Player HP set to: " << hp << std::endl;
                }
                if (strcmp(parsed_file[i][j], "maxhp") == 0 && parsed_file[i][j + 1] != NULL) {
                    maxHp = std::stoi(parsed_file[i][j + 1]);
                    if (maxHp <= 0)
                        maxHp = 100;
                    if (debug)
                        std::cout << "Player max HP set to: " << maxHp << std::endl;
                }
                if (strcmp(parsed_file[i][j], "height") == 0 && parsed_file[i][j + 1] != NULL) {
                    height = atof(parsed_file[i][j + 1]);
                    if (height <= 0.0)
                        height = 1.0;
                    defaultHeight = height;
                    if (debug)
                        std::cout << "Player height set to: " << height << std::endl;
                }
                if (strcmp(parsed_file[i][j], "crouch_height") == 0 && parsed_file[i][j + 1] != NULL) {
                    crouchHeight = atof(parsed_file[i][j + 1]);
                    if (crouchHeight <= 0.0)
                        crouchHeight = height * 0.5;
                    if (debug)
                        std::cout << "Player crouch height set to: " << crouchHeight << std::endl;
                }
                if (strcmp(parsed_file[i][j], "stamina") == 0 && parsed_file[i][j + 1] != NULL) {
                    stamina = std::stoi(parsed_file[i][j + 1]);
                    if (stamina < 0)
                        stamina = 100;
                    maxStamina = stamina;
                    if (debug)
                        std::cout << "Player stamina set to: " << stamina << std::endl;
                }
                if (strcmp(parsed_file[i][j], "speed") == 0 && parsed_file[i][j + 1] != NULL) {
                    float speed = atof(parsed_file[i][j + 1]);
                    if (speed <= 0.0f)
                        speed = 5.0f;
                    if (debug)
                        std::cout << "Player speed set to: " << speed << std::endl;
                }
                if (strcmp(parsed_file[i][j], "sprint_speed") == 0 && parsed_file[i][j + 1] != NULL) {
                    float sprint_speed = atof(parsed_file[i][j + 1]);
                    if (sprint_speed <= 0.0f)
                        sprint_speed = 8.0f;
                    if (debug)
                        std::cout << "Player sprint speed set to: " << sprint_speed << std::endl;
                }
                if (strcmp(parsed_file[i][j], "stamina_cooldown") == 0 && parsed_file[i][j + 1] != NULL) {
                    sprintCooldown = std::stof(parsed_file[i][j + 1]);
                    if (sprintCooldown < 0.0f)
                        sprintCooldown = 100.0f;
                    if (debug)
                        std::cout << "Player sprint cooldown set to: " << sprintCooldown << std::endl;
                }
                if (strcmp(parsed_file[i][j], "max_weapons") == 0 && parsed_file[i][j + 1] != NULL) {
                    maxWeapons = std::stoi(parsed_file[i][j + 1]);
                    if (maxWeapons <= 0)
                        maxWeapons = 2;
                    if (debug)
                        std::cout << "Player max weapons set to: " << maxWeapons << std::endl;
                }
                if (strcmp(parsed_file[i][j], "stamina_consumption") == 0 && parsed_file[i][j + 1] != NULL) {
                    staminaConsumption = std::stof(parsed_file[i][j + 1]);
                    if (staminaConsumption < 0.0f)
                        staminaConsumption = 30.0f;
                    if (debug)
                        std::cout << "Player stamina consumption set to: " << staminaConsumption << std::endl;
                }
                if (strcmp(parsed_file[i][j], "stamina_recovery") == 0 && parsed_file[i][j + 1] != NULL) {
                    staminaRecovery = std::stof(parsed_file[i][j + 1]);
                    if (staminaRecovery < 0.0f)
                        staminaRecovery = 15.0f;
                    if (debug)
                        std::cout << "Player stamina recovery set to: " << staminaRecovery << std::endl;
                }
            }
        }
        if (debug)
            std::cout << "Player HP: " << hp << "/" << maxHp << std::endl;
        break;
    }
    eyeHeight = height + 0.5;
}

Player::~Player()
{
}
