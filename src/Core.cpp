/*
** EPITECH PROJECT, 2026
** MY_GZDOOM
** File description:
** ChooseLevel
*/
#include "Core.hpp"
#include <iostream>
#include "Maps.hpp"
#include "Game.hpp"
#include "math.h"
#include "string.h"

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

static double distanceTo(double ax, double ay, double bx, double by)
{
    double dx = bx - ax;
    double dy = by - ay;
    return sqrt(dx * dx + dy * dy);
}

static bool isWalkable(char **map, int mapRows, int x, int y)
{
    if (y < 0 || y >= mapRows || !map[y] || x < 0 || (size_t)x >= strlen(map[y]))
        return false;
    return map[y][x] != 'W';
}

int Core::getMapHeightForEnemy(int mx, int my, int map_rows) const
{
    char **map = getMaps()->getMapArray();
    int **heatmap = getMaps()->getHeatmap();
    if (my < 0 || my >= map_rows || mx < 0 || !map[my] || (size_t)mx >= strlen(map[my]))
        return 0;
    if (map[my][mx] == 'W') return 99;
    int h = heatmap[my][mx];
    return (h < 0) ? 0 : h;
}

void Core::updateEnemyState(ActiveEnemy &e, Player &p, float dt, int map_rows)
{
    if (!std::isfinite(e.x) || !std::isfinite(e.y)) {
        e.x = 1.5;
        e.y = 1.5;
        e.state = 0;
        e.animTimer = 0.0f;
    }

    Enemy *tpl = e.templateData;
    e.animTimer += dt;
    double dist = distanceTo(e.x, e.y, p.x, p.y);
    float fireDist = tpl->getDist();
    float moveSpeed = 1.5f;
    char **map = getMaps()->getMapArray();

    switch (e.state) {
        case 0:
            if (dist <= fireDist) {
                if (e.ammo > 0) {
                    e.state = 1;
                    e.animTimer = 0.0f;
                } else {
                    e.state = 3;
                    e.animTimer = 0.0f;
                }
            } else {
                e.state = 2;
                e.animTimer = 0.0f;
            }
            break;

        case 1:
            if (e.animTimer >= tpl->getReshootTime()) {
                e.ammo--;
                p.hp -= tpl->getDmg();
                if (e.ammo <= 0) {
                    e.state = 3;
                    e.animTimer = 0.0f;
                } else {
                    e.state = 0;
                    e.animTimer = 0.0f;
                }
            }
            break;

        case 3:
            if (e.animTimer >= tpl->getReloadTime()) {
                e.ammo = tpl->getAmmo();
                e.state = 0;
                e.animTimer = 0.0f;
            }
            break;

        case 2: {
            if (dist <= fireDist) {
                e.state = 0;
                e.animTimer = 0.0f;
                e.stuckTimer = 0.0f;
                break;
            }
            if (dist <= 0.0001 || !std::isfinite(dist)) break;

            double baseAngle = atan2(p.y - e.y, p.x - e.x);
            const double offsets[] = { 0.0, M_PI / 6.0, M_PI / 3.0, M_PI / 2.0, 2.0 * M_PI / 3.0 };

            int curH = getMapHeightForEnemy((int)e.x, (int)e.y, map_rows);
            bool moved = false;

            for (double off : offsets) {
                double signedOff = e.wanderRight ? off : -off;
                double angle = baseAngle + signedOff;
                double nextX = e.x + cos(angle) * moveSpeed * dt;
                double nextY = e.y + sin(angle) * moveSpeed * dt;

                if (!std::isfinite(nextX) || !std::isfinite(nextY))
                    continue;

                int nhX = getMapHeightForEnemy((int)nextX, (int)e.y, map_rows);
                int nhY = getMapHeightForEnemy((int)e.x, (int)nextY, map_rows);

                bool okX = isWalkable(map, map_rows, (int)nextX, (int)e.y) && (nhX - curH <= 1) && (nhX != 99);
                bool okY = isWalkable(map, map_rows, (int)e.x, (int)nextY) && (nhY - curH <= 1) && (nhY != 99);

                if (okX || okY) {
                    if (okX) e.x = nextX;
                    if (okY) e.y = nextY;
                    moved = true;
                    break;
                }
            }

            if (!moved) {
                e.stuckTimer += dt;
                if (e.stuckTimer > 0.4f) {
                    e.wanderRight = !e.wanderRight;
                    e.stuckTimer = 0.0f;
                }
            } else {
                e.stuckTimer = 0.0f;
            }
            break;
        }
    }
}

void UpdateEnemies(std::vector<ActiveEnemy> &enemies, Player &p, float dt, Core *core, int map_rows)
{
    for (auto &e : enemies) {
        if (!e.isAlive)
            continue;
        core->updateEnemyState(e, p, dt, map_rows);
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
