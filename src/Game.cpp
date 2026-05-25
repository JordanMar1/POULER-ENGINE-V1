#include "Game.hpp"
#include <math.h>
#include <cstring>
#include <algorithm>
#include <iostream>
#include "Renderer.hpp"

static bool findTeleport(Core *core, int map_rows, int srcX, int srcY, double &dstX, double &dstY)
{
    char **map = core->getMaps()->getMapArray();
    char val = map[srcY][srcX];

    for (int y = 0; y < map_rows; y++) {
        if (!map[y]) continue;
        for (int x = 0; map[y][x]; x++) {
            if (x == srcX && y == srcY) continue;
            if (map[y][x] == val) {
                dstX = x + 0.5;
                dstY = y + 0.5;
                return true;
            }
        }
    }
    return false;
}

int Game::getH(Core *core, int mx, int my, int map_rows)
{
    char **map = core->getMaps()->getMapArray();
    int **heatmap = core->getMaps()->getHeatmap();

    if (my < 0 || my >= map_rows || mx < 0 || !map[my] || (size_t)mx >= strlen(map[my]))
        return 0;
    if (map[my][mx] == 'W') return 99;
    int h = heatmap[my][mx];
    return (h < 0) ? 0 : h;
}

void Game::RenderWeapon(Core *core, sfRenderWindow *window, const std::vector<Weapons *> &weapons, const std::vector<sfTexture *> &weapon_textures, sfSprite *weapon_sprite, int current_weapon_idx, int weapon_state, float weapon_timer)
{
    if (weapons.empty() || current_weapon_idx >= (int)weapon_textures.size() || !weapon_textures[current_weapon_idx])
        return;
    Weapons *w = weapons[current_weapon_idx];
    sfTexture *w_tex = weapon_textures[current_weapon_idx];
    sfSprite_setTexture(weapon_sprite, w_tex, sfTrue);
    int gridX = 0;
    int gridY = 0;
    if (weapon_state == 1 && w->ammo > 0) {
        if (w->shoot_anim.empty()) {
            if (core->getWindow()->isDebug())
                std::cout << "[DEBUG ANIM] Attention: shoot_anim est VIDE pour cette arme !\n";
            float progress = (w->shoot_time > 0.0f) ? (weapon_timer / w->shoot_time) : 0.0f;
            gridX = (int)(progress * 4); 
            if (gridX > 3) gridX = 3;
            gridY = 0;
        } else {
            int total_frames = w->shoot_anim.size();
            float progress = (w->shoot_time > 0.0f) ? (weapon_timer / w->shoot_time) : 0.0f;
            int current_frame = (int)(progress * total_frames);
            if (current_frame >= total_frames) current_frame = total_frames - 1;
            
            gridX = w->shoot_anim[current_frame].first;
            gridY = w->shoot_anim[current_frame].second;
        }
    }
    else if (weapon_state == 2 && w->mag > 0) {
        if (w->reload_anim.empty()) {
            if (core->getWindow()->isDebug())
                std::cout << "[DEBUG ANIM] Attention: reload_anim est VIDE pour cette arme !\n";
            
            float progress = (w->reload_time > 0.0f) ? (weapon_timer / w->reload_time) : 0.0f;
            gridX = (int)(progress * 4);
            if (gridX > 3) gridX = 3;
            gridY = 0;
        } else {
            int total_frames = w->reload_anim.size();
            float progress = (w->reload_time > 0.0f) ? (weapon_timer / w->reload_time) : 0.0f;
            int current_frame = (int)(progress * total_frames);
            if (current_frame >= total_frames) current_frame = total_frames - 1;
            
            gridX = w->reload_anim[current_frame].first;
            gridY = w->reload_anim[current_frame].second;
        }
    }
    else {
        gridX = 0;
        gridY = 0;
    }
    int pixel_X_start = 0;
    int pixel_X_end   = 0;
    int pixel_Y_start = 0;
    int pixel_Y_end   = 0;
    int default_frame_size = 64;
    if (!w->sprite_x.empty() && gridX < (int)w->sprite_x.size()) {
        pixel_X_start = w->sprite_x[gridX];
        if (gridX + 1 < (int)w->sprite_x.size()) {
            pixel_X_end = w->sprite_x[gridX + 1];
        } else {
            pixel_X_end = pixel_X_start + default_frame_size;
        }
    } else {
        pixel_X_start = gridX * default_frame_size;
        pixel_X_end   = pixel_X_start + default_frame_size;
    }
    if (!w->sprite_y.empty() && gridY < (int)w->sprite_y.size()) {
        pixel_Y_start = w->sprite_y[gridY];
        if (gridY + 1 < (int)w->sprite_y.size()) {
            pixel_Y_end = w->sprite_y[gridY + 1];
        } else {
            pixel_Y_end = pixel_Y_start + default_frame_size;
        }
    } else {
        pixel_Y_start = gridY * default_frame_size;
        pixel_Y_end   = pixel_Y_start + default_frame_size;
    }
    sfIntRect rect;
    rect.left   = pixel_X_start;
    rect.top    = pixel_Y_start;
    rect.width  = pixel_X_end - pixel_X_start;
    rect.height = pixel_Y_end - pixel_Y_start;
    sfSprite_setTextureRect(weapon_sprite, rect);
    sfVector2f scale = {w->scale_x, w->scale_y};
    sfSprite_setScale(weapon_sprite, scale);
    sfSprite_setPosition(weapon_sprite, (sfVector2f){w->x, w->y});
    sfRenderWindow_drawSprite(window, weapon_sprite, nullptr);
}

void Game::UpdateWeaponTimer(std::vector<Weapons *> &weapons, int current_weapon_idx, int &weapon_state, float &weapon_timer, float dt)
{
    if (weapons.empty()) return;
    Weapons *w = weapons[current_weapon_idx];
    if (weapon_state == 1) {
        weapon_timer += dt;
        if (weapon_timer >= w->shoot_time) {
            weapon_state = 0;
            weapon_timer = 0.0f;
        }
    } else if (weapon_state == 2) {
        weapon_timer += dt;
        if (weapon_timer >= w->reload_time) {
            w->ammo = w->max_ammo;
            weapon_state = 0;
            weapon_timer = 0.0f;
        }
    }
}

void Game::ManageMouse(sfRenderWindow *window, Player &p, Settings *settings)
{
    if (!sfRenderWindow_hasFocus(window))
        return;
    sfVector2u winSize = sfRenderWindow_getSize(window);
    sfVector2i center = {(int)(winSize.x / 2), (int)(winSize.y / 2)};
    sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);
    int deltaX = mousePos.x - center.x;
    int deltaY = mousePos.y - center.y;
    if (deltaX != 0) {
        float sensitivity = settings->horizontal_sensitivity;
        double angle = deltaX * sensitivity;
        double oldDirX = p.dirX;
        p.dirX = p.dirX * cos(angle) - p.dirY * sin(angle);
        p.dirY = oldDirX * sin(angle) + p.dirY * cos(angle);
        double oldPlaneX = p.planeX;
        p.planeX = p.planeX * cos(angle) - p.planeY * sin(angle);
        p.planeY = oldPlaneX * sin(angle) + p.planeY * cos(angle);
    }
    if (deltaY != 0) {
        float vertSensitivity = settings->vertical_sensitivity;
        p.pitch -= deltaY * vertSensitivity;
        int pitchLimit = (int)(winSize.y / 2);
        if (p.pitch > pitchLimit)  p.pitch = pitchLimit;
        if (p.pitch < -pitchLimit) p.pitch = -pitchLimit;
    }
    if (deltaX != 0 || deltaY != 0) {
        sfMouse_setPositionRenderWindow(center, window);
    }
}


void Game::HandleInputs(Core *core, Player &p, float dt, int map_rows, std::vector<Weapons *> &weapons, int current_weapon_idx, int &weapon_state, float &weapon_timer)
{
    float mv = 4.0f * dt;
    auto tryMove = [&](double nx, double ny) {
        int nh = getH(core, (int)nx, (int)ny, map_rows);
        int ch = getH(core, (int)p.x, (int)p.y, map_rows);
        if (nh != 99 && nh - ch <= 1) {
            p.x = nx;
            p.y = ny;
        }
    };
    float leanTarget = 0.0f;
    if (sfKeyboard_isKeyPressed(core->getSettings()->binds.leanLeft))  leanTarget = -1.0f;
    if (sfKeyboard_isKeyPressed(core->getSettings()->binds.leanRight)) leanTarget =  1.0f;
    p.lean += (leanTarget - p.lean) * 10.0f * dt;
    if (sfKeyboard_isKeyPressed(core->getSettings()->binds.moveForward)) tryMove(p.x + p.dirX * mv, p.y + p.dirY * mv);
    if (sfKeyboard_isKeyPressed(core->getSettings()->binds.moveBack))    tryMove(p.x - p.dirX * mv, p.y - p.dirY * mv);
    if (sfKeyboard_isKeyPressed(core->getSettings()->binds.moveLeft))
        tryMove(p.x + p.dirY * mv, p.y - p.dirX * mv);
    if (sfKeyboard_isKeyPressed(core->getSettings()->binds.moveRight))
        tryMove(p.x - p.dirY * mv, p.y + p.dirX * mv);
    double mapH = (double)getH(core, (int)p.x, (int)p.y, map_rows);
    if (mapH >= 99) mapH = p.height;
    double targetH = mapH + (p.crouching ? -0.25 : 0.0);
    double diff = targetH - p.height;
    if (diff < 0.0) p.height = targetH;
    else p.height += diff * 14.0 * dt;
    p.eyeHeight = p.height + 0.5;
    if (_teleportCooldown > 0.0f)
        _teleportCooldown -= dt;
    char **map = core->getMaps()->getMapArray();
    char tile = map[(int)p.y][(int)p.x];
    if (tile >= '1' && tile <= '9' && _teleportCooldown <= 0.0f) {
        double dstX, dstY;
        if (findTeleport(core, map_rows, (int)p.x, (int)p.y, dstX, dstY)) {
            p.x = dstX;
            p.y = dstY;
            int newH = core->getMaps()->getHeatmap()[(int)p.y][(int)p.x];
            p.height = (double)std::max(0, newH);
            p.eyeHeight = p.height + 0.5;
            _teleportCooldown = 1.5f;
        }
    }

    if (weapon_state == 0 && !weapons.empty() && sfKeyboard_isKeyPressed((core->getSettings()->binds.reload))) {
        if (core->getWindow()->isDebug())
            std::cout << "reloading..." << std::endl;
        Weapons *w = weapons[current_weapon_idx];
        if (w->mag > 0) {
            weapon_state = 2;
            weapon_timer = 0.0f;
            w->mag -= 1;
            w->ammo = w->max_ammo;
            sfSound_play(w->reload_sound);
        }
    }
    if (weapon_state == 0 && !weapons.empty()) {
        if (sfMouse_isButtonPressed((core->getSettings()->binds.shoot))) {
            if (core->getWindow()->isDebug())
                std::cout << "shooting..." << std::endl;
            Weapons *w = weapons[current_weapon_idx];
            if (w->ammo > 0) {
                w->ammo--;
                weapon_state = 1;
                weapon_timer = 0.0f;
                sfSound_play(w->shoot_sound);
            }
        }
    }
}

void Game::RenderScene(Core *core, sfUint8 *pixels, Player &p, int map_rows, int w, int h)
{
    double leanSideX = -p.dirY * p.lean * 0.3;
    double leanSideY = p.dirX * p.lean * 0.3;
    double eyeX = p.x + leanSideX;
    double eyeY = p.y + leanSideY;
    double horizonF = h * 0.5 + p.pitch;
    int horizon = (int)horizonF;

    for (int i = 0; i < w * h * 4; i += 4) {
        int pixelY = (i / 4) / w;
        pixels[i] = pixels[i+1] = pixels[i+2] = (pixelY < horizon) ? 30 : 50;
        pixels[i+3] = 255;
    }
    for (int x = 0; x < w; x++) {
        double cameraX = 2.0 * x / (double)w - 1.0;
        double rayX = p.dirX + p.planeX * cameraX;
        double rayY = p.dirY + p.planeY * cameraX;
        int mX = (int)eyeX, mY = (int)eyeY;
        double dX = fabs(1.0 / rayX);
        double dY = fabs(1.0 / rayY);
        int stX = (rayX < 0) ? -1 : 1;
        int stY = (rayY < 0) ? -1 : 1;
        double sX = (rayX < 0) ? (eyeX - mX) * dX : (mX + 1.0 - eyeX) * dX;
        double sY = (rayY < 0) ? (eyeY - mY) * dY : (mY + 1.0 - eyeY) * dY;
        int screen_floor_limit = h;
        int lastH = getH(core, mX, mY, map_rows);
        int steps = 0;
        while (steps++ < 64) {
            int side = (sX < sY) ? 0 : 1;
            if (side == 0) { sX += dX; mX += stX; }
            else           { sY += dY; mY += stY; }
            double dist = (side == 0) ? (sX - dX) : (sY - dY);
            if (dist < 0.01) dist = 0.01;
            int curH = getH(core, mX, mY, map_rows);
            double lineH = (double)h / dist;
            int floor_px = (int)(horizonF - (lastH - p.eyeHeight) * lineH);
            if (floor_px < screen_floor_limit) {
                float dShade = std::min(1.0f, 14.0f / (float)(dist + 1.0f));
                sfUint8 shade = (sfUint8)std::max(0, (int)((38 + lastH * 9) * dShade));
                for (int y = std::max(0, floor_px); y < screen_floor_limit; y++) {
                    int idx = (y * w + x) * 4;
                    pixels[idx] = pixels[idx+1] = pixels[idx+2] = shade;
                }
                screen_floor_limit = std::max(0, floor_px);
            }
            if (curH != lastH) {
                double wallTopH = (curH == 99) ? (double)lastH + 3.0 : (double)std::max(curH, lastH);
                double wallBotH = (double)std::min(curH, lastH);
                int pxTop = (int)(horizonF - (wallTopH - p.eyeHeight) * lineH);
                int pxBot = (int)(horizonF - (wallBotH - p.eyeHeight) * lineH);
                int drawTop = std::max(pxTop, 0);
                int drawBot = std::min(pxBot, screen_floor_limit);
                if (drawTop < drawBot) {
                    float dShade = std::min(1.0f, 12.0f / (float)(dist + 1.0f));
                    for (int y = drawTop; y < drawBot; y++) {
                        float fGrad = (pxBot > pxTop) ? (float)(y - pxTop) / (float)(pxBot - pxTop) : 0.0f;
                        int val = (curH == 99) ? (145 - fGrad * 65) : (135 - fGrad * 55);
                        val = (int)(std::max(0, std::min(255, val)) * dShade);
                        if (side == 1) val = val * 7 / 10;
                        int idx = (y * w + x) * 4;
                        pixels[idx] = pixels[idx+1] = pixels[idx+2] = (sfUint8)val;
                    }
                    if (curH == 99)
                        break;
                }
            }
            lastH = curH;
        }
    }
    if (fabs(p.lean) > 0.001f) {
        float angle = p.lean * -0.08f;
        float cosA = cos(angle);
        float sinA = sin(angle);
        float cx = w * 0.5f;
        float cy = h * 0.5f;
        float maxZoom = 1.125f / cos(fabs(angle));
        float zoom = 1.0f + (maxZoom - 1.0f) * fabs(p.lean);

        std::vector<sfUint8> tmp(w * h * 4);
        memcpy(tmp.data(), pixels, w * h * 4);
        memset(pixels, 0, w * h * 4);
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                float dx = (x - cx) / zoom;
                float dy = (y - cy) / zoom;
                int srcX = (int)(cosA * dx + sinA * dy + cx);
                int srcY = (int)(-sinA * dx + cosA * dy + cy);
                if (srcX < 0 || srcX >= w || srcY < 0 || srcY >= h) continue;
                int dst = (y * w + x) * 4;
                int src = (srcY * w + srcX) * 4;
                pixels[dst]   = tmp[src];
                pixels[dst+1] = tmp[src+1];
                pixels[dst+2] = tmp[src+2];
                pixels[dst+3] = tmp[src+3];
            }
        }
    }
}

int Game::Play(Core *core, Maps *map)
{
    char **mapArray = map->getMapArray();
    sfMusic_setVolume(map->getMusic(), core->getSettings()->musicVolume);
    sfMusic_setLoop(map->getMusic(), true);
    sfMusic_play(map->getMusic());
    int map_rows = 0;
    while (mapArray[map_rows]) map_rows++;
    sfRenderWindow *window = core->getWindow()->getWindow();

    Player p = {2.5, 2.5, 1.0, 0.0, 0.0, 0.66, 0.0, 0.0, 0.5, false, 0.0f, 0.0f};
    for (int i = 0; i < map_rows; i++) {
        for (int j = 0; mapArray[i][j]; j++) {
            if (mapArray[i][j] == 'S') {
                p.x = j + 0.5; p.y = i + 0.5;
                p.height = (double)std::max(0, map->getHeatmap()[i][j]);
                p.eyeHeight = p.height + 0.5;
            }
        }
    }
    sfVector2u currentSize = sfRenderWindow_getSize(window);
    Renderer renderer(currentSize);
    sfClock *clock = sfClock_create();
    std::vector<Weapons *> weapons = core->getWeapons();
    setWeaponsVolume(weapons, core->getSettings()->soundVolume);
    std::vector<sfTexture *> weapon_textures;
    sfSprite *weapon_sprite = sfSprite_create();
    int current_weapon_idx = 0;
    int weapon_state = 0;
    float weapon_timer = 0.0f;
    for (auto *w : weapons) {
        sfTexture *t = sfTexture_createFromFile(w->sprite_path.c_str(), nullptr);
        weapon_textures.push_back(t);
    }
    sfRenderWindow_setMouseCursorVisible(window, sfFalse);
    sfRenderWindow_setFramerateLimit(window, core->getSettings()->fps);
    while (sfRenderWindow_isOpen(window)) {
        float dt = sfTime_asSeconds(sfClock_restart(clock));
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
            if (event.type == sfEvtResized) {
                currentSize.x = event.size.width;
                currentSize.y = event.size.height;
                renderer.resize(currentSize);
                sfFloatRect visibleArea = {0.f, 0.f, (float)currentSize.x, (float)currentSize.y};
                sfView* view = sfView_createFromRect(visibleArea);
                sfRenderWindow_setView(window, view);
                sfView_destroy(view);
            }
            if (event.type == sfEvtKeyPressed && event.key.code == sfKeyEscape) {
                for (auto *t : weapon_textures) if (t) sfTexture_destroy(t);
                sfSprite_destroy(weapon_sprite);
                for (auto *w: weapons) {
                    if (w) {
                        w->ammo = w->max_ammo;
                        w->mag = w->max_mag;
                    }
                }
                sfMusic_stop(core->getMaps()->getMusic());
                sfRenderWindow_setMouseCursorVisible(window, sfTrue);
                sfClock_destroy(clock);
                return core->menu_return();
            }
            if (event.type == sfEvtKeyPressed && event.key.code == core->getSettings()->binds.crouch)
                p.crouching = !p.crouching;   
            if (event.type == sfEvtKeyPressed && event.key.code >= sfKeyNum1 && event.key.code <= sfKeyNum9) {
                int selected = event.key.code - sfKeyNum1;
                if (selected < (int)weapons.size() && weapon_state == 0) {
                    current_weapon_idx = selected;
                }
            }
        }
        ManageMouse(window, p, core->getSettings());
        HandleInputs(core, p, dt, map_rows, weapons, current_weapon_idx, weapon_state, weapon_timer);
        renderer.initFrameRender(core, p, map_rows);
        sfRenderWindow_clear(window, sfBlack);
        renderer.drawScene(window, p.lean);
        RenderWeapon(core, window, weapons, weapon_textures, weapon_sprite, current_weapon_idx, weapon_state, weapon_timer);
        UpdateWeaponTimer(weapons, current_weapon_idx, weapon_state, weapon_timer, dt);
        sfRenderWindow_display(window);
    }
    for (auto *t : weapon_textures) {
        if (t) sfTexture_destroy(t);
    }
    sfSprite_destroy(weapon_sprite);
    sfMusic_stop(core->getMaps()->getMusic());
    sfClock_destroy(clock);
    return 0;
}