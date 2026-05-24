/*
** EPITECH PROJECT, 2026
** MY_GZDOOM
** File description:
** Settings
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
            sfKeyCode leanLeft = sfKeyA;
            sfKeyCode leanRight = sfKeyE;
            sfMouseButton shoot = sfMouseLeft;
            sfMouseButton aim = sfMouseRight;
        } binds;
        float horizontal_sensitivity;
        float vertical_sensitivity;
        Settings();
        ~Settings();
        void changeSettings(Window& window, Menu& menu);
    private:
        void updateTexts(sfText* mf, sfText* ml, sfText* mb, sfText* mr, sfText* c, sfText* s, sfText* a, sfText* r, sfText* hSens, sfText* vSens);
        void saveSettings();
};