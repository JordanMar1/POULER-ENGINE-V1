/*
** EPITECH PROJECT, 2026
** POULER OR NOT
** File description:
** Setting
*/

#pragma once

#include <SFML/Window.h>
#include <SFML/Graphics.h>

class Window;
class Menu;

class Settings {
    public:
        struct Binds {
            sfKeyCode moveForward = sfKeyZ;
            sfKeyCode moveLeft = sfKeyQ;
            sfKeyCode moveBack = sfKeyS;
            sfKeyCode moveRight = sfKeyD;
            sfKeyCode crouch = sfKeyLControl;
            sfKeyCode reload = sfKeyR;
            sfMouseButton shoot = sfMouseLeft;
            sfMouseButton aim = sfMouseRight;
        } binds;
        Settings();
        ~Settings();
        void changeSettings(Window& window, Menu& menu);

    protected:
    private:
        void updateTexts(sfText* mf, sfText* ml, sfText* mb, sfText* mr, sfText* s, sfText* a, sfText* c, sfText* r);
        void saveSettings();
};
