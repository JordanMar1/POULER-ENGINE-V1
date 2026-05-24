/*
** EPITECH PROJECT, 2026
** POULER OR NOT
** File description:
** Window
*/

#pragma once

#include <SFML/Audio.h>
#include <SFML/Graphics.h>
#include <SFML/System.h>

class Window {
    public:
        Window(char *windowtitle, bool debug);
        ~Window();

        void setWindow(sfRenderWindow *window) {_window = window; }
        sfRenderWindow* getWindow() const { return _window; }
        sfVideoMode getVm() const { return _vm; }
        sfTexture* getText() const { return _text; }
        sfSprite* getSprite() const { return _sprite; }
        const sfEvent& getEvent() const { return _event; }
        int getMusicMuted() const { return _music_muted; }
        int getSoundMuted() const { return _sound_muted; }
        bool isDebug() const { return _debug; }
    private:
        sfRenderWindow *_window;
        sfVideoMode _vm;
        sfTexture *_text;
        sfSprite *_sprite;
        sfEvent _event;
        int _music_muted;
        int _sound_muted;
        bool _debug;
};
