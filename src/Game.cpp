#include "Game.hpp"
#include <math.h>
#include <cstring>
#include <algorithm>
#include <iostream>
#include "Renderer.hpp"
#include "InterfaceItems.hpp"

static bool findTeleport(Core *core, int map_rows, int srcX, int srcY, double &dstX, double &dstY){
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

int Game::getH(Core *core, int mx, int my, int map_rows){
    char **map = core->getMaps()->getMapArray();
    int **heatmap = core->getMaps()->getHeatmap();
    if (my < 0 || my >= map_rows || mx < 0 || !map[my] || (size_t)mx >= strlen(map[my]))
        return 0;
    if (map[my][mx] == 'W') return 99;
    int h = heatmap[my][mx];
    return (h < 0) ? 0 : h;
}

void Game::RenderWeapon(sfRenderWindow *window, const std::vector<Weapons *> &weapons, const std::vector<sfTexture *> &weapon_textures, sfSprite *weapon_sprite, int current_weapon_idx, int weapon_state, float weapon_timer){
    if (weapons.empty() || current_weapon_idx >= (int)weapon_textures.size() || !weapon_textures[current_weapon_idx])
        return;
    Weapons *w = weapons[current_weapon_idx];
    sfTexture *w_tex = weapon_textures[current_weapon_idx];
    sfSprite_setTexture(weapon_sprite, w_tex, sfTrue);
    int gridX = 0;
    int gridY = 0;
    if (weapon_state == 1 && w->ammo > 0) {
        if (w->shoot_anim.empty()) {
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
    } else if (weapon_state == 2 && w->mag > 0) {
        if (w->reload_anim.empty()) {
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
    } else if (weapon_state == 3) {
        if (!w->check_anim.empty()) {
            int total_frames = w->check_anim.size();
            float progress = (w->check_time > 0.0f) ? (weapon_timer / w->check_time) : 0.0f;
            int current_frame = (int)(progress * total_frames);
            if (current_frame >= total_frames) current_frame = total_frames - 1;
            gridX = w->check_anim[current_frame].first;
            gridY = w->check_anim[current_frame].second;
        }
    } else {
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
        if (gridX + 1 < (int)w->sprite_x.size())
            pixel_X_end = w->sprite_x[gridX + 1];
        else
            pixel_X_end = pixel_X_start + default_frame_size;
    } else {
        pixel_X_start = gridX * default_frame_size;
        pixel_X_end   = pixel_X_start + default_frame_size;
    }
    if (!w->sprite_y.empty() && gridY < (int)w->sprite_y.size()) {
        pixel_Y_start = w->sprite_y[gridY];
        if (gridY + 1 < (int)w->sprite_y.size())
            pixel_Y_end = w->sprite_y[gridY + 1];
        else
            pixel_Y_end = pixel_Y_start + default_frame_size;
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

void Game::RenderHead(Core *core, sfRenderWindow *window, Player &p) {
    Head *head = core->getHead();
    
    if (!head || !head->texture || !head->sprite)
        return;
    HeadThreshold *t = head->get_threshold(p.hp);
    if (!t)
        return;
    sfIntRect rect;
    rect.left   = t->x1;
    rect.top    = t->y1;
    rect.width  = t->x2 - t->x1;
    rect.height = t->y2 - t->y1;
    sfSprite_setTexture(head->sprite, head->texture, sfTrue);
    sfSprite_setTextureRect(head->sprite, rect);
    sfSprite_setOrigin(head->sprite, (sfVector2f){rect.width / 2.0f, rect.height / 2.0f});
    sfSprite_setScale(head->sprite, (sfVector2f){head->scale, head->scale});
    sfSprite_setPosition(head->sprite, (sfVector2f){(float)head->x, (float)head->y});
    sfRenderWindow_drawSprite(window, head->sprite, nullptr);
}

void Game::UpdateWeaponTimer(std::vector<Weapons *> &weapons, int current_weapon_idx, int &weapon_state, float &weapon_timer, float dt){
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
    } else if (weapon_state == 3) {
        weapon_timer += dt;
        if (weapon_timer >= w->check_time) {
            weapon_state = 0;
            weapon_timer = 0.0f;
        }
    }
}

void Game::ManageMouse(sfRenderWindow *window, Player &p, Settings *settings){
    if (!sfRenderWindow_hasFocus(window))
        return;
    sfVector2u winSize = sfRenderWindow_getSize(window);
    sfVector2i center = {(int)(winSize.x / 2), (int)(winSize.y / 2)};
    sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);
    int deltaX = mousePos.x - center.x;
    int deltaY = mousePos.y - center.y;
    if (deltaX != 0) {
        float sensitivity = settings->getEffectiveHSens();
        double angle = deltaX * sensitivity;
        double oldDirX = p.dirX;
        p.dirX = p.dirX * cos(angle) - p.dirY * sin(angle);
        p.dirY = oldDirX * sin(angle) + p.dirY * cos(angle);
        double oldPlaneX = p.planeX;
        p.planeX = p.planeX * cos(angle) - p.planeY * sin(angle);
        p.planeY = oldPlaneX * sin(angle) + p.planeY * cos(angle);
    }
    if (deltaY != 0) {
        float vertSensitivity = settings->getEffectiveVSens();
        p.pitch -= deltaY * vertSensitivity;
        int pitchLimit = (int)(winSize.y / 2);
        if (p.pitch > pitchLimit)  p.pitch = pitchLimit;
        if (p.pitch < -pitchLimit) p.pitch = -pitchLimit;
    }
    if (deltaX != 0 || deltaY != 0) {
        sfMouse_setPositionRenderWindow(center, window);
    }
}

void Game::HandleInputs(Core *core, Player &p, float dt, int map_rows, std::vector<Weapons *> &weapons, int current_weapon_idx, int &weapon_state, float &weapon_timer){
    auto tryMove = [&](double nx, double ny) {
        int nh = getH(core, (int)nx, (int)ny, map_rows);
        int ch = getH(core, (int)p.x, (int)p.y, map_rows);
        if (nh == 99) return;
        if (nh - ch > 1) return;
        p.x = nx;
        p.y = ny;
        if (nh > ch) {
            p.height = (double)nh + (p.crouching ? p.crouchHeight : p.defaultHeight);
        }
    };
    bool isMoving = sfKeyboard_isKeyPressed(core->getSettings()->binds.moveForward) ||
                    sfKeyboard_isKeyPressed(core->getSettings()->binds.moveBack) ||
                    sfKeyboard_isKeyPressed(core->getSettings()->binds.moveLeft) ||
                    sfKeyboard_isKeyPressed(core->getSettings()->binds.moveRight);
    float leanTarget = 0.0f;
    float mv = p.speed * dt;
    bool isTryingToSprint = sfKeyboard_isKeyPressed(core->getSettings()->binds.sprint) && isMoving;
    if (isTryingToSprint && p.stamina > 0.0f && !p.staminaCooldown) {
        mv = p.sprint_speed * dt;
        p.stamina -= p.staminaConsumption * dt;
        if (p.stamina <= 0.0f) {
            p.stamina = 0.0f;
            p.staminaCooldown = true;
        }
    } else {
        p.stamina += p.staminaRecovery * dt;
        if (p.stamina > p.maxStamina)
            p.stamina = p.maxStamina;
        if (p.stamina >= p.sprintCooldown)
            p.staminaCooldown = false;
    }
    if (sfKeyboard_isKeyPressed(core->getSettings()->binds.leanLeft))  leanTarget = -1.0f;
    if (sfKeyboard_isKeyPressed(core->getSettings()->binds.leanRight)) leanTarget =  1.0f;
    p.lean += (leanTarget - p.lean) * 10.0f * dt;
    if (sfKeyboard_isKeyPressed(core->getSettings()->binds.moveForward)) tryMove(p.x + p.dirX * mv, p.y + p.dirY * mv);
    if (sfKeyboard_isKeyPressed(core->getSettings()->binds.moveBack))    tryMove(p.x - p.dirX * mv, p.y - p.dirY * mv);
    if (sfKeyboard_isKeyPressed(core->getSettings()->binds.moveLeft))        tryMove(p.x + p.dirY * mv, p.y - p.dirX * mv);
    if (sfKeyboard_isKeyPressed(core->getSettings()->binds.moveRight))        tryMove(p.x - p.dirY * mv, p.y + p.dirX * mv);
    double mapH = (double)getH(core, (int)p.x, (int)p.y, map_rows);
    if (mapH >= 99) mapH = p.height;
    double targetH = mapH + (p.crouching ? p.crouchHeight : p.defaultHeight);
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

int Game::Play(Core *core, Maps *map){
    char **mapArray = map->getMapArray();
    sfMusic_setVolume(map->getMusic(), core->getSettings()->musicVolume);
    sfMusic_setLoop(map->getMusic(), true);
    sfMusic_play(map->getMusic());
    int map_rows = 0;
    while (mapArray[map_rows]) map_rows++;
    sfRenderWindow *window = core->getWindow()->getWindow();    
    Player &p = *core->getPlayer();
    bool spawn_found = false;
    for (int i = 0; i < map_rows; i++) {
        for (int j = 0; mapArray[i][j]; j++) {
            if (mapArray[i][j] == 'S') {
                p.x = j + 0.5; p.y = i + 0.5;
                p.height = (double)std::max(0, map->getHeatmap()[i][j]);
                p.eyeHeight = p.height + 0.5;
                spawn_found = true;
            }
        }
    }
    if (!spawn_found) {
        std::cerr << "[ERROR] Aucun point de spawn 'S' trouvé dans la map !" << std::endl;
        p.x = 1.5; p.y = 1.5; 
    }
    sfVector2u currentSize = sfRenderWindow_getSize(window);
    Renderer renderer(currentSize);
    sfClock *clock = sfClock_create();
    std::vector<Weapons *> weapons = core->getWeapons();
    setWeaponsVolume(weapons, core->getSettings()->soundVolume);
    InterfaceItems ui(nullptr);
    ui.loadItems(core->interfaceItems);
    std::vector<sfTexture *> weapon_textures;
    sfSprite *weapon_sprite = sfSprite_create();
    int current_weapon_idx = 0;
    int weapon_state = 0;
    float weapon_timer = 0.0f;
    for (auto *w : weapons) {
        sfTexture *t = sfTexture_createFromFile(w->sprite_path.c_str(), nullptr);
        weapon_textures.push_back(t);
    }
    sfTexture *head_texture = nullptr;
    sfSprite  *head_sprite  = sfSprite_create();
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
            if (event.type == sfEvtKeyPressed && event.key.code == core->getSettings()->binds.checkAmmo) {
                if (weapon_state == 0 && !weapons.empty()) {
                    weapon_state = 3;
                    weapon_timer = 0.0f;
                    ui.notifyCheckPressed(weapons[current_weapon_idx]->check_time);
                }
            }
            if (event.type == sfEvtKeyPressed && event.key.code == sfKeyEscape) {
                for (auto *t : weapon_textures) if (t) sfTexture_destroy(t);
                sfSprite_destroy(weapon_sprite);
                if (head_texture) sfTexture_destroy(head_texture);
                sfSprite_destroy(head_sprite);
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
        RenderWeapon(window, weapons, weapon_textures, weapon_sprite, current_weapon_idx, weapon_state, weapon_timer);
        RenderHead(core, window, p);
        ui.render(window, core->getPlayer(), weapons, current_weapon_idx, dt);
        UpdateWeaponTimer(weapons, current_weapon_idx, weapon_state, weapon_timer, dt);
        sfRenderWindow_display(window);
    }
    for (auto *t : weapon_textures) {
        if (t) sfTexture_destroy(t);
    }
    sfSprite_destroy(weapon_sprite);
    if (head_texture) sfTexture_destroy(head_texture);
    sfSprite_destroy(head_sprite);
    sfMusic_stop(core->getMaps()->getMusic());
    sfClock_destroy(clock);
    return 0;
}