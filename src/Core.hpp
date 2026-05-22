/*
** EPITECH PROJECT, 2026
** MY_GZDOOM
** File description:
** Core
*/

#pragma once
#include "ErrorHandler.hpp"
#include "GameData.hpp"
#include "Window.hpp"
#include "Menu.hpp"
#include "Settings.hpp"
#include "Maps.hpp"
#include "Weapons.hpp"
class Core {
    public:
        Core(Window *window, Menu *menu, GameData *gameData, Settings *settings, std::vector<Weapons *> weapons) : window(window), menu(menu), gameData(gameData), settings(settings), weapons(weapons) {};
        int chooseLevel(void);
        int menu_return(void);
        void setMaps(Maps *maps) { this->maps = maps; }
        Maps *getMaps() const { return maps; }
        Window *getWindow() const { return window; }
        Menu *getMenu() const { return menu; }
        GameData *getGameData() const { return gameData; }
        Settings *getSettings() const { return settings; }
        std::vector<Weapons *> getWeapons() const {return weapons;};
        void displayLevels();
    private:
        Window *window;
        Menu *menu;
        GameData *gameData;
        ErrorHandler *errorHandler;
        Settings *settings;
        Maps *maps;
        std::vector<Weapons *> weapons;
};
