/*
** EPITECH PROJECT, 2026
** POULER OR NOT
** File description:
** Menu
*/

#include "Menu.hpp"
#include "../include/array_lib.hpp"
#include <iostream>
#include <cstdlib>
#include <cstring>

Menu::Menu(char ***parsed_file, bool debug)
{
    button_list *button = new button_list();

    buttons = button;
    sprite = nullptr;
    texture = nullptr;
    music = nullptr;
    font = nullptr;
    if (parsed_file == nullptr) {
        std::cerr << "Error: Parsed file is null" << std::endl;
        return;
    }
    for (int i = 0; parsed_file[i] != NULL; i++) {
        if (strcmp(parsed_file[i][0], "menu") == 0) {
            for (int j = 1; parsed_file[i][j] != NULL; j++) {
                if (strcmp(parsed_file[i][j], "sprite") == 0 && parsed_file[i][j + 1] != NULL) {
                    texture = sfTexture_createFromFile(parsed_file[i][j + 1], NULL);
                    if (texture != nullptr) {
                        sprite = sfSprite_create();
                        sfSprite_setTexture(sprite, texture, sfTrue);
                    } else {
                        std::cerr << "Failed to load texture: " << parsed_file[i][j + 1] << std::endl;
                    }
                }
                if (strcmp(parsed_file[i][j], "music") == 0 && parsed_file[i][j + 1] != NULL) {
                    music = sfMusic_createFromFile(parsed_file[i][j + 1]);
                    if (debug)
                        printf("Playing music: %s\n", parsed_file[i][j + 1]);
                    sfMusic_setLoop(music, true);
                    sfMusic_play(music);
                }
                if (strcmp(parsed_file[i][j], "font") == 0 && parsed_file[i][j + 1] != NULL) {
                    font = sfFont_createFromFile(parsed_file[i][j + 1]);
                    if (debug)
                        printf("Loaded font: %s\n", parsed_file[i][j + 1]);
                }
            }
        }
        if (strcmp(parsed_file[i][0], "menu_button") == 0) {
            button_list *but = buttons;
            while (but->get_next() != nullptr)
                but = but->get_next();
            button_list *new_but = new button_list();
            but->set_next(new_but);
            but = new_but;
            but->set_scale(1.0f);
            for (int j = 1; parsed_file[i][j] != NULL; j++) {
                if (strcmp(parsed_file[i][j], "x") == 0 && parsed_file[i][j + 1] != NULL)
                    but->set_x(atoi(parsed_file[i][j + 1]));
                if (strcmp(parsed_file[i][j], "y") == 0 && parsed_file[i][j + 1] != NULL)
                    but->set_y(atoi(parsed_file[i][j + 1]));
                if (strcmp(parsed_file[i][j], "r") == 0 && parsed_file[i][j + 1] != NULL)
                    but->set_r(atoi(parsed_file[i][j + 1]));
                if (strcmp(parsed_file[i][j], "g") == 0 && parsed_file[i][j + 1] != NULL)
                    but->set_g(atoi(parsed_file[i][j + 1]));
                if (strcmp(parsed_file[i][j], "b") == 0 && parsed_file[i][j + 1] != NULL)
                    but->set_b(atoi(parsed_file[i][j + 1]));
                if (strcmp(parsed_file[i][j], "width") == 0 && parsed_file[i][j + 1] != NULL)
                    but->set_width(atoi(parsed_file[i][j + 1]));
                if (strcmp(parsed_file[i][j], "height") == 0 && parsed_file[i][j + 1] != NULL)
                    but->set_height(atoi(parsed_file[i][j + 1]));
                if (strcmp(parsed_file[i][j], "text") == 0 && parsed_file[i][j + 1] != NULL)
                    but->set_text(parsed_file[i][j + 1]);
                if (strcmp(parsed_file[i][j], "text_r") == 0 && parsed_file[i][j + 1] != NULL)
                    but->set_text_r(atoi(parsed_file[i][j + 1]));
                if (strcmp(parsed_file[i][j], "text_g") == 0 && parsed_file[i][j + 1] != NULL)
                    but->set_text_g(atoi(parsed_file[i][j + 1]));
                if (strcmp(parsed_file[i][j], "text_b") == 0 && parsed_file[i][j + 1] != NULL)
                    but->set_text_b(atoi(parsed_file[i][j + 1]));
                if (strcmp(parsed_file[i][j], "text_s") == 0 && parsed_file[i][j + 1] != NULL)
                    but->set_text_s(atoi(parsed_file[i][j + 1]));
                if (strcmp(parsed_file[i][j], "id") == 0 && parsed_file[i][j + 1] != NULL)
                    but->set_id(atoi(parsed_file[i][j + 1]));
                if (strcmp(parsed_file[i][j], "scale") == 0 && parsed_file[i][j + 1] != NULL) {
                    but->set_scale(atof(parsed_file[i][j + 1]));
                }
            }
        }
    }
}


Menu::~Menu()
{
    if (sprite != nullptr)
        sfSprite_destroy(sprite);
    if (texture != nullptr)
        sfTexture_destroy(texture);
    if (music != nullptr)
        sfMusic_destroy(music);
    if (font != nullptr)
        sfFont_destroy(font);
    if (buttons != nullptr)
        delete buttons;
}

void Menu::display(sfRenderWindow *window, int buttonId)
{
    if (sprite != nullptr && texture != nullptr)
        sfRenderWindow_drawSprite(window, sprite, NULL);
    if (buttons != nullptr && buttonId != 2) {
        button_list *current = buttons->get_next();
        while (current != nullptr) {
            current->draw(window, font);
            current = current->get_next();
        }
    }
}

int Menu::handleClick(int mouseX, int mouseY)
{
    if (buttons != nullptr) {
        button_list *current = buttons->get_next();
        while (current != nullptr) {
            if (mouseX >= current->get_x() && mouseX <= current->get_x() + current->get_width() &&
                mouseY >= current->get_y() && mouseY <= current->get_y() + current->get_height()) {
                return current->get_id();
            }
            current = current->get_next();
        }
    }
    return -1;
}

void Menu::handleHover(int x, int y)
{
    if (buttons == nullptr) return;

    button_list *current = buttons->get_next();
    while (current != nullptr) {
        bool isHovered = (x >= current->get_x() && x <= current->get_x() + current->get_width() &&
                          y >= current->get_y() && y <= current->get_y() + current->get_height());
        
        if (isHovered) {
            sfRectangleShape_setScale(current->get_shape(), (sfVector2f){current->get_scale(), current->get_scale()});
        } else {
            sfRectangleShape_setScale(current->get_shape(), (sfVector2f){1.0f, 1.0f});
        }
        current = current->get_next();
    }
}

button_list::button_list()
{
    Buttons = sfRectangleShape_create();
    x = 0;
    y = 0;
    width = 0;
    height = 0;
    r = 255;
    g = 255;
    b = 255;
    text = nullptr;
    text_r = 255;
    text_g = 255;
    text_b = 255;
    next = nullptr;
    id = 0;
}

button_list::~button_list()
{
    if (Buttons != nullptr)
        sfRectangleShape_destroy(Buttons);
    if (text != nullptr)
        free(text);
    if (next != nullptr)
        delete next;
}

button_list *button_list::get_next()
{
    return next;
}

void button_list::set_next(button_list *next)
{
    this->next = next;
}

void button_list::set_id(int id)
{
    this->id = id;
};

void button_list::set_x(int x)
{
    this->x = x;
}

void button_list::set_y(int y)
{
    this->y = y;
}

void button_list::set_width(int width)
{
    this->width = width;
}

void button_list::set_height(int height)
{
    this->height = height;
}

void button_list::set_st(bool state)
{
    this->state = state;
}

void button_list::set_r(int r)
{
    this->r = r;
}

void button_list::set_g(int g)
{
    this->g = g;
}

void button_list::set_b(int b)
{
    this->b = b;
}

void button_list::set_text(const char *text)
{
    if (this->text != nullptr)
        free(this->text);
    if (text == nullptr) {
        this->text = nullptr;
        return;
    }
    size_t len = strlen(text);
    this->text = (char *)malloc(len + 1);
    if (this->text != nullptr) {
        strcpy(this->text, text);
    }
}

void button_list::set_text_r(int r)
{
    this->text_r = r;
}

void button_list::set_text_g(int g)
{
    this->text_g = g;
}

void button_list::set_scale(float s)
{
    this->scale = s;
}

void button_list::set_text_s(int ts)
{
    this->text_s = ts;
}

void button_list::set_text_b(int b)
{
    this->text_b = b;
}

void button_list::draw(sfRenderWindow *window, sfFont *font)
{
    if (Buttons != nullptr) {
        sfVector2f pos = {(float)x, (float)y};
        sfVector2f size = {(float)width, (float)height};
        sfRectangleShape_setPosition(Buttons, pos);
        sfRectangleShape_setSize(Buttons, size);
        sfRectangleShape_setFillColor(Buttons, sfColor_fromRGB(r, g, b));
        sfRenderWindow_drawRectangleShape(window, Buttons, NULL);
        if (font != nullptr && text != nullptr) {
            sfText *sf_text = sfText_create();
            sfText_setFont(sf_text, font);
            sfText_setString(sf_text, text);
            sfText_setCharacterSize(sf_text, text_s);
            sfText_setFillColor(sf_text, sfColor_fromRGB(text_r, text_g, text_b));
            sfVector2f scale = sfRectangleShape_getScale(Buttons);
            sfFloatRect bounds = sfText_getLocalBounds(sf_text);
            sfVector2f center = {
                pos.x + (size.x * scale.x) / 2.0f,
                pos.y + (size.y * scale.y) / 2.0f
            };
            sfText_setOrigin(sf_text, {bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f});
            sfText_setPosition(sf_text, center);
            sfRenderWindow_drawText(window, sf_text, NULL);
            sfText_destroy(sf_text);
        }
    }
}