/*
** EPITECH PROJECT, 2026
** POULER-ENGINE-V1.1
** File description:
** Renderer
*/

#include "Renderer.hpp"
#include "Core.hpp"
#include <math.h>
#include <algorithm>
#include <cstring>

Renderer::Renderer(sfVector2u size) : m_pixels(nullptr), m_texture(nullptr), m_sprite(nullptr), m_isRendering(false)
{
    m_sprite = sfSprite_create();
    m_enemySprite = sfSprite_create();
    resize(size);
}

Renderer::~Renderer()
{
    if (m_renderFuture.valid()) m_renderFuture.wait();
    delete[] m_pixels;
    sfTexture_destroy(m_texture);
    sfSprite_destroy(m_sprite);
    sfSprite_destroy(m_enemySprite);
}

void Renderer::resize(sfVector2u newSize)
{
    if (m_renderFuture.valid()) m_renderFuture.wait();

    m_size = newSize;
    delete[] m_pixels;
    if (m_texture) sfTexture_destroy(m_texture);

    m_pixels = new sfUint8[m_size.x * m_size.y * 4];
    m_texture = sfTexture_create(m_size.x, m_size.y);
    sfSprite_setTexture(m_sprite, m_texture, sfTrue);
    m_depthBuffer.assign(m_size.x, 1e9);
}

int Renderer::getMapHeight(Core* core, int mx, int my, int map_rows) const
{
    char **map = core->getMaps()->getMapArray();
    int **heatmap = core->getMaps()->getHeatmap();

    if (my < 0 || my >= map_rows)
        return 0;
    if (!map[my])
        return 0;
    if (mx < 0)
        return 0;
    size_t len = strlen(map[my]);
    if ((size_t)mx >= len)
        return 0;
    if (map[my][mx] == 'W') return 99;
    int h = heatmap[my][mx];
    return (h < 0) ? 0 : h;
}

void Renderer::clearScreen(int horizon)
{
    int totalPixels = m_size.x * m_size.y * 4;
    for (int i = 0; i < totalPixels; i += 4) {
        int pixelY = (i / 4) / m_size.x;
        m_pixels[i]   = (pixelY < horizon) ? 30 : 50;
        m_pixels[i+1] = (pixelY < horizon) ? 30 : 50;
        m_pixels[i+2] = (pixelY < horizon) ? 30 : 50;
        m_pixels[i+3] = 255;
    }
}

void Renderer::setupRay(const Player& p, double cameraX, Ray& ray) const
{
    double leanSideX = -p.dirY * p.lean * 0.3;
    double leanSideY = p.dirX * p.lean * 0.3;
    ray.posX = p.x + leanSideX;
    ray.posY = p.y + leanSideY;
    ray.dirX = p.dirX + p.planeX * cameraX;
    ray.dirY = p.dirY + p.planeY * cameraX;
    ray.mapX = (int)ray.posX;
    ray.mapY = (int)ray.posY;
    ray.deltaX = fabs(1.0 / ray.dirX);
    ray.deltaY = fabs(1.0 / ray.dirY);
    if (ray.dirX < 0) {
        ray.stepX = -1;
        ray.sideX = (ray.posX - ray.mapX) * ray.deltaX;
    } else {
        ray.stepX = 1;
        ray.sideX = (ray.mapX + 1.0 - ray.posX) * ray.deltaX;
    }
    if (ray.dirY < 0) {
        ray.stepY = -1;
        ray.sideY = (ray.posY - ray.mapY) * ray.deltaY;
    } else {
        ray.stepY = 1;
        ray.sideY = (ray.mapY + 1.0 - ray.posY) * ray.deltaY;
    }
}

void Renderer::renderColumn(Core* core, int x, const Player& p, int map_rows, int horizon)
{
    double cameraX = 2.0 * x / (double)m_size.x - 1.0;
    Ray ray;
    setupRay(p, cameraX, ray);
    int screen_floor_limit = m_size.y;
    int playerH = getMapHeight(core, (int)p.x, (int)p.y, map_rows);
    int lastH = getMapHeight(core, ray.mapX, ray.mapY, map_rows);
    int steps = 0;
    int side = 0;
    bool hadTransition = (lastH != playerH);
    m_depthBuffer[x] = 1e9;
    while (steps++ < 64) {
        if (ray.sideX < ray.sideY) {
            ray.sideX += ray.deltaX;
            ray.mapX += ray.stepX;
            side = 0;
        } else {
            ray.sideY += ray.deltaY;
            ray.mapY += ray.stepY;
            side = 1;
        }
        double dist = (side == 0) ? (ray.sideX - ray.deltaX) : (ray.sideY - ray.deltaY);
        if (dist < 0.01) dist = 0.01;
        int curH = getMapHeight(core, ray.mapX, ray.mapY, map_rows);
        double heightScale = (double)m_size.y / dist;
        int drawFloorH = (hadTransition || curH != lastH) ? lastH : playerH;
        int floor_px = (int)(horizon - (drawFloorH - p.eyeHeight) * heightScale);
        if (floor_px < screen_floor_limit) {
            int yStart = std::max(0, floor_px);
            int yEnd   = screen_floor_limit;
            for (int y = yStart; y < yEnd; y++) {
                int dy = horizon - y;
                double rowDist = (dy != 0)
                    ? fabs((p.eyeHeight - (double)drawFloorH) * (double)m_size.y / (double)dy)
                    : 1000.0;
                float rowShade = std::min(1.0f, 14.0f / (float)(rowDist + 1.0f));
                sfUint8 rowColor = (sfUint8)std::max(0, (int)((38 + drawFloorH * 9) * rowShade));
                int idx = (y * m_size.x + x) * 4;
                m_pixels[idx] = m_pixels[idx+1] = m_pixels[idx+2] = rowColor;
            }
            screen_floor_limit = std::max(0, floor_px);
        }
        if (curH != lastH) {
            hadTransition = true;
            double wallTopH = (curH == 99) ? (double)lastH + 3.0 : (double)std::max(curH, lastH);
            double wallBotH = (double)std::min(curH, lastH);
            int pxTop = (int)(horizon - (wallTopH - p.eyeHeight) * heightScale);
            int pxBot = (int)(horizon - (wallBotH - p.eyeHeight) * heightScale);
            int drawTop = std::max(pxTop, 0);
            int drawBot = std::min(pxBot, screen_floor_limit);
            if (drawTop < drawBot) {
                float dShade = std::min(1.0f, 12.0f / (float)(dist + 1.0f));
                for (int y = drawTop; y < drawBot; y++) {
                    float fGrad = (pxBot > pxTop) ? (float)(y - pxTop) / (float)(pxBot - pxTop) : 0.0f;
                    int val = (curH == 99) ? (145 - fGrad * 65) : (135 - fGrad * 55);
                    val = (int)(std::max(0, std::min(255, val)) * dShade);
                    if (side == 1) val = val * 7 / 10;
                    int idx = (y * m_size.x + x) * 4;
                    m_pixels[idx] = m_pixels[idx+1] = m_pixels[idx+2] = (sfUint8)val;
                }
                if (curH > lastH) {
                    m_depthBuffer[x] = std::min(m_depthBuffer[x], dist);
                }
                if (curH == 99) {
                    m_depthBuffer[x] = dist;
                    break;
                }
            }
        }
        lastH = curH;
    }
}

void Renderer::initFrameRender(Core* core, Player p, int map_rows)
{
    if (m_isRendering && m_renderFuture.valid()) {
        m_renderFuture.wait();
    }

    m_isRendering = true;
    m_renderFuture = std::async(std::launch::async, [this, core, p, map_rows]() {
        int horizon = (int)(m_size.y * 0.5 + p.pitch);
        clearScreen(horizon);
        for (int x = 0; x < (int)m_size.x; x++) {
            renderColumn(core, x, p, map_rows, horizon);
        }
    });
}

void Renderer::drawScene(sfRenderWindow* window, float lean)
{
    if (m_isRendering && m_renderFuture.valid()) {
        m_renderFuture.wait();
        m_isRendering = false;
    }

    sfTexture_updateFromPixels(m_texture, m_pixels, m_size.x, m_size.y, 0, 0);

    float angle_rad = lean * -0.08f;
    float angle_deg = angle_rad * 180.0f / 3.14159265f;

    sfSprite_setOrigin(m_sprite, (sfVector2f){ m_size.x / 2.0f, m_size.y / 2.0f });
    sfSprite_setPosition(m_sprite, (sfVector2f){ m_size.x / 2.0f, m_size.y / 2.0f });
    sfSprite_setRotation(m_sprite, angle_deg);

    float maxZoom = 1.125f / cos(fabs(angle_rad));
    float zoom = 1.0f + (maxZoom - 1.0f) * fabs(lean);
    sfSprite_setScale(m_sprite, (sfVector2f){ zoom, zoom });

    sfRenderWindow_drawSprite(window, m_sprite, nullptr);
}

void Renderer::drawEnemies(sfRenderWindow* window, Core* core, const Player& p,
                            std::vector<ActiveEnemy>& enemies,
                            std::vector<sfTexture*>& enemyTextures, int map_rows)
{
    std::vector<ActiveEnemy*> visible;
    for (auto &e : enemies) {
        if (e.isAlive) visible.push_back(&e);
    }
    std::sort(visible.begin(), visible.end(), [&](ActiveEnemy* a, ActiveEnemy* b) {
        double da = (a->x - p.x) * (a->x - p.x) + (a->y - p.y) * (a->y - p.y);
        double db = (b->x - p.x) * (b->x - p.x) + (b->y - p.y) * (b->y - p.y);
        return da > db;
    });
    double invDet = 1.0 / (p.planeX * p.dirY - p.dirX * p.planeY);
    Enemy **templates = core->getEnemies();
    for (auto* ePtr : visible) {
        ActiveEnemy &e = *ePtr;
        double ex = std::isfinite(e.x) ? e.x : 0.0;
        double ey = std::isfinite(e.y) ? e.y : 0.0;
        double spriteX = ex - p.x;
        double spriteY = ey - p.y;
        double transformX = invDet * (p.dirY * spriteX - p.dirX * spriteY);
        double transformY = invDet * (-p.planeY * spriteX + p.planeX * spriteY);
        if (transformY <= 0.3) continue;
        if (!std::isfinite(transformX) || !std::isfinite(transformY)) continue;
        int spriteScreenX = (int)((m_size.x / 2.0) * (1 + transformX / transformY));
        double heightScale = m_size.y / transformY;
        int spriteHeight = abs((int)heightScale);
        spriteHeight = std::min(spriteHeight, (int)m_size.y * 3);
        int spriteWidth = spriteHeight;
        int horizon = (int)(m_size.y * 0.5 + p.pitch);
        double enemyFootH = (double)getMapHeight(core, (int)ex, (int)ey, map_rows);
        int footPx = (int)(horizon - (enemyFootH - p.eyeHeight) * heightScale);
        int drawStartY = footPx - spriteHeight;
        int drawStartX = spriteScreenX - spriteWidth / 2;
        int drawEndX   = spriteScreenX + spriteWidth / 2;
        int idx = -1;
        if (templates) {
            int max_templates = enemyTextures.size();
            for (int k = 0; k < max_templates && templates[k] != nullptr; k++) {
                if (templates[k] == e.templateData) { idx = k; break; }
            }
        }
        if (idx < 0 || idx >= (int)enemyTextures.size()) continue;
        sfTexture* tex = enemyTextures[idx];
        if (!tex) continue;
        Enemy *tpl = e.templateData;
        std::vector<int> sprite_x = tpl->getSpriteX();
        std::vector<int> sprite_y = tpl->getSpriteY();
        std::vector<std::pair<int,int>> shoot_anim  = tpl->getShootAnim();
        std::vector<std::pair<int,int>> walk_anim   = tpl->getWalkAnim();
        std::vector<std::pair<int,int>> reload_anim = tpl->getReloadAnim();

        int gridX = 0;
        int gridY = 0;
        int default_frame_size = 64;
        if (e.state == 1) {
            float progress = (tpl->getReshootTime() > 0.0f) ? (e.animTimer / tpl->getReshootTime()) : 0.0f;
            progress = std::max(0.0f, progress);
            if (shoot_anim.empty()) {
                gridX = std::max(0, std::min(3, (int)(progress * 4)));
                gridY = 0;
            } else {
                int total_frames = shoot_anim.size();
                int current_frame = std::max(0, std::min(total_frames - 1, (int)(progress * total_frames)));
                gridX = shoot_anim[current_frame].first;
                gridY = shoot_anim[current_frame].second;
            }
        } else if (e.state == 3) {
            float progress = (tpl->getReloadTime() > 0.0f) ? (e.animTimer / tpl->getReloadTime()) : 0.0f;
            progress = std::max(0.0f, progress);
            if (reload_anim.empty()) {
                gridX = std::max(0, std::min(3, (int)(progress * 4)));
                gridY = 0;
            } else {
                int total_frames = reload_anim.size();
                int current_frame = std::max(0, std::min(total_frames - 1, (int)(progress * total_frames)));
                gridX = reload_anim[current_frame].first;
                gridY = reload_anim[current_frame].second;
            }
        } else if (e.state == 2) {
            if (walk_anim.empty()) {
                gridX = std::abs((int)(e.animTimer * 6.0f)) % 4;
                gridY = 0;
            } else {
                int total_frames = walk_anim.size();
                int current_frame = std::abs((int)(e.animTimer * 6.0f)) % total_frames;
                gridX = walk_anim[current_frame].first;
                gridY = walk_anim[current_frame].second;
            }
        } else {
            gridX = 0;
            gridY = 0;
        }
        gridX = std::max(0, gridX);
        gridY = std::max(0, gridY);

        int pixel_X_start = 0;
        int pixel_X_end   = 0;
        int pixel_Y_start = 0;
        int pixel_Y_end   = 0;

        if (!sprite_x.empty() && gridX < (int)sprite_x.size()) {
            pixel_X_start = sprite_x[gridX];
            pixel_X_end = (gridX + 1 < (int)sprite_x.size())
                ? sprite_x[gridX + 1]
                : pixel_X_start + default_frame_size;
        } else {
            pixel_X_start = gridX * default_frame_size;
            pixel_X_end   = pixel_X_start + default_frame_size;
        }

        if (!sprite_y.empty() && gridY < (int)sprite_y.size()) {
            pixel_Y_start = sprite_y[gridY];
            pixel_Y_end = (gridY + 1 < (int)sprite_y.size())
                ? sprite_y[gridY + 1]
                : pixel_Y_start + default_frame_size;
        } else {
            pixel_Y_start = gridY * default_frame_size;
            pixel_Y_end   = pixel_Y_start + default_frame_size;
        }

        int frameW = pixel_X_end - pixel_X_start;
        int frameH = pixel_Y_end - pixel_Y_start;
        if (frameW <= 0) frameW = default_frame_size;
        if (frameH <= 0) frameH = default_frame_size;

        for (int stripe = std::max(0, drawStartX); stripe < std::min((int)m_size.x, drawEndX); stripe++) {
            if (transformY >= m_depthBuffer[stripe])
                continue;
            int localX = (int)(((stripe - drawStartX) * frameW) / (float)spriteWidth);
            localX = std::max(0, std::min(frameW - 1, localX));
            int texX = pixel_X_start + localX;

            sfIntRect rect = { texX, pixel_Y_start, 1, frameH };
            sfSprite_setTexture(m_enemySprite, tex, sfFalse);
            sfSprite_setTextureRect(m_enemySprite, rect);
            sfSprite_setPosition(m_enemySprite, (sfVector2f){ (float)stripe, (float)std::max(0, drawStartY) });
            float scaleY = spriteHeight / (float)frameH;
            sfSprite_setScale(m_enemySprite, (sfVector2f){ 1.0f, scaleY });
            sfRenderWindow_drawSprite(window, m_enemySprite, nullptr);
        }
    }
}
