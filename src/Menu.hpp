/*
** EPITECH PROJECT, 2026
** POULER OR NOT
** File description:
** Menu
*/

#pragma once

#include <SFML/Audio.h>
#include <SFML/Graphics.h>
#include <SFML/System.h>

class button_list {
    public:
        button_list();
        ~button_list();
        button_list *get_next();
        void set_next(button_list *next);
        void set_x(int x);
        void set_y(int y);
        void set_width(int width);
        void set_height(int height);
        void set_st(bool state);
        void set_r(int r);
        void set_g(int g);
        void set_b(int b);
        void set_text(const char *text);
        void set_text_r(int r);
        void set_text_g(int g);
        void set_text_b(int b);
        const char *get_text() const {
            return text != nullptr ? text : "";
        };
        int get_text_r() const {
            return text_r;
        };
        int get_text_g() const {
            return text_g;
        };
        int get_text_b() const {
            return text_b;
        };
        void draw(sfRenderWindow *window, sfFont *font);
        int get_x() const {
            return x;
        };
        int get_y() const {
            return y;
        };
        int get_width() const {
            return width;
        };
        int get_height() const {
            return height;
        };
        bool get_st() const {
            return state;
        };
        int get_r() const {
            return r;
        };
        int get_g() const {
            return g;
        };
        int get_b() const {
            return b;
        };
        int get_id() const {
            return id;
        };
        void set_id(int id) {
            this->id = id;
        };
    private:
        sfRectangleShape *Buttons;
        button_list *next = nullptr;
        int x;
        int y;
        int width;
        int height;
        bool state;
        int r;
        int g;
        int b;
        char *text;
        int text_r;
        int text_g;
        int text_b;
        int id;
};


class Menu {
    public:
        void setMusicVolume(int volume) {sfMusic_setVolume(music, (float)volume);}
        Menu(char ***parsed_file, bool debug);
        ~Menu();
        void display(sfRenderWindow *window, int buttonId);
        int handleClick(int mouseX, int mouseY);
        sfFont *getFont() { return font; }
        sfMusic *getMusic() { return music; }
    private:
        sfSprite *sprite;
        sfTexture *texture;
        sfMusic *music;
        sfFont *font;
        button_list *buttons;
        int action_id = 0;
};
