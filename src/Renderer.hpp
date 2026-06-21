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
#include <vector>
#include "ActiveEnemy.hpp"
class Core;

class Renderer {
    public:
        Renderer(sfVector2u size);
        ~Renderer();
        void resize(sfVector2u newSize);
        void initFrameRender(Core* core, Player p, int map_rows);
        void drawScene(sfRenderWindow* window, float lean);
        sfSprite* getSprite() { return m_sprite; }
        void drawEnemies(sfRenderWindow* window, Core* core, const Player& p,
                          std::vector<ActiveEnemy>& enemies,
                          std::vector<sfTexture*>& enemyTextures, int map_rows);
    private:
        sfUint8* m_pixels;
        sfTexture* m_texture;
        sfSprite* m_sprite;
        sfVector2u m_size;
        sfSprite *m_enemySprite;
        std::future<void> m_renderFuture;
        bool m_isRendering;
        std::vector<double> m_depthBuffer;
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
