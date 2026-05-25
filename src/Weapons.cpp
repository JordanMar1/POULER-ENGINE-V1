/*
** EPITECH PROJECT, 2026
** MY_GZDOOM
** File description:
** Weapons parsing implementation
*/

#include "Weapons.hpp"
#include <sstream>
#include <stdexcept>
#include <iostream>

static std::vector<int> parse_int_list(const std::string &s, char sep = '-')
{
    std::vector<int> result;
    if (s.empty()) return result;
    std::stringstream ss(s);
    std::string token;
    while (std::getline(ss, token, sep)) {
        if (!token.empty())
            result.push_back(std::stoi(token));
    }
    return result;
}

static std::vector<std::pair<int,int>> parse_anim_frames(const std::string &s)
{
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

static std::string get_field(char **block, const std::string &key)
{
    if (!block) return "";
    for (int i = 0; block[i]; i += 1) {
        if (std::string(block[i]) == key && block[i + 1])
            return std::string(block[i + 1]);
    }
    return "";
}

static Weapons *create_weapon_from_block(char **block)
{
    if (!block || !block[0] || std::string(block[0]) != "weapon")
        return nullptr;

    Weapons *w = new Weapons();

    try {
        w->name = get_field(block, "name");
        w->sprite_path = get_field(block, "sprite");
        std::string id_str = get_field(block, "id");
        w->id = id_str.empty() ? 0 : std::stoi(id_str);
        std::string x = get_field(block, "x");
        w->x = x.empty() ? 800 : std::stof(x);
        std::string y = get_field(block, "y");
        w->y = y.empty() ? 800 : std::stof(y);
        std::string scale_x = get_field(block, "scale_x");
        w->scale_x = scale_x.empty() ? 3.5 : std::stof(scale_x);
        std::string scale_y = get_field(block, "scale_y");
        w->scale_y = scale_y.empty() ? 3.5 : std::stof(scale_y);
        std::string dmg_str = get_field(block, "dmg");
        w->dmg           = dmg_str.empty() ? 0 : std::stoi(dmg_str);
        std::string ammo_str = get_field(block, "ammo");
        w->ammo          = ammo_str.empty() ? 0 : std::stoi(ammo_str);
        w->max_ammo = w->ammo;
        std::string mag_str = get_field(block, "mag");
        w->mag           = mag_str.empty() ? 0 : std::stoi(mag_str);
        std::string fr_str = get_field(block, "firerate");
        w->firerate      = fr_str.empty() ? 0.f : std::stof(fr_str);
        std::string fd_str = get_field(block, "fire_distance");
        w->fire_distance = fd_str.empty() ? 0.f : std::stof(fd_str);
        w->sprite_x      = parse_int_list(get_field(block, "SP_x"));
        w->sprite_y      = parse_int_list(get_field(block, "SP_y"));
        w->shoot_anim    = parse_anim_frames(get_field(block, "SH_AM"));
        w->check_anim    = parse_anim_frames(get_field(block, "CH_AM"));
        std::string check_time_str = get_field(block, "CH_TIME");
        w->check_time    = check_time_str.empty() ? 0.f : std::stof(check_time_str);
        w->sh_buff = sfSoundBuffer_createFromFile(get_field(block, "SH_SD").c_str());
        w->shoot_sound = sfSound_create();
        if (w->sh_buff)
            sfSound_setBuffer(w->shoot_sound, w->sh_buff);
        else
            sfSound_destroy(w->shoot_sound);
        std::string st_str = get_field(block, "SH_TIME");
        w->shoot_time    = st_str.empty() ? 0.f : std::stof(st_str);
        w->reload_anim   = parse_anim_frames(get_field(block, "RD_AM"));
        w->rd_buff = sfSoundBuffer_createFromFile(get_field(block, "RD_SD").c_str());
        w->reload_sound = sfSound_create();
        if (w->rd_buff)
            sfSound_setBuffer(w->reload_sound, w->rd_buff);
        else
            sfSound_destroy(w->reload_sound);
        std::string rt_str = get_field(block, "RD_TIME");
        w->reload_time   = rt_str.empty() ? 0.f : std::stof(rt_str);
        
    } catch (const std::exception &e) {
        delete w;
        return nullptr;
    }

    return w;
}

std::vector<Weapons *> create_weapons(char ***game_array, bool isDebug)
{
    std::vector<Weapons *> weapons;

    if (!game_array) {
            if (isDebug)
                std::cout << "WARNING: No weapons included" << std::endl;
        return weapons;
    }
    for (int i = 0; game_array[i]; i++) {
        if (!game_array[i][0]) continue;
        if (std::string(game_array[i][0]) != "weapon") continue;
        Weapons *w = create_weapon_from_block(game_array[i]);
        if (isDebug) {
            std::cout << "========================================" << std::endl;
            std::cout << "Adding weapon: " << (w->name.empty() ? "UNKNOWN" : w->name) << " (ID: " << w->id << ")" << std::endl;
            std::cout << "  -> Sprite Path: " << w->sprite_path << std::endl;
            std::cout << "  -> Sprite X positions: [ ";
            for (int val : w->sprite_x) std::cout << val << " ";
            std::cout << "]" << std::endl;
            std::cout << "  -> Sprite Y positions: [ ";
            for (int val : w->sprite_y) std::cout << val << " ";
            std::cout << "]" << std::endl;
            std::cout << "  -> Shoot Animation Frames (SH_AM): [ ";
            for (const auto &frame : w->shoot_anim) {
                std::cout << "(" << frame.first << "," << frame.second << ") ";
            }
            std::cout << "]" << std::endl;
            std::cout << "  -> Reload Animation Frames (RD_AM): [ ";
            for (const auto &frame : w->reload_anim) {
                std::cout << "(" << frame.first << "," << frame.second << ") ";
            }
            std::cout << "]" << std::endl;
            std::cout << "========================================" << std::endl;
        }
        if (w) weapons.push_back(w);
        if (!w) std::cout << "Error: weapon not added";
    }
    return weapons;
}

void setWeaponsVolume(std::vector<Weapons *> weapons, int volume)
{
    for (const auto &wp : weapons) {
        wp->setWeaponVolume(volume);
    }
} 