/*
** EPITECH PROJECT, 2026
** POULER-ENGINE-V1.1
** File description:
** Renderer
*/

#pragma once
#include <SFML/Graphics.h>
#include <future>
#include "Player.hpp"

class Core;

class Renderer {
    public:
        Renderer(sfVector2u size);
        ~Renderer();
        void resize(sfVector2u newSize);
        void initFrameRender(Core* core, Player p, int map_rows);
        void drawScene(sfRenderWindow* window, float lean);

        sfSprite* getSprite() { return m_sprite; }
    private:
        sfUint8* m_pixels;
        sfTexture* m_texture;
        sfSprite* m_sprite;
        sfVector2u m_size;
        std::future<void> m_renderFuture;
        bool m_isRendering;

        struct Ray {
            double posX, posY;
            double dirX, dirY;
            int mapX, mapY;
            double deltaX, deltaY;
            int stepX, stepY;
            double sideX, sideY;
        };
        void clearScreen(int horizon);
        void setupRay(const Player& p, double cameraX, Ray& ray) const;
        int  getMapHeight(Core* core, int mx, int my, int map_rows) const;
        void renderColumn(Core* core, int x, const Player& p, int map_rows, int horizon);
};
