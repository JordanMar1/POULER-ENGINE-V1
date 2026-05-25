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
#include "math.h"
#include "Head.hpp"
Core::~Core()
{
}

void Core::displayLevels(Maps* selected)
{
    sfVector2u winSize = sfRenderWindow_getSize(window->getWindow());
    int totalItems = 0;
    int selectedIndex = 0;
    Maps *temp = maps;
    while (temp != nullptr) {
        if (temp == selected) {
            selectedIndex = totalItems;
        }
        totalItems++;
        temp = temp->next;
    }
    float spacing = 60.0f;
    float startY = (float)winSize.y / 4.0f;
    float centerY = (float)winSize.y / 2.0f;
    float scrollOffset = 0.0f;
    float idealSelectedY = startY + (selectedIndex * spacing);
    if (idealSelectedY > centerY) {
        scrollOffset = idealSelectedY - centerY;
    }
    float maxScroll = (totalItems * spacing) + startY - (winSize.y * 0.8f);
    if (maxScroll < 0.0f) maxScroll = 0.0f; 

    if (scrollOffset > maxScroll) {
        scrollOffset = maxScroll;
    }
    Maps *clone = maps;
    int i = 0;
    while (clone != nullptr) {
        sfText *text = sfText_create();
        sfText_setFont(text, menu->getFont());
        sfText_setString(text, clone->getName());
        sfText_setCharacterSize(text, 24);
        if (clone == selected) {
            sfText_setFillColor(text, sfRed); 
        } else {
            sfText_setFillColor(text, sfWhite);
        }

        sfFloatRect textRect = sfText_getLocalBounds(text);
        sfText_setOrigin(text, {textRect.width / 2.0f, textRect.height / 2.0f});
        float posX = (float)winSize.x / 2.0f;
        float posY = startY + (i * spacing) - scrollOffset;
        sfText_setPosition(text, {posX, posY});
        sfRenderWindow_drawText(window->getWindow(), text, nullptr);
        sfText_destroy(text);
        clone = clone->next;
        i++;
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
                    setMaps(selected);
                    return game.Play(this, selected);
                }
            }
        }
        displayLevels(selected); 
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
            sfVector2i mousePos = sfMouse_getPositionRenderWindow(window->getWindow());
            if (event.type == sfEvtMouseButtonPressed) {
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
                    menu->setMusicVolume(settings->musicVolume);
                    getPlayer()->planeY = tan((getSettings()->Pfov * M_PI / 180.0) / 2.0);
                    sfMusic_play(menu->getMusic());
                } else if (buttonId == 3) {
                    sfMusic_stop(menu->getMusic());
                    if (window->isDebug())
                        std::cout << "Quitting..." << std::endl;
                    sfRenderWindow_close(window->getWindow());
                }
            }
            menu->handleHover(mousePos.x, mousePos.y);
        }
        sfRenderWindow_clear(window->getWindow(), sfBlack);
        menu->display(window->getWindow(), buttonId);
        if (sfKeyboard_isKeyPressed(sfKeyQ))
            sfRenderWindow_close(window->getWindow());
        sfRenderWindow_display(window->getWindow());
        sfRenderWindow_setFramerateLimit(window->getWindow(), this->getSettings()->fps);
    }
    return 0;
}
