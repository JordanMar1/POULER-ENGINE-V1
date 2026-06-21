/*
** EPITECH PROJECT, 2026
** Enemy
** File description:
** Manage Enemies
*/

#include "Enemy.hpp"
#include <SFML/Graphics/Sprite.h>
#include <SFML/Graphics/Texture.h>
#include <cstddef>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>
#include <sstream>

static std::vector<int> parse_int_list(const std::string &s, char sep = '-') {
    std::vector<int> result;
    if (s.empty()) return result;
    std::stringstream ss(s);
    std::string token;
    while (std::getline(ss, token, sep)) {
        if (!token.empty()) result.push_back(std::stoi(token));
    }
    return result;
}

static std::vector<std::pair<int,int>> parse_anim_frames(const std::string &s) {
    std::vector<std::pair<int,int>> frames;
    if (s.empty()) return frames;
    std::stringstream ss(s);
    std::string token;
    while (std::getline(ss, token, '-')) {
        auto comma = token.find(',');
        if (comma == std::string::npos) continue;
        int x = std::stoi(token.substr(0, comma));
        int y = std::stoi(token.substr(comma + 1));
        frames.push_back({x, y});
    }
    return frames;
}

Enemy::Enemy(char **enemyContent)
{
    _sprite = sfSprite_create();
    _dmg = 0;
    _hp = 0;

    for (int i = 0; enemyContent[i] != NULL; i++) {
        if (enemyContent[i + 1] == NULL) continue;
        std::string key = enemyContent[i];
        std::string val = enemyContent[i + 1];

        if (key == "name") _name = val;
        if (key == "ammo") _ammo = std::stoi(val);
        if (key == "fire_distance") _dist = std::stof(val);
        if (key == "hp") _hp = std::stoi(val);
        if (key == "reshoot_time") _reshootTime = std::stof(val);
        if (key == "reload_time") _reloadTime = std::stof(val);
        if (key == "char") _char = val[0];
        if (key == "sprite") _texture = sfTexture_createFromFile(val.c_str(), NULL);
        if (key == "dmg") _dmg = std::stoi(val);
        if (key == "SP_x") _sprite_x = parse_int_list(val);
        if (key == "SP_y") _sprite_y = parse_int_list(val);
        if (key == "SH_AM") _shoot_anim = parse_anim_frames(val);
        if (key == "RD_AM") _reload_anim = parse_anim_frames(val);
    }
}

Enemy::~Enemy() {}

char Enemy::getChar() { return _char; }
std::string Enemy::getName() { return _name; }
int Enemy::getAmmo() { return _ammo; }
float Enemy::getReloadTime() { return _reloadTime; }
float Enemy::getReshootTime() { return _reshootTime; }
float Enemy::getDist() { return _dist; }
sfSprite* Enemy::getSprite() { return _sprite; }
sfTexture* Enemy::getTexture() { return _texture; }
int Enemy::getHp() { return _hp; }
int Enemy::getDmg() { return _dmg; }
std::vector<int> Enemy::getSpriteX() { return _sprite_x; }
std::vector<int> Enemy::getSpriteY() { return _sprite_y; }
std::vector<std::pair<int, int>> Enemy::getShootAnim() { return _shoot_anim; }
std::vector<std::pair<int, int>> Enemy::getReloadAnim() { return _reload_anim; }

Enemy **enemyBuilder(char ***parsed_file)
{
    std::vector<Enemy *> created;
    for (int i = 0; parsed_file[i] != nullptr; i++) {
        if (strcmp(parsed_file[i][0], "enemy") != 0)
            continue;
        Enemy *e = new Enemy(parsed_file[i]);
        created.push_back(e);
    }
    Enemy **enemies = new Enemy*[created.size() + 1];
    for (size_t i = 0; i < created.size(); i++)
        enemies[i] = created[i];
    enemies[created.size()] = nullptr;
    return enemies;
}
