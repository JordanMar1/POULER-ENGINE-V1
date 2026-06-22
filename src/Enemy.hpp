/*
** EPITECH PROJECT, 2026
** Enemy
** File description:
** Enemy class init
*/

#pragma once

#include <SFML/Graphics/Types.h>
#include <string>

#pragma once

#include <SFML/Graphics/Sprite.h>
#include <SFML/Graphics/Texture.h>
#include <string>
#include <vector>
#include <utility>

class Enemy {
    public:
        Enemy(char **enemyContent);
        ~Enemy();

        char getChar();
        std::string getName();
        int getAmmo();
        float getReloadTime();
        float getReshootTime();
        float getDist();
        sfSprite* getSprite();
        sfTexture* getTexture();
        int getHp();
        int getDmg();
        std::vector<int> getSpriteX();
        std::vector<int> getSpriteY();
        std::vector<std::pair<int, int>> getShootAnim();
        std::vector<std::pair<int, int>> getReloadAnim();
        std::vector<std::pair<int,int>> getWalkAnim();

    private:
        char _char;
        std::string _name;
        int _ammo;
        float _reloadTime;
        float _reshootTime;
        float _dist;
        sfSprite* _sprite;
        sfTexture* _texture;
        int _hp;
        int _dmg;
        std::vector<int> _sprite_x;
        std::vector<int> _sprite_y;
        std::vector<std::pair<int, int>> _shoot_anim;
        std::vector<std::pair<int, int>> _reload_anim;
        std::vector<std::pair<int,int>> _walk_anim;
};

Enemy **enemyBuilder(char ***parsed_file);
