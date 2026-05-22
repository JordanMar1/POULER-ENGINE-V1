/*
** EPITECH PROJECT, 2026
** MY_GZDOOM
** File description:
** Settings
*/

#include "Settings.hpp"
#include "Window.hpp"
#include "Menu.hpp"
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>

static std::string trim(const std::string &s)
{
    auto begin = s.find_first_not_of(" \t\r\n");
    if (begin == std::string::npos)
        return "";
    auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(begin, end - begin + 1);
}

static sfMouseButton mouseButtonFromString(const std::string &button)
{
    if (button == "mouse_left") return sfMouseLeft;
    if (button == "mouse_right") return sfMouseRight;
    if (button == "mouse_middle") return sfMouseMiddle;
    return sfMouseButtonCount;
}

static std::string mouseButtonToString(sfMouseButton button)
{
    switch (button) {
        case sfMouseLeft: return "mouse_left";
        case sfMouseRight: return "mouse_right";
        case sfMouseMiddle: return "mouse_middle";
        default: return "unknown";
    }
}

static std::string keyboardKeyToString(sfKeyCode key)
{
    switch (key) {
        case sfKeyA: return "A";
        case sfKeyB: return "B";
        case sfKeyC: return "C";
        case sfKeyD: return "D";
        case sfKeyE: return "E";
        case sfKeyF: return "F";
        case sfKeyG: return "G";
        case sfKeyH: return "H";
        case sfKeyI: return "I";
        case sfKeyJ: return "J";
        case sfKeyK: return "K";
        case sfKeyL: return "L";
        case sfKeyM: return "M";
        case sfKeyN: return "N";
        case sfKeyO: return "O";
        case sfKeyP: return "P";
        case sfKeyQ: return "Q";
        case sfKeyR: return "R";
        case sfKeyS: return "S";
        case sfKeyT: return "T";
        case sfKeyU: return "U";
        case sfKeyV: return "V";
        case sfKeyW: return "W";
        case sfKeyX: return "X";
        case sfKeyY: return "Y";
        case sfKeyZ: return "Z";
        case sfKeyNum0: return "Num0";
        case sfKeyNum1: return "Num1";
        case sfKeyNum2: return "Num2";
        case sfKeyNum3: return "Num3";
        case sfKeyNum4: return "Num4";
        case sfKeyNum5: return "Num5";
        case sfKeyNum6: return "Num6";
        case sfKeyNum7: return "Num7";
        case sfKeyNum8: return "Num8";
        case sfKeyNum9: return "Num9";
        case sfKeyEscape: return "Escape";
        case sfKeyLControl: return "LControl";
        case sfKeyLShift: return "LShift";
        case sfKeyLAlt: return "LAlt";
        case sfKeyLSystem: return "LSystem";
        case sfKeyRControl: return "RControl";
        case sfKeyRShift: return "RShift";
        case sfKeyRAlt: return "RAlt";
        case sfKeyRSystem: return "RSystem";
        case sfKeyMenu: return "Menu";
        case sfKeyLBracket: return "LBracket";
        case sfKeyRBracket: return "RBracket";
        case sfKeySemicolon: return "Semicolon";
        case sfKeyComma: return "Comma";
        case sfKeyPeriod: return "Period";
        case sfKeyQuote: return "Quote";
        case sfKeySlash: return "Slash";
        case sfKeyBackslash: return "Backslash";
        case sfKeyTilde: return "Tilde";
        case sfKeyEqual: return "Equal";
        case sfKeyHyphen: return "Hyphen";
        case sfKeySpace: return "Space";
        case sfKeyEnter: return "Enter";
        case sfKeyBackspace: return "Backspace";
        case sfKeyTab: return "Tab";
        case sfKeyPageUp: return "PageUp";
        case sfKeyPageDown: return "PageDown";
        case sfKeyEnd: return "End";
        case sfKeyHome: return "Home";
        case sfKeyInsert: return "Insert";
        case sfKeyDelete: return "Delete";
        case sfKeyAdd: return "Add";
        case sfKeySubtract: return "Subtract";
        case sfKeyMultiply: return "Multiply";
        case sfKeyDivide: return "Divide";
        case sfKeyLeft: return "Left";
        case sfKeyRight: return "Right";
        case sfKeyUp: return "Up";
        case sfKeyDown: return "Down";
        case sfKeyNumpad0: return "Numpad0";
        case sfKeyNumpad1: return "Numpad1";
        case sfKeyNumpad2: return "Numpad2";
        case sfKeyNumpad3: return "Numpad3";
        case sfKeyNumpad4: return "Numpad4";
        case sfKeyNumpad5: return "Numpad5";
        case sfKeyNumpad6: return "Numpad6";
        case sfKeyNumpad7: return "Numpad7";
        case sfKeyNumpad8: return "Numpad8";
        case sfKeyNumpad9: return "Numpad9";
        case sfKeyF1: return "F1";
        case sfKeyF2: return "F2";
        case sfKeyF3: return "F3";
        case sfKeyF4: return "F4";
        case sfKeyF5: return "F5";
        case sfKeyF6: return "F6";
        case sfKeyF7: return "F7";
        case sfKeyF8: return "F8";
        case sfKeyF9: return "F9";
        case sfKeyF10: return "F10";
        case sfKeyF11: return "F11";
        case sfKeyF12: return "F12";
        case sfKeyF13: return "F13";
        case sfKeyF14: return "F14";
        case sfKeyF15: return "F15";
        case sfKeyPause: return "Pause";
        default: return "Unknown";
    }
}

static sfKeyCode keyboardKeyFromString(const std::string &str)
{
    if (str == "A") return sfKeyA;
    if (str == "B") return sfKeyB;
    if (str == "C") return sfKeyC;
    if (str == "D") return sfKeyD;
    if (str == "E") return sfKeyE;
    if (str == "F") return sfKeyF;
    if (str == "G") return sfKeyG;
    if (str == "H") return sfKeyH;
    if (str == "I") return sfKeyI;
    if (str == "J") return sfKeyJ;
    if (str == "K") return sfKeyK;
    if (str == "L") return sfKeyL;
    if (str == "M") return sfKeyM;
    if (str == "N") return sfKeyN;
    if (str == "O") return sfKeyO;
    if (str == "P") return sfKeyP;
    if (str == "Q") return sfKeyQ;
    if (str == "R") return sfKeyR;
    if (str == "S") return sfKeyS;
    if (str == "T") return sfKeyT;
    if (str == "U") return sfKeyU;
    if (str == "V") return sfKeyV;
    if (str == "W") return sfKeyW;
    if (str == "X") return sfKeyX;
    if (str == "Y") return sfKeyY;
    if (str == "Z") return sfKeyZ;
    if (str == "Num0") return sfKeyNum0;
    if (str == "Num1") return sfKeyNum1;
    if (str == "Num2") return sfKeyNum2;
    if (str == "Num3") return sfKeyNum3;
    if (str == "Num4") return sfKeyNum4;
    if (str == "Num5") return sfKeyNum5;
    if (str == "Num6") return sfKeyNum6;
    if (str == "Num7") return sfKeyNum7;
    if (str == "Num8") return sfKeyNum8;
    if (str == "Num9") return sfKeyNum9;
    if (str == "Escape") return sfKeyEscape;
    if (str == "LControl") return sfKeyLControl;
    if (str == "LShift") return sfKeyLShift;
    if (str == "LAlt") return sfKeyLAlt;
    if (str == "LSystem") return sfKeyLSystem;
    if (str == "RControl") return sfKeyRControl;
    if (str == "RShift") return sfKeyRShift;
    if (str == "RAlt") return sfKeyRAlt;
    if (str == "RSystem") return sfKeyRSystem;
    if (str == "Menu") return sfKeyMenu;
    if (str == "LBracket") return sfKeyLBracket;
    if (str == "RBracket") return sfKeyRBracket;
    if (str == "Semicolon") return sfKeySemicolon;
    if (str == "Comma") return sfKeyComma;
    if (str == "Period") return sfKeyPeriod;
    if (str == "Quote") return sfKeyQuote;
    if (str == "Slash") return sfKeySlash;
    if (str == "Backslash") return sfKeyBackslash;
    if (str == "Tilde") return sfKeyTilde;
    if (str == "Equal") return sfKeyEqual;
    if (str == "Hyphen") return sfKeyHyphen;
    if (str == "Space") return sfKeySpace;
    if (str == "Enter") return sfKeyEnter;
    if (str == "Backspace") return sfKeyBackspace;
    if (str == "Tab") return sfKeyTab;
    if (str == "PageUp") return sfKeyPageUp;
    if (str == "PageDown") return sfKeyPageDown;
    if (str == "End") return sfKeyEnd;
    if (str == "Home") return sfKeyHome;
    if (str == "Insert") return sfKeyInsert;
    if (str == "Delete") return sfKeyDelete;
    if (str == "Add") return sfKeyAdd;
    if (str == "Subtract") return sfKeySubtract;
    if (str == "Multiply") return sfKeyMultiply;
    if (str == "Divide") return sfKeyDivide;
    if (str == "Left") return sfKeyLeft;
    if (str == "Right") return sfKeyRight;
    if (str == "Up") return sfKeyUp;
    if (str == "Down") return sfKeyDown;
    if (str == "Numpad0") return sfKeyNumpad0;
    if (str == "Numpad1") return sfKeyNumpad1;
    if (str == "Numpad2") return sfKeyNumpad2;
    if (str == "Numpad3") return sfKeyNumpad3;
    if (str == "Numpad4") return sfKeyNumpad4;
    if (str == "Numpad5") return sfKeyNumpad5;
    if (str == "Numpad6") return sfKeyNumpad6;
    if (str == "Numpad7") return sfKeyNumpad7;
    if (str == "Numpad8") return sfKeyNumpad8;
    if (str == "Numpad9") return sfKeyNumpad9;
    if (str == "F1") return sfKeyF1;
    if (str == "F2") return sfKeyF2;
    if (str == "F3") return sfKeyF3;
    if (str == "F4") return sfKeyF4;
    if (str == "F5") return sfKeyF5;
    if (str == "F6") return sfKeyF6;
    if (str == "F7") return sfKeyF7;
    if (str == "F8") return sfKeyF8;
    if (str == "F9") return sfKeyF9;
    if (str == "F10") return sfKeyF10;
    if (str == "F11") return sfKeyF11;
    if (str == "F12") return sfKeyF12;
    if (str == "F13") return sfKeyF13;
    if (str == "F14") return sfKeyF14;
    if (str == "F15") return sfKeyF15;
    if (str == "Pause") return sfKeyPause;
    return sfKeyUnknown;
}

static void writeDefaultBinds(const std::string &path, const Settings::Binds &binds)
{
    std::ofstream output(path, std::ios::trunc);
    if (!output.is_open())
        return;

    output << "move_forward=" << keyboardKeyToString(binds.moveForward) << "\n";
    output << "move_left=" << keyboardKeyToString(binds.moveLeft) << "\n";
    output << "move_back=" << keyboardKeyToString(binds.moveBack) << "\n";
    output << "move_right=" << keyboardKeyToString(binds.moveRight) << "\n";
    output << "crouch=" << keyboardKeyToString(binds.crouch) << "\n";
    output << "reload=" << keyboardKeyToString(binds.reload) << "\n"; // Ajouté
    output << "shoot=" << mouseButtonToString(binds.shoot) << "\n";
    output << "aim=" << mouseButtonToString(binds.aim) << "\n";
}

Settings::Settings()
{
    std::string path("settings.conf");
    bool writeDefaults = false;

    std::ifstream input(path);
    if (!input.is_open()) {
        writeDefaults = true;
    } else {
        input.seekg(0, std::ios::end);
        if (input.tellg() <= 0)
            writeDefaults = true;
        else {
            input.seekg(0);
            std::string line;
            while (std::getline(input, line)) {
                line = trim(line);
                if (line.empty() || line[0] == '#')
                    continue;
                auto eq = line.find('=');
                if (eq == std::string::npos)
                    continue;
                std::string key = trim(line.substr(0, eq));
                std::string val = trim(line.substr(eq + 1));
                if (key == "move_forward")
                    binds.moveForward = keyboardKeyFromString(val);
                else if (key == "move_left")
                    binds.moveLeft = keyboardKeyFromString(val);
                else if (key == "move_back")
                    binds.moveBack = keyboardKeyFromString(val);
                else if (key == "move_right")
                    binds.moveRight = keyboardKeyFromString(val);
                else if (key == "crouch")
                    binds.crouch = keyboardKeyFromString(val);
                else if (key == "reload") // Ajouté
                    binds.reload = keyboardKeyFromString(val);
                else if (key == "shoot") {
                    auto m = mouseButtonFromString(val);
                    if (m != sfMouseButtonCount) binds.shoot = m;
                } else if (key == "aim") {
                    auto m = mouseButtonFromString(val);
                    if (m != sfMouseButtonCount) binds.aim = m;
                }
            }
            if (binds.moveForward == sfKeyUnknown ||
                binds.moveLeft == sfKeyUnknown ||
                binds.moveBack == sfKeyUnknown ||
                binds.moveRight == sfKeyUnknown ||
                binds.crouch == sfKeyUnknown ||
                binds.reload == sfKeyUnknown || // Ajouté
                binds.shoot == sfMouseButtonCount ||
                binds.aim == sfMouseButtonCount) {
                writeDefaults = true;
            }
        }
        input.close();
    }
    if (writeDefaults) {
        writeDefaultBinds(path, binds);
    }
}

Settings::~Settings()
{
}

void Settings::changeSettings(Window& window, Menu& menu)
{
    sfFont* font = sfFont_createFromFile("assets/fonts/menu.ttf");
    if (!font) {
        return;
    }
    sfRenderWindow* renderWindow = window.getWindow();

    sfText* title = sfText_create();
    sfText_setFont(title, font);
    sfText_setCharacterSize(title, 30);
    sfText_setString(title, "Settings");
    sfText_setPosition(title, (sfVector2f){100, 50});
    sfText_setColor(title, sfWhite);

    sfText* moveForwardText = sfText_create();
    sfText_setFont(moveForwardText, font);
    sfText_setCharacterSize(moveForwardText, 20);
    sfText_setPosition(moveForwardText, (sfVector2f){100, 150});
    sfText_setColor(moveForwardText, sfWhite);

    sfText* moveLeftText = sfText_create();
    sfText_setFont(moveLeftText, font);
    sfText_setCharacterSize(moveLeftText, 20);
    sfText_setPosition(moveLeftText, (sfVector2f){100, 200});
    sfText_setColor(moveLeftText, sfWhite);

    sfText* moveBackText = sfText_create();
    sfText_setFont(moveBackText, font);
    sfText_setCharacterSize(moveBackText, 20);
    sfText_setPosition(moveBackText, (sfVector2f){100, 250});
    sfText_setColor(moveBackText, sfWhite);

    sfText* moveRightText = sfText_create();
    sfText_setFont(moveRightText, font);
    sfText_setCharacterSize(moveRightText, 20);
    sfText_setPosition(moveRightText, (sfVector2f){100, 300});
    sfText_setColor(moveRightText, sfWhite);

    sfText* crouchText = sfText_create();
    sfText_setFont(crouchText, font);
    sfText_setCharacterSize(crouchText, 20);
    sfText_setPosition(crouchText, (sfVector2f){100, 350});
    sfText_setColor(crouchText, sfWhite);

    sfText* shootText = sfText_create();
    sfText_setFont(shootText, font);
    sfText_setCharacterSize(shootText, 20);
    sfText_setPosition(shootText, (sfVector2f){100, 400});
    sfText_setColor(shootText, sfWhite);

    sfText* aimText = sfText_create();
    sfText_setFont(aimText, font);
    sfText_setCharacterSize(aimText, 20);
    sfText_setPosition(aimText, (sfVector2f){100, 450});
    sfText_setColor(aimText, sfWhite);

    // Ajout graphique du texte Reload à Y = 500
    sfText* reloadText = sfText_create();
    sfText_setFont(reloadText, font);
    sfText_setCharacterSize(reloadText, 20);
    sfText_setPosition(reloadText, (sfVector2f){100, 500});
    sfText_setColor(reloadText, sfWhite);

    // On descend un peu le texte d'attente à Y = 580 pour laisser de la place
    sfText* waitingText = sfText_create();
    sfText_setFont(waitingText, font);
    sfText_setCharacterSize(waitingText, 20);
    sfText_setPosition(waitingText, (sfVector2f){100, 580});
    sfText_setColor(waitingText, sfYellow);

    updateTexts(moveForwardText, moveLeftText, moveBackText, moveRightText, crouchText, shootText, aimText, reloadText);

    enum State { Normal, WaitingKey, WaitingMouse };
    State state = Normal;
    int waitingFor = -1;

    while (sfRenderWindow_isOpen(renderWindow)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(renderWindow, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(renderWindow);
                sfText_destroy(title);
                sfText_destroy(moveForwardText);
                sfText_destroy(moveLeftText);
                sfText_destroy(moveBackText);
                sfText_destroy(moveRightText);
                sfText_destroy(crouchText);
                sfText_destroy(shootText);
                sfText_destroy(aimText);
                sfText_destroy(reloadText); // Ajouté
                sfText_destroy(waitingText);
                sfFont_destroy(font);
                return;
            }
            if (event.type == sfEvtKeyPressed) {
                if (state == Normal) {
                    if (event.key.code == sfKeyNum1) {
                        state = WaitingKey;
                        waitingFor = 1;
                        sfText_setString(waitingText, "Press new key for Move Forward");
                    } else if (event.key.code == sfKeyNum2) {
                        state = WaitingKey;
                        waitingFor = 2;
                        sfText_setString(waitingText, "Press new key for Move Left");
                    } else if (event.key.code == sfKeyNum3) {
                        state = WaitingKey;
                        waitingFor = 3;
                        sfText_setString(waitingText, "Press new key for Move Back");
                    } else if (event.key.code == sfKeyNum4) {
                        state = WaitingKey;
                        waitingFor = 4;
                        sfText_setString(waitingText, "Press new key for Move Right");
                    } else if (event.key.code == sfKeyNum5) {
                        state = WaitingKey;
                        waitingFor = 5;
                        sfText_setString(waitingText, "Press new key for Crouch");
                    } else if (event.key.code == sfKeyNum6) {
                        state = WaitingMouse;
                        waitingFor = 6;
                        sfText_setString(waitingText, "Press new mouse button for Shoot");
                    } else if (event.key.code == sfKeyNum7) {
                        state = WaitingMouse;
                        waitingFor = 7;
                        sfText_setString(waitingText, "Press new mouse button for Aim");
                    } else if (event.key.code == sfKeyNum8) { // Ajouté
                        state = WaitingKey;
                        waitingFor = 8;
                        sfText_setString(waitingText, "Press new key for Reload");
                    } else if (event.key.code == sfKeyEscape) {
                        sfText_destroy(title);
                        sfText_destroy(moveForwardText);
                        sfText_destroy(moveLeftText);
                        sfText_destroy(moveBackText);
                        sfText_destroy(moveRightText);
                        sfText_destroy(crouchText);
                        sfText_destroy(shootText);
                        sfText_destroy(aimText);
                        sfText_destroy(reloadText); // Ajouté
                        sfText_destroy(waitingText);
                        sfFont_destroy(font);
                        return;
                    }
                } else if (state == WaitingKey) {
                    if (event.key.code != sfKeyUnknown) {
                        if (waitingFor == 1) binds.moveForward = event.key.code;
                        else if (waitingFor == 2) binds.moveLeft = event.key.code;
                        else if (waitingFor == 3) binds.moveBack = event.key.code;
                        else if (waitingFor == 4) binds.moveRight = event.key.code;
                        else if (waitingFor == 5) binds.crouch = event.key.code;
                        else if (waitingFor == 8) binds.reload = event.key.code; // Ajouté
                        updateTexts(moveForwardText, moveLeftText, moveBackText, moveRightText, crouchText, shootText, aimText, reloadText);
                        saveSettings();
                        state = Normal;
                        sfText_setString(waitingText, "");
                    }
                }
            } else if (event.type == sfEvtMouseButtonPressed) {
                if (state == Normal && event.mouseButton.button == sfMouseLeft) {
                    int y = event.mouseButton.y;
                    if (y >= 150 && y < 200) {
                        state = WaitingKey;
                        waitingFor = 1;
                        sfText_setString(waitingText, "Press new key for Move Forward");
                    } else if (y >= 200 && y < 250) {
                        state = WaitingKey;
                        waitingFor = 2;
                        sfText_setString(waitingText, "Press new key for Move Left");
                    } else if (y >= 250 && y < 300) {
                        state = WaitingKey;
                        waitingFor = 3;
                        sfText_setString(waitingText, "Press new key for Move Back");
                    } else if (y >= 300 && y < 350) {
                        state = WaitingKey;
                        waitingFor = 4;
                        sfText_setString(waitingText, "Press new key for Move Right");
                    } else if (y >= 350 && y < 400) {
                        state = WaitingKey;
                        waitingFor = 5;
                        sfText_setString(waitingText, "Press new key for Crouch");
                    } else if (y >= 400 && y < 450) {
                        state = WaitingMouse;
                        waitingFor = 6;
                        sfText_setString(waitingText, "Press new mouse button for Shoot");
                    } else if (y >= 450 && y < 500) {
                        state = WaitingMouse;
                        waitingFor = 7;
                        sfText_setString(waitingText, "Press new mouse button for Aim");
                    } else if (y >= 500 && y < 550) { // Ajouté (Zone cliquable du Reload)
                        state = WaitingKey;
                        waitingFor = 8;
                        sfText_setString(waitingText, "Press new key for Reload");
                    }
                } else if (state == WaitingMouse) {
                    if (waitingFor == 6) binds.shoot = event.mouseButton.button;
                    else if (waitingFor == 7) binds.aim = event.mouseButton.button;
                    updateTexts(moveForwardText, moveLeftText, moveBackText, moveRightText, crouchText, shootText, aimText, reloadText);
                    saveSettings();
                    state = Normal;
                    sfText_setString(waitingText, "");
                }
            }
        }
        sfRenderWindow_clear(renderWindow, sfBlack);
        menu.display(renderWindow, 2);
        sfRenderWindow_drawText(renderWindow, title, NULL);
        sfRenderWindow_drawText(renderWindow, moveForwardText, NULL);
        sfRenderWindow_drawText(renderWindow, moveLeftText, NULL);
        sfRenderWindow_drawText(renderWindow, moveBackText, NULL);
        sfRenderWindow_drawText(renderWindow, moveRightText, NULL);
        sfRenderWindow_drawText(renderWindow, crouchText, NULL);
        sfRenderWindow_drawText(renderWindow, shootText, NULL);
        sfRenderWindow_drawText(renderWindow, aimText, NULL);
        sfRenderWindow_drawText(renderWindow, reloadText, NULL); // Ajouté
        if (state != Normal) {
            sfRenderWindow_drawText(renderWindow, waitingText, NULL);
        }
        sfRenderWindow_display(renderWindow);
    }
    sfText_destroy(title);
    sfText_destroy(moveForwardText);
    sfText_destroy(moveLeftText);
    sfText_destroy(moveBackText);
    sfText_destroy(moveRightText);
    sfText_destroy(crouchText);
    sfText_destroy(shootText);
    sfText_destroy(aimText);
    sfText_destroy(reloadText); // Ajouté
    sfText_destroy(waitingText);
    sfFont_destroy(font);
}

void Settings::updateTexts(sfText* mf, sfText* ml, sfText* mb, sfText* mr, sfText* c, sfText* s, sfText* a, sfText* r)
{
    sfText_setString(mf, ("1. Move Forward: " + keyboardKeyToString(binds.moveForward)).c_str());
    sfText_setString(ml, ("2. Move Left: " + keyboardKeyToString(binds.moveLeft)).c_str());
    sfText_setString(mb, ("3. Move Back: " + keyboardKeyToString(binds.moveBack)).c_str());
    sfText_setString(mr, ("4. Move Right: " + keyboardKeyToString(binds.moveRight)).c_str());
    sfText_setString(c,  ("5. Crouch: " + keyboardKeyToString(binds.crouch)).c_str());
    sfText_setString(s,  ("6. Shoot: " + mouseButtonToString(binds.shoot)).c_str());
    sfText_setString(a,  ("7. Aim: " + mouseButtonToString(binds.aim)).c_str());
    sfText_setString(r,  ("8. Reload: " + keyboardKeyToString(binds.reload)).c_str()); // Ajouté
}

void Settings::saveSettings()
{
    std::string path("settings.conf");
    writeDefaultBinds(path, binds);
}