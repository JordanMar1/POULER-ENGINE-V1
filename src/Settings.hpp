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
        int fps = 60;
        unsigned int windowWidth  = 1920;
        unsigned int windowHeight = 1080;
    private:
        void updateTexts(sfText** textArray);
        void saveSettings();
};