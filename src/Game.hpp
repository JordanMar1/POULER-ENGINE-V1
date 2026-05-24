/*
** EPITECH PROJECT, 2026
** MY_GZDOOM
** File description:
** Game
*/
#pragma once
#include "Core.hpp"

struct Player {
    double x, y;
    double dirX, dirY;
    double planeX, planeY;
    double height;
    double defaultHeight = height;
    double eyeHeight;
    bool crouching = false;
    float lean = 0.0f;
    float pitch = 0.0f;
};

class Game {
public:
    int Play(Core *core, Maps *map);
    float _teleportCooldown = 0.0f;
private:
    void HandleInputs(Core *core, Player &p, float dt, int map_rows, std::vector<Weapons *> &weapons, int current_weapon_idx, int &weapon_state, float &weapon_timer);
    void UpdateWeaponTimer(std::vector<Weapons *> &weapons, int current_weapon_idx, int &weapon_state, float &weapon_timer, float dt);
    void RenderWeapon(Core *core, sfRenderWindow *window, const std::vector<Weapons *> &weapons, const std::vector<sfTexture *> &weapon_textures, sfSprite *weapon_sprite, int current_weapon_idx, int weapon_state, float weapon_timer);
    void ManageMouse(sfRenderWindow *window, Player &p, Settings *settings);
    void RenderScene(Core *core, sfUint8 *pixels, Player &p, int map_rows);
    int getH(Core *core, int mx, int my, int map_rows);
};