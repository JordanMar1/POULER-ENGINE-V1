/*
** EPITECH PROJECT, 2026
** MY_GZDOOM
** File description:
** ChooseLevel
*/
#include "Core.hpp"
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Maps.hpp"
#include "Game.hpp"

void Core::displayLevels()
{
    Maps *clone = maps;

    while (clone != nullptr) {
        sfText *text = sfText_create();
        sfText_setFont(text, menu->getFont());
        sfText_setString(text, clone->getName());
        sfText_setCharacterSize(text, 24);
        sfFloatRect textRect = sfText_getLocalBounds(text);
        sfText_setOrigin(text, {textRect.width / 2, textRect.height / 2});
        sfVector2u winSize = sfRenderWindow_getSize(window->getWindow());
        sfText_setPosition(text, {(float)winSize.x / 2, (float)winSize.y / 4 + (float)(winSize.y / 4) * (maps->getMapArray() ? 0 : 1)});
        sfRenderWindow_drawText(window->getWindow(), text, nullptr);
        sfText_destroy(text);
        clone = clone->next;
    }
}

int Core::chooseLevel(void)
{
    Game game;
    Maps *maps = new Maps(gameData->game_array, window->isDebug());
    setMaps(maps);
    Maps *selected = maps;

    while (sfRenderWindow_isOpen(window->getWindow())) {
        sfEvent event;
        sfRenderWindow_clear(window->getWindow(), sfBlack);
        while (sfRenderWindow_pollEvent(window->getWindow(), &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window->getWindow());
            if (event.type == sfEvtKeyPressed) {
                if (event.key.code == sfKeyEscape)
                    return menu_return();
                if (event.key.code == sfKeyUp) {
                    if (selected->prev) selected = selected->prev;
                } else if (event.key.code == sfKeyDown) {
                    if (selected->next) selected = selected->next;
                } else if (event.key.code == sfKeyEnter || event.key.code == sfKeyReturn) {
                    if (window->isDebug())
                        std::cout << "Selected level: " << selected->getName() << std::endl;
                    return game.Play(this);
                }
            }
        }
        displayLevels();
        sfRenderWindow_display(window->getWindow());
    }
    return 0;
}

int Core::menu_return(void)
{
    sfMusic_play(menu->getMusic());
    if (window->isDebug())
        std::cout << "Returning to menu..." << std::endl;
    while (sfRenderWindow_isOpen(window->getWindow()))
    {
        sfEvent event;
        int buttonId = 0;

        while (sfRenderWindow_pollEvent(window->getWindow(), &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window->getWindow());
            if (event.type == sfEvtMouseButtonPressed) {
                sfVector2i mousePos = sfMouse_getPositionRenderWindow(window->getWindow());
                buttonId = menu->handleClick(mousePos.x, mousePos.y);
                if (buttonId == 1) {
                    sfMusic_stop(menu->getMusic());
                    if (window->isDebug())
                        std::cout << "Starting game..." << std::endl;
                    return this->chooseLevel();
                } else if (buttonId == 2) {
                    if (window->isDebug())
                        std::cout << "Opening options..." << std::endl;
                    settings->changeSettings(*window, *menu);
                } else if (buttonId == 3) {
                    sfMusic_stop(menu->getMusic());
                    if (window->isDebug())
                        std::cout << "Quitting..." << std::endl;
                    sfRenderWindow_close(window->getWindow());
                }
            }
        }
        sfRenderWindow_clear(window->getWindow(), sfBlack);
        menu->display(window->getWindow(), buttonId);
        if (sfKeyboard_isKeyPressed(sfKeyQ))
            sfRenderWindow_close(window->getWindow());
        sfRenderWindow_display(window->getWindow());
    }
    return 0;
}
