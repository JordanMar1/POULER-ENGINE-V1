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
#include "Player.hpp"
#include "InterfaceItems.hpp"
#include "Head.hpp"
#include "Enemy.hpp"
#include "ActiveEnemy.hpp"
class Core {
    private:
        Window *window;
        Menu *menu;
        GameData *gameData;
        ErrorHandler *errorHandler;
        Settings *settings;
        Maps *maps;
        std::vector<Weapons *> weapons;
        Player *player;
        Head *head;
        Enemy **enemies;
        int getMapHeightForEnemy(int mx, int my, int map_rows) const;
    public:
        Core(Window *window, Menu *menu, GameData *gameData, Settings *settings, std::vector<Weapons *> weapons, Player *player, Head *head, Enemy **enemies) : window(window), menu(menu), gameData(gameData), settings(settings), weapons(weapons), player(player), head(head), enemies(enemies), interfaceItems({}) {};
        ~Core();
        void updateEnemyState(ActiveEnemy &e, Player &p, float dt, int map_rows);
        void UpdateEnemies(std::vector<ActiveEnemy> &enemies, Player &p, float dt, Core *core, int map_rows);
        int chooseLevel(void);
        int menu_return(void);
        void setMaps(Maps *maps) { this->maps = maps; }
        Maps *getMaps() const { return maps; }
        Window *getWindow() const { return window; }
        Menu *getMenu() const { return menu; }
        Enemy **getEnemies() const { return enemies; }
        GameData *getGameData() const { return gameData; }
        Settings *getSettings() const { return settings; }
        std::vector<Weapons *> getWeapons() const {return weapons;};
        Player *getPlayer() const { return player; }
        Head *getHead() const { return head; }
        std::vector<InterfaceItemDef> getInterfaceItems() const { return interfaceItems; }
        void displayLevels(Maps *selected);
        std::vector<InterfaceItemDef> interfaceItems;
};
