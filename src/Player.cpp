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
    for (int i = 0; parsed_file[i] != NULL; i++) {
        if (strcmp(parsed_file[i][0], "player") == 0) {
            for (int j = 1; parsed_file[i][j] != NULL; j++) {
                if (strcmp(parsed_file[i][j], "hp") == 0 && parsed_file[i][j + 1] != NULL) {
                    hp = std::stoi(parsed_file[i][j + 1]);
                    if (hp < 0) {
                        hp = 100;
                    }
                }
                if (strcmp(parsed_file[i][j], "max_hp") == 0 && parsed_file[i][j + 1] != NULL) {
                    maxHp = std::stoi(parsed_file[i][j + 1]);
                    if (maxHp <= 0) {
                        maxHp = 100;
                    }
                }
                if (strcmp(parsed_file[i][j], "height") == 0 && parsed_file[i][j + 1] != NULL) {
                    height = atof(parsed_file[i][j + 1]);
                    if (height <= 0.0) {
                        height = 1.0;
                    }
                    defaultHeight = height;
                }
                if (strcmp(parsed_file[i][j], "crouch_height") == 0 && parsed_file[i][j + 1] != NULL) {
                    crouchHeight = atof(parsed_file[i][j + 1]);
                    if (crouchHeight <= 0.0) {
                        crouchHeight = height * 0.5;
                    }
                }
            }
             if (debug)
                std::cout << "Player HP: " << hp << "/" << maxHp << std::endl;
            break;
        }
    }
    eyeHeight = height + 0.5;
}

Player::~Player()
{
}
