/*
** EPITECH PROJECT, 2026
** POULER-ENGINE-V1.1
** File description:
** InterfaceItems
*/

#include "InterfaceItems.hpp"
#include "Player.hpp"
#include "Weapons.hpp"
#include <cstring>
#include <cmath>
#include <iostream>

static constexpr const char *FALLBACK_FONT_PATH = "assets/fonts/menu.ttf";

static constexpr float BEAT_PERIOD   = 0.85f;
static constexpr float BEAT_PULSE1   = 0.12f;
static constexpr float BEAT_PULSE2   = 0.28f;
static constexpr float BEAT_MAX_SCALE = 1.18f;

InterfaceItems::InterfaceItems(sfFont *fallbackFont)
    : _fallbackFont(fallbackFont)
{}

InterfaceItems::~InterfaceItems()
{
    for (auto *s : _items) {
        if (s->font_owned && s->font)
            sfFont_destroy(s->font);
        if (s->texture) sfTexture_destroy(s->texture);
        if (s->sprite)  sfSprite_destroy(s->sprite);
        delete s;
    }
}

sfFont *InterfaceItems::_resolveFont(const std::string &path)
{
    if (!path.empty()) {
        sfFont *f = sfFont_createFromFile(path.c_str());
        if (f) return f;
        std::cerr << "[UI] Font not found: " << path
                  << " – falling back to " << FALLBACK_FONT_PATH << "\n";
    }
    sfFont *f = sfFont_createFromFile(FALLBACK_FONT_PATH);
    if (f) return f;
    return nullptr;
}

void InterfaceItems::loadItems(const std::vector<InterfaceItemDef> &defs)
{
    for (auto *s : _items) {
        if (s->font_owned && s->font) sfFont_destroy(s->font);
        if (s->texture) sfTexture_destroy(s->texture);
        if (s->sprite)  sfSprite_destroy(s->sprite);
        delete s;
    }
    _items.clear();
    for (const auto &def : defs) {
        auto *s = new InterfaceItemState();
        s->def = def;
        if (!def.font_path.empty()) {
            sfFont *f = sfFont_createFromFile(def.font_path.c_str());
            if (f) {
                s->font = f;
                s->font_owned = true;
            } else {
                std::cerr << "[UI] Custom font failed: " << def.font_path << "\n";
            }
        }
        if (!s->font) {
            sfFont *f = sfFont_createFromFile(FALLBACK_FONT_PATH);
            if (f) {
                s->font = f;
                s->font_owned = true;
            } else {
                s->font = _fallbackFont;
                s->font_owned = false;
            }
        }
        if (!def.image.empty()) {
            sfTexture *t = sfTexture_createFromFile(def.image.c_str(), nullptr);
            if (t) {
                s->texture = t;
                s->sprite  = sfSprite_create();
                sfSprite_setTexture(s->sprite, t, sfTrue);
            } else {
                std::cerr << "[UI] Image not found: " << def.image << "\n";
            }
        }
        _items.push_back(s);
    }
}

void InterfaceItems::notifyCheckPressed(float duration)
{
    for (auto *s : _items)
        if (s->def.on_check)
            s->check_timer = duration;
}

void InterfaceItems::_drawText(sfRenderWindow *window, sfFont *font,
                               const std::string &str, unsigned int size,
                               sfColor color, float x, float y)
{
    if (!font || !window) return;
    sfText *t = sfText_create();
    sfText_setFont(t, font);
    sfText_setString(t, str.c_str());
    sfText_setCharacterSize(t, size);
    sfText_setColor(t, color);
    sfText_setPosition(t, {x, y});
    sfRenderWindow_drawText(window, t, nullptr);
    sfText_destroy(t);
}

static float heartbeatScale(float timer, float hpRatio)
{
    float t = fmodf(timer, BEAT_PERIOD) / BEAT_PERIOD;
    auto gauss = [](float t, float center, float width) -> float {
        float d = (t - center) / width;
        return expf(-d * d * 8.0f);
    };
    float pulse = gauss(t, BEAT_PULSE1 / BEAT_PERIOD, 0.06f)
                + gauss(t, BEAT_PULSE2 / BEAT_PERIOD, 0.05f) * 0.7f;
    float intensity = 1.0f + (1.0f - hpRatio) * 0.6f;
    return 1.0f + pulse * (BEAT_MAX_SCALE - 1.0f) * intensity;
}

void InterfaceItems::_renderHpText(sfRenderWindow *window,
                                   InterfaceItemState *s,
                                   Player *player)
{
    if (!player || !window || !s) return;
    int currentHp = (int)player->hp;
    int maxHp = (int)player->maxHp;
    float hpRatio = (maxHp > 0) ? (float)currentHp / (float)maxHp : 0.0f;
    if (hpRatio < 0.0f) hpRatio = 0.0f;
    if (hpRatio > 1.0f) hpRatio = 1.0f;
    sfColor col = sfColor_fromRGB((sfUint8)s->def.r, (sfUint8)s->def.g, (sfUint8)s->def.b);
    if (hpRatio < 0.3f)      col = sfRed;
    else if (hpRatio < 0.6f) col = sfYellow;
    char buf[32];
    snprintf(buf, sizeof(buf), "HP: %d / %d", currentHp, maxHp);
    _drawText(window, s->font, buf, 20, col, s->def.x, s->def.y);
}

void InterfaceItems::_renderAmmoText(sfRenderWindow *window,
                                     InterfaceItemState *s,
                                     const std::vector<Weapons *> &weapons,
                                     int idx)
{
    if (weapons.empty() || idx >= (int)weapons.size() || !s || !window) return;
    Weapons *w = weapons[idx];
    if (!w) return;
    sfFont *font = s->font ? s->font : _fallbackFont;
    if (!font) return;
    sfColor col = sfColor_fromRGB((sfUint8)s->def.r, (sfUint8)s->def.g, (sfUint8)s->def.b);
    char buf[64];
    snprintf(buf, sizeof(buf), "%d | %d/%d", w->ammo, w->mag, w->max_mag);
    _drawText(window, font, buf, 20, col, s->def.x, s->def.y);
}

void InterfaceItems::_renderHpImage(sfRenderWindow *window,
                                    InterfaceItemState *s,
                                    Player *player, float dt)
{
    if (!s->sprite || !player) return;
    float hpRatio = (player->maxHp > 0)
        ? (float)player->hp / (float)player->maxHp : 0.0f;
    if (hpRatio < 0.0f) hpRatio = 0.0f;
    if (hpRatio > 1.0f) hpRatio = 1.0f;
    sfVector2u texSz = sfTexture_getSize(s->texture);
    unsigned int visibleH = (unsigned int)(texSz.y * hpRatio);
    unsigned int cropTop  = texSz.y - visibleH;
    sfIntRect rect = { 0, (int)cropTop, (int)texSz.x, (int)visibleH };
    sfSprite_setTextureRect(s->sprite, rect);
    sfColor col = sfColor_fromRGB((sfUint8)s->def.r, (sfUint8)s->def.g, (sfUint8)s->def.b);
    sfSprite_setColor(s->sprite, col);
    float scale = 1.0f;
    if (s->def.beating) {
        s->beat_timer += dt;
        scale = heartbeatScale(s->beat_timer, hpRatio);
    }
    sfSprite_setScale(s->sprite, {scale, scale});
    sfFloatRect lb = sfSprite_getLocalBounds(s->sprite);
    sfSprite_setOrigin(s->sprite, {lb.width / 2.0f, lb.height / 2.0f});
    float posX = s->def.x;
    float posY = s->def.y + (float)cropTop * scale;
    sfSprite_setPosition(s->sprite, {posX, posY});
    sfRenderWindow_drawSprite(window, s->sprite, nullptr);
}

void InterfaceItems::_renderAmmoImage(sfRenderWindow *window,
                                      InterfaceItemState *s,
                                      const std::vector<Weapons *> &weapons,
                                      int idx, float dt)
{
    if (!s->sprite || weapons.empty() || idx >= (int)weapons.size()) return;
    Weapons *w = weapons[idx];
    if (!w) return;
    float ammoRatio = (w->max_ammo > 0)
        ? (float)w->ammo / (float)w->max_ammo : 0.0f;
    if (ammoRatio < 0.0f) ammoRatio = 0.0f;
    if (ammoRatio > 1.0f) ammoRatio = 1.0f;
    sfVector2u texSz = sfTexture_getSize(s->texture);
    unsigned int visibleH = (unsigned int)(texSz.y * ammoRatio);
    unsigned int cropTop  = texSz.y - visibleH;
    sfIntRect rect = { 0, (int)cropTop, (int)texSz.x, (int)visibleH };
    sfSprite_setTextureRect(s->sprite, rect);
    sfColor col = sfColor_fromRGB((sfUint8)s->def.r, (sfUint8)s->def.g, (sfUint8)s->def.b);
    sfSprite_setColor(s->sprite, col);

    float scale = 1.0f;
    if (s->def.beating) {
        s->beat_timer += dt;
        scale = heartbeatScale(s->beat_timer, ammoRatio);
    }
    sfSprite_setScale(s->sprite, {scale, scale});
    sfFloatRect lb = sfSprite_getLocalBounds(s->sprite);
    sfSprite_setOrigin(s->sprite, {lb.width / 2.0f, lb.height / 2.0f});
    float posY = s->def.y + (float)cropTop * scale;
    sfSprite_setPosition(s->sprite, {s->def.x, posY});
    sfRenderWindow_drawSprite(window, s->sprite, nullptr);
}

void InterfaceItems::render(sfRenderWindow *window, Player *player,
                            const std::vector<Weapons *> &weapons,
                            int current_weapon_idx, float dt,
                            float ammo_display_timer)
{
    if (!window || !player) return;
    for (auto *s : _items)
        if (s->def.on_check && s->check_timer > 0.0f)
            s->check_timer -= dt;
    if (_items.empty()) {
        drawWeaponList(window, weapons, current_weapon_idx);
        drawHealthBar(window, player);
        drawAmmoInfo(window, weapons, current_weapon_idx);
        drawItemInfo(window, player, weapons);
        return;
    }
    for (auto *s : _items) {
        if (s->def.on_check && s->check_timer <= 0.0f)
            continue;
        const std::string &info = s->def.info;
        if (info == "hp") {
            if (s->texture)
                _renderHpImage(window, s, player, dt);
        }
        else if (info == "hp_text") {
            _renderHpText(window, s, player);
        }
        else if (info == "ammo") {
            if (s->texture)
                _renderAmmoImage(window, s, weapons, current_weapon_idx, dt);
        }
        else if (info == "ammo_text") {
            _renderAmmoText(window, s, weapons, current_weapon_idx);
        }
        else {
            std::cerr << "[UI] unknown info type: '" << info << "'\n";
        }
    }
}

void InterfaceItems::drawHealthBar(sfRenderWindow *window, Player *player)
{
    if (!player || !window || !_fallbackFont) return;
    sfVector2u winSize = sfRenderWindow_getSize(window);
    float barWidth = 200.0f, barHeight = 30.0f;
    float barX = 20.0f, barY = (float)winSize.y - 60.0f;
    sfRectangleShape *bgBar = sfRectangleShape_create();
    sfRectangleShape_setSize(bgBar, {barWidth, barHeight});
    sfRectangleShape_setPosition(bgBar, {barX, barY});
    sfRectangleShape_setFillColor(bgBar, sfBlack);
    sfRectangleShape_setOutlineThickness(bgBar, 2.0f);
    sfRectangleShape_setOutlineColor(bgBar, sfWhite);
    sfRenderWindow_drawRectangleShape(window, bgBar, nullptr);
    float healthRatio = (player->maxHp > 0)
        ? (float)player->hp / (float)player->maxHp : 0.0f;
    if (healthRatio < 0.0f) healthRatio = 0.0f;
    if (healthRatio > 1.0f) healthRatio = 1.0f;
    sfColor healthColor = sfGreen;
    if (healthRatio < 0.3f)      healthColor = sfRed;
    else if (healthRatio < 0.6f) healthColor = sfYellow;
    sfRectangleShape *healthFill = sfRectangleShape_create();
    sfRectangleShape_setSize(healthFill, {barWidth * healthRatio - 4.0f, barHeight - 4.0f});
    sfRectangleShape_setPosition(healthFill, {barX + 2.0f, barY + 2.0f});
    sfRectangleShape_setFillColor(healthFill, healthColor);
    sfRenderWindow_drawRectangleShape(window, healthFill, nullptr);
    char healthStr[32];
    snprintf(healthStr, sizeof(healthStr), "HP: %d/%d", player->hp, player->maxHp);
    _drawText(window, _fallbackFont, healthStr, 14, sfWhite, barX + 50.0f, barY + 7.0f);
    sfRectangleShape_destroy(bgBar);
    sfRectangleShape_destroy(healthFill);
}

void InterfaceItems::drawAmmoInfo(sfRenderWindow *window,
                                  const std::vector<Weapons *> &weapons,
                                  int current_weapon_idx)
{
    if (weapons.empty() || current_weapon_idx >= (int)weapons.size()
        || !window || !_fallbackFont) return;
    Weapons *w = weapons[current_weapon_idx];
    sfVector2u winSize = sfRenderWindow_getSize(window);
    float infoX = (float)winSize.x - 250.0f;
    float infoY = (float)winSize.y - 60.0f;
    sfRectangleShape *ammoBg = sfRectangleShape_create();
    sfRectangleShape_setSize(ammoBg, {220.0f, 50.0f});
    sfRectangleShape_setPosition(ammoBg, {infoX, infoY});
    sfRectangleShape_setFillColor(ammoBg, sfBlack);
    sfRectangleShape_setOutlineThickness(ammoBg, 2.0f);
    sfRectangleShape_setOutlineColor(ammoBg, sfWhite);
    sfRenderWindow_drawRectangleShape(window, ammoBg, nullptr);
    _drawText(window, _fallbackFont, w->name, 16, sfYellow, infoX + 10.0f, infoY + 5.0f);
    char ammoStr[64];
    snprintf(ammoStr, sizeof(ammoStr), "Ammo: %d | Mag: %d/%d", w->ammo, w->mag, w->max_mag);
    _drawText(window, _fallbackFont, ammoStr, 14, sfWhite, infoX + 10.0f, infoY + 25.0f);
    sfRectangleShape_destroy(ammoBg);
}

void InterfaceItems::drawWeaponList(sfRenderWindow *window,
                                    const std::vector<Weapons *> &weapons,
                                    int current_weapon_idx)
{
    if (weapons.empty() || !window || !_fallbackFont) return;
    float listX = 20.0f, listY = 20.0f, itemHeight = 25.0f;
    _drawText(window, _fallbackFont, "WEAPONS", 18, sfYellow, listX, listY);
    for (size_t i = 0; i < weapons.size() && i < 9; i++) {
        Weapons *w = weapons[i];
        if (!w) continue;
        float itemY = listY + 30.0f + (float)i * itemHeight;
        if ((int)i == current_weapon_idx) {
            sfRectangleShape *bg = sfRectangleShape_create();
            sfRectangleShape_setSize(bg, {200.0f, itemHeight - 5.0f});
            sfRectangleShape_setPosition(bg, {listX, itemY});
            sfRectangleShape_setFillColor(bg, sfColor_fromRGB(50, 50, 100));
            sfRenderWindow_drawRectangleShape(window, bg, nullptr);
            sfRectangleShape_destroy(bg);
        }
        char weaponStr[64];
        snprintf(weaponStr, sizeof(weaponStr), "[%zu] %s (%d)", i + 1, w->name.c_str(), w->ammo);
        _drawText(window, _fallbackFont, weaponStr, 14,
                  (int)i == current_weapon_idx ? sfGreen : sfWhite,
                  listX + 10.0f, itemY + 2.0f);
    }
}

void InterfaceItems::drawItemInfo(sfRenderWindow *window, Player *player,
                                  const std::vector<Weapons *> &weapons)
{
    if (!player || !window || !_fallbackFont) return;
    sfVector2u winSize = sfRenderWindow_getSize(window);
    float infoX = (float)winSize.x / 2.0f - 100.0f;
    float infoY = 20.0f;
    sfRectangleShape *infoBg = sfRectangleShape_create();
    sfRectangleShape_setSize(infoBg, {200.0f, 80.0f});
    sfRectangleShape_setPosition(infoBg, {infoX, infoY});
    sfRectangleShape_setFillColor(infoBg, sfBlack);
    sfRectangleShape_setOutlineThickness(infoBg, 2.0f);
    sfRectangleShape_setOutlineColor(infoBg, sfYellow);
    sfRenderWindow_drawRectangleShape(window, infoBg, nullptr);
    _drawText(window, _fallbackFont, "STATUS", 16, sfYellow, infoX + 70.0f, infoY + 5.0f);
    int totalAmmo = 0;
    for (const auto *w : weapons) if (w) totalAmmo += w->ammo;
    char statsStr[128];
    snprintf(statsStr, sizeof(statsStr), "HP: %d/%d  Weapons: %zu  Ammo: %d",
             player->hp, player->maxHp, weapons.size(), totalAmmo);
    _drawText(window, _fallbackFont, statsStr, 12, sfWhite, infoX + 10.0f, infoY + 30.0f);
    sfRectangleShape_destroy(infoBg);
}

std::vector<InterfaceItemDef> buildInterfaceItems(char ***parsed_lines, bool debug)
{
    if (debug)
        std::cout << "Parsing interface items from game data..." << std::endl;

    std::vector<InterfaceItemDef> defs;

    for (int i = 0; parsed_lines[i] != nullptr; i++) {
        if (parsed_lines[i][0] != nullptr && strcmp(parsed_lines[i][0], "interface_item") == 0) {
            InterfaceItemDef def;
            for (int j = 1; parsed_lines[i][j] != nullptr; j++) {
                
                if (strcmp(parsed_lines[i][j], "info") == 0 && parsed_lines[i][j + 1] != nullptr) {
                    def.info = parsed_lines[i][j + 1];
                }
                if (strcmp(parsed_lines[i][j], "image") == 0 && parsed_lines[i][j + 1] != nullptr) {
                    def.image = parsed_lines[i][j + 1];
                }
                if (strcmp(parsed_lines[i][j], "font") == 0 && parsed_lines[i][j + 1] != nullptr) {
                    def.font_path = parsed_lines[i][j + 1];
                }
                if (strcmp(parsed_lines[i][j], "x") == 0 && parsed_lines[i][j + 1] != nullptr) {
                    def.x = std::stof(parsed_lines[i][j + 1]);
                }
                if (strcmp(parsed_lines[i][j], "y") == 0 && parsed_lines[i][j + 1] != nullptr) {
                    def.y = std::stof(parsed_lines[i][j + 1]);
                }
                if (strcmp(parsed_lines[i][j], "r") == 0 && parsed_lines[i][j + 1] != nullptr) {
                    def.r = std::stoi(parsed_lines[i][j + 1]);
                }
                if (strcmp(parsed_lines[i][j], "g") == 0 && parsed_lines[i][j + 1] != nullptr) {
                    def.g = std::stoi(parsed_lines[i][j + 1]);
                }
                if (strcmp(parsed_lines[i][j], "b") == 0 && parsed_lines[i][j + 1] != nullptr) {
                    def.b = std::stoi(parsed_lines[i][j + 1]);
                }
                if (strcmp(parsed_lines[i][j], "beating") == 0 && parsed_lines[i][j + 1] != nullptr) {
                    def.beating = (strcmp(parsed_lines[i][j + 1], "true") == 0);
                }
                if (strcmp(parsed_lines[i][j], "on_check") == 0 && parsed_lines[i][j + 1] != nullptr) {
                    def.on_check = (strcmp(parsed_lines[i][j + 1], "true") == 0);
                }
            }
            defs.push_back(def);
        }
    }
    return defs;
}