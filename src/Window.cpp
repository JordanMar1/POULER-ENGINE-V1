/*
** EPITECH PROJECT, 2026
** POULER OR NOT
** File description:
** Window
*/

#include "Window.hpp"
#include "Settings.hpp"
#include <iostream>

Window::Window(char *windowtitle, bool debug)
{
    _debug = debug;
    Settings settings;
    if (_debug)
        std::cout << "Window created with title '" << windowtitle << "'" << std::endl;
    _vm = {(unsigned int)settings.windowWidth, (unsigned int)settings.windowHeight, 32};
    if (_debug)
        std::cout << "Video mode set to " << _vm.width << "x" << _vm.height << std::endl;
    _window = sfRenderWindow_create(_vm, windowtitle, sfDefaultStyle, NULL);
    if (!_window) {
        std::cerr << "Error: Could not create window" << std::endl;
        exit(84);
    } else if (_debug)
        std::cout << "Window successfully created" << std::endl;
    _text = sfTexture_create(_vm.width, _vm.height);
    if (!_text && _debug) {
        std::cerr << "Error: Could not create texture" << std::endl;
        exit(84);
    } else if (_debug)
        std::cout << "Texture successfully created" << std::endl;
    _sprite = sfSprite_create();
    if (!_sprite && _debug) {
        std::cerr << "Error: Could not create sprite" << std::endl;
        exit(84);
    } else if (_debug)
        std::cout << "Sprite successfully created" << std::endl;
    _event = sfEvent();
    _sound_muted = 0;
    _music_muted = 0;
}

Window::~Window()
{
    if (_debug)
        std::cout << "Destroying window and its resources" << std::endl;
    if (_debug)
        std::cout << "Window destroyed" << std::endl;
    sfTexture_destroy(_text);
    if (_debug)
        std::cout << "Texture destroyed" << std::endl;
    sfSprite_destroy(_sprite);
    if (_debug)
        std::cout << "Sprite destroyed" << std::endl;
    if (getWindow())
        sfRenderWindow_destroy(_window);
}
