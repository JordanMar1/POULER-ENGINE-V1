/*
** EPITECH PROJECT, 2026
** POULER-ENGINE-V1.1
** File description:
** InterfaceItems
*/

#pragma once

#include <SFML/Graphics.h>
#include <string>
#include <vector>

class Player;
class Weapons;

struct InterfaceItemDef {
    std::string info;
    std::string image;
    std::string font_path;
    float x = 0.0f;
    float y = 0.0f;
    int r = 255;
    int g = 255;
    int b = 255;
    int mid_r = -1;
    int mid_g = -1;
    int mid_b = -1;
    int low_r = -1;
    int low_g = -1;
    int low_b = -1;
    bool beating = false;
    bool on_check = false;
};

struct InterfaceItemState {
    InterfaceItemDef def;
    sfFont *font = nullptr;
    bool font_owned = false;
    sfTexture *texture = nullptr;
    sfSprite  *sprite  = nullptr;
    float beat_timer = 0.0f;
    float beat_scale = 1.0f;
    float check_timer = 0.0f;
    static constexpr float CHECK_DURATION = 0.25f;
};

class InterfaceItems {
public:

    InterfaceItems(sfFont *fallbackFont);
    ~InterfaceItems();

    void loadItems(const std::vector<InterfaceItemDef> &defs);

    void notifyCheckPressed(float duration);

    void render(sfRenderWindow *window, Player *player,
            const std::vector<Weapons *> &weapons,
            int current_weapon_idx, float dt,
            float ammo_display_timer = 0.0f);
    void drawHealthBar  (sfRenderWindow *window, Player *player);
    void drawAmmoInfo   (sfRenderWindow *window, const std::vector<Weapons *> &weapons, int current_weapon_idx);
    void drawWeaponList (sfRenderWindow *window, const std::vector<Weapons *> &weapons, int current_weapon_idx);
    void drawItemInfo   (sfRenderWindow *window, Player *player, const std::vector<Weapons *> &weapons);

private:
    sfFont *_fallbackFont = nullptr;
    std::vector<InterfaceItemState *> _items;
    
    void _renderHpImage        (sfRenderWindow *window, InterfaceItemState *s, Player *player, float dt);
    void _renderHpText         (sfRenderWindow *window, InterfaceItemState *s, Player *player);
    void _renderStaminaImage   (sfRenderWindow *window, InterfaceItemState *s, Player *player, float dt);
    void _renderStaminaText    (sfRenderWindow *window, InterfaceItemState *s, Player *player);
    void _renderAmmoImage      (sfRenderWindow *window, InterfaceItemState *s,
                                const std::vector<Weapons *> &weapons, int idx, float dt);
    void _renderAmmoText       (sfRenderWindow *window, InterfaceItemState *s,
                                const std::vector<Weapons *> &weapons, int idx);
                                
    sfFont *_resolveFont(const std::string &path);
    void _drawText(sfRenderWindow *window, sfFont *font,
                   const std::string &str, unsigned int size,
                   sfColor color, float x, float y);
};

std::vector<InterfaceItemDef> buildInterfaceItems(char ***parsed_lines, bool debug = false);