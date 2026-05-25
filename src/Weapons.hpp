#pragma once

/*
** EPITECH PROJECT, 2026
** MY_GZDOOM
** File description:
** Weapons
*/

#pragma once

#include <string>
#include <vector>
#include <utility>
#include <SFML/Audio.h>

class Weapons {
public:
    int id;
    std::string name;
    std::string sprite_path;
    float x;
    float y;
    float scale_x;
    float scale_y;
    int dmg;
    int ammo;
    int max_ammo;
    int mag;
    int max_mag;
    float firerate;
    float fire_distance;
    std::vector<int> sprite_x;
    std::vector<int> sprite_y;
    std::vector<std::pair<int,int>> shoot_anim;
    std::vector<std::pair<int,int>> reload_anim;
    std::vector<std::pair<int,int>> check_anim;
    float shoot_time;
    sfSoundBuffer *rd_buff;
    sfSoundBuffer *sh_buff;
    sfSound *reload_sound;
    sfSound *shoot_sound;
    float reload_time;
    float check_time;
    void setWeaponVolume(int volume) { sfSound_setVolume(reload_sound, (float)volume); sfSound_setVolume(shoot_sound, (float)volume); }
    Weapons()
        : id(0), dmg(0), ammo(0), mag(0),
          firerate(0.f), fire_distance(0.f),
          shoot_time(0.f), reload_time(0.f), check_time(0.f) {}
};

std::vector<Weapons *> create_weapons(char ***game_array, bool isDebug);
void setWeaponsVolume(std::vector<Weapons *> weapons, int volume);