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
#include <sstream>

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
        case sfKeyA: return "A"; case sfKeyB: return "B"; case sfKeyC: return "C";
        case sfKeyD: return "D"; case sfKeyE: return "E"; case sfKeyF: return "F";
        case sfKeyG: return "G"; case sfKeyH: return "H"; case sfKeyI: return "I";
        case sfKeyJ: return "J"; case sfKeyK: return "K"; case sfKeyL: return "L";
        case sfKeyM: return "M"; case sfKeyN: return "N"; case sfKeyO: return "O";
        case sfKeyP: return "P"; case sfKeyQ: return "Q"; case sfKeyR: return "R";
        case sfKeyS: return "S"; case sfKeyT: return "T"; case sfKeyU: return "U";
        case sfKeyV: return "V"; case sfKeyW: return "W"; case sfKeyX: return "X";
        case sfKeyY: return "Y"; case sfKeyZ: return "Z";
        case sfKeyNum0: return "Num0"; case sfKeyNum1: return "Num1"; case sfKeyNum2: return "Num2";
        case sfKeyNum3: return "Num3"; case sfKeyNum4: return "Num4"; case sfKeyNum5: return "Num5";
        case sfKeyNum6: return "Num6"; case sfKeyNum7: return "Num7"; case sfKeyNum8: return "Num8";
        case sfKeyNum9: return "Num9"; case sfKeyEscape: return "Escape";
        case sfKeyLControl: return "LControl"; case sfKeyLShift: return "LShift"; case sfKeyLAlt: return "LAlt";
        case sfKeyLSystem: return "LSystem"; case sfKeyRControl: return "RControl"; case sfKeyRShift: return "RShift";
        case sfKeyRAlt: return "RAlt"; case sfKeyRSystem: return "RSystem"; case sfKeyMenu: return "Menu";
        case sfKeyLBracket: return "LBracket"; case sfKeyRBracket: return "RBracket"; case sfKeySemicolon: return "Semicolon";
        case sfKeyComma: return "Comma"; case sfKeyPeriod: return "Period"; case sfKeyQuote: return "Quote";
        case sfKeySlash: return "Slash"; case sfKeyBackslash: return "Backslash"; case sfKeyTilde: return "Tilde";
        case sfKeyEqual: return "Equal"; case sfKeyHyphen: return "Hyphen"; case sfKeySpace: return "Space";
        case sfKeyEnter: return "Enter"; case sfKeyBackspace: return "Backspace"; case sfKeyTab: return "Tab";
        case sfKeyPageUp: return "PageUp"; case sfKeyPageDown: return "PageDown"; case sfKeyEnd: return "End";
        case sfKeyHome: return "Home"; case sfKeyInsert: return "Insert"; case sfKeyDelete: return "Delete";
        case sfKeyAdd: return "Add"; case sfKeySubtract: return "Subtract"; case sfKeyMultiply: return "Multiply";
        case sfKeyDivide: return "Divide"; case sfKeyLeft: return "Left"; case sfKeyRight: return "Right";
        case sfKeyUp: return "Up"; case sfKeyDown: return "Down";
        case sfKeyNumpad0: return "Numpad0"; case sfKeyNumpad1: return "Numpad1"; case sfKeyNumpad2: return "Numpad2";
        case sfKeyNumpad3: return "Numpad3"; case sfKeyNumpad4: return "Numpad4"; case sfKeyNumpad5: return "Numpad5";
        case sfKeyNumpad6: return "Numpad6"; case sfKeyNumpad7: return "Numpad7"; case sfKeyNumpad8: return "Numpad8";
        case sfKeyNumpad9: return "Numpad9";
        case sfKeyF1: return "F1"; case sfKeyF2: return "F2"; case sfKeyF3: return "F3";
        case sfKeyF4: return "F4"; case sfKeyF5: return "F5"; case sfKeyF6: return "F6";
        case sfKeyF7: return "F7"; case sfKeyF8: return "F8"; case sfKeyF9: return "F9";
        case sfKeyF10: return "F10"; case sfKeyF11: return "F11"; case sfKeyF12: return "F12";
        case sfKeyF13: return "F13"; case sfKeyF14: return "F14"; case sfKeyF15: return "F15";
        case sfKeyPause: return "Pause";
        default: return "Unknown";
    }
}

static sfKeyCode keyboardKeyFromString(const std::string &str)
{
    if (str == "A") return sfKeyA;
    else if (str == "B") return sfKeyB;
    else if (str == "C") return sfKeyC;
    else if (str == "D") return sfKeyD;
    else if (str == "E") return sfKeyE;
    else if (str == "F") return sfKeyF;
    else if (str == "G") return sfKeyG;
    else if (str == "H") return sfKeyH;
    else if (str == "I") return sfKeyI;
    else if (str == "J") return sfKeyJ;
    else if (str == "K") return sfKeyK;
    else if (str == "L") return sfKeyL;
    else if (str == "M") return sfKeyM;
    else if (str == "N") return sfKeyN;
    else if (str == "O") return sfKeyO;
    else if (str == "P") return sfKeyP;
    else if (str == "Q") return sfKeyQ;
    else if (str == "R") return sfKeyR;
    else if (str == "S") return sfKeyS;
    else if (str == "T") return sfKeyT;
    else if (str == "U") return sfKeyU;
    else if (str == "V") return sfKeyV;
    else if (str == "W") return sfKeyW;
    else if (str == "X") return sfKeyX;
    else if (str == "Y") return sfKeyY;
    else if (str == "Z") return sfKeyZ;
    else if (str == "Num0") return sfKeyNum0;
    else if (str == "Num1") return sfKeyNum1;
    else if (str == "Num2") return sfKeyNum2;
    else if (str == "Num3") return sfKeyNum3;
    else if (str == "Num4") return sfKeyNum4;
    else if (str == "Num5") return sfKeyNum5;
    else if (str == "Num6") return sfKeyNum6;
    else if (str == "Num7") return sfKeyNum7;
    else if (str == "Num8") return sfKeyNum8;
    else if (str == "Num9") return sfKeyNum9;
    else if (str == "Escape") return sfKeyEscape;
    else if (str == "LControl") return sfKeyLControl;
    else if (str == "LShift") return sfKeyLShift;
    else if (str == "LAlt") return sfKeyLAlt;
    else if (str == "LSystem") return sfKeyLSystem;
    else if (str == "RControl") return sfKeyRControl;
    else if (str == "RShift") return sfKeyRShift;
    else if (str == "RAlt") return sfKeyRAlt;
    else if (str == "RSystem") return sfKeyRSystem;
    else if (str == "Menu") return sfKeyMenu;
    else if (str == "LBracket") return sfKeyLBracket;
    else if (str == "RBracket") return sfKeyRBracket;
    else if (str == "Semicolon") return sfKeySemicolon;
    else if (str == "Comma") return sfKeyComma;
    else if (str == "Period") return sfKeyPeriod;
    else if (str == "Quote") return sfKeyQuote;
    else if (str == "Slash") return sfKeySlash;
    else if (str == "Backslash") return sfKeyBackslash;
    else if (str == "Tilde") return sfKeyTilde;
    else if (str == "Equal") return sfKeyEqual;
    else if (str == "Hyphen") return sfKeyHyphen;
    else if (str == "Space") return sfKeySpace;
    else if (str == "Enter") return sfKeyEnter;
    else if (str == "Backspace") return sfKeyBackspace;
    else if (str == "Tab") return sfKeyTab;
    else if (str == "PageUp") return sfKeyPageUp;
    else if (str == "PageDown") return sfKeyPageDown;
    else if (str == "End") return sfKeyEnd;
    else if (str == "Home") return sfKeyHome;
    else if (str == "Insert") return sfKeyInsert;
    else if (str == "Delete") return sfKeyDelete;
    else if (str == "Add") return sfKeyAdd;
    else if (str == "Subtract") return sfKeySubtract;
    else if (str == "Multiply") return sfKeyMultiply;
    else if (str == "Divide") return sfKeyDivide;
    else if (str == "Left") return sfKeyLeft;
    else if (str == "Right") return sfKeyRight;
    else if (str == "Up") return sfKeyUp;
    else if (str == "Down") return sfKeyDown;
    else if (str == "Numpad0") return sfKeyNumpad0;
    else if (str == "Numpad1") return sfKeyNumpad1;
    else if (str == "Numpad2") return sfKeyNumpad2;
    else if (str == "Numpad3") return sfKeyNumpad3;
    else if (str == "Numpad4") return sfKeyNumpad4;
    else if (str == "Numpad5") return sfKeyNumpad5;
    else if (str == "Numpad6") return sfKeyNumpad6;
    else if (str == "Numpad7") return sfKeyNumpad7;
    else if (str == "Numpad8") return sfKeyNumpad8;
    else if (str == "F1") return sfKeyF1;
    else if (str == "F2") return sfKeyF2;
    else if (str == "F3") return sfKeyF3;
    else if (str == "F4") return sfKeyF4;
    else if (str == "F5") return sfKeyF5;
    else if (str == "F6") return sfKeyF6;
    else if (str == "F7") return sfKeyF7;
    else if (str == "F8") return sfKeyF8;
    else if (str == "F9") return sfKeyF9;
    else if (str == "F10") return sfKeyF10;
    else if (str == "F11") return sfKeyF11;
    else if (str == "F12") return sfKeyF12;
    else if (str == "F13") return sfKeyF13;
    else if (str == "F14") return sfKeyF14;
    else if (str == "F15") return sfKeyF15;
    return sfKeyUnknown;
}

static void writeDefaultBinds(const std::string &path, const Settings::Binds &binds, float hSens, float vSens)
{
    std::ofstream output(path, std::ios::trunc);
    if (!output.is_open())
        return;
    output << "move_forward=" << keyboardKeyToString(binds.moveForward) << "\n";
    output << "move_left=" << keyboardKeyToString(binds.moveLeft) << "\n";
    output << "move_back=" << keyboardKeyToString(binds.moveBack) << "\n";
    output << "move_right=" << keyboardKeyToString(binds.moveRight) << "\n";
    output << "crouch=" << keyboardKeyToString(binds.crouch) << "\n";
    output << "reload=" << keyboardKeyToString(binds.reload) << "\n";
    output << "shoot=" << mouseButtonToString(binds.shoot) << "\n";
    output << "aim=" << mouseButtonToString(binds.aim) << "\n";
    output << "horizontal_sensitivity=" << hSens << "\n";
    output << "vertical_sensitivity=" << vSens << "\n";
}

Settings::Settings()
{
    std::string path("settings.conf");
    horizontal_sensitivity = 0.0025f;
    vertical_sensitivity = 1.25f;
    bool writeDefaults = false;

    std::ifstream input(path);
    if (!input.is_open()) {
        writeDefaults = true;
    } else {
        input.seekg(0, std::ios::end);
        if (input.tellg() <= 0) {
            writeDefaults = true;
        } else {
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
                else if (key == "reload")
                    binds.reload = keyboardKeyFromString(val);
                else if (key == "shoot") {
                    auto m = mouseButtonFromString(val);
                    if (m != sfMouseButtonCount) binds.shoot = m;
                } else if (key == "aim") {
                    auto m = mouseButtonFromString(val);
                    if (m != sfMouseButtonCount) binds.aim = m;
                } else if (key == "sensitivity") { // Rétrocompatibilité
                    try {
                        horizontal_sensitivity = std::stof(val);
                        vertical_sensitivity = std::stof(val);
                    } catch (...) {}
                } else if (key == "horizontal_sensitivity") {
                    try { horizontal_sensitivity = std::stof(val); } catch (...) { horizontal_sensitivity = 0.0025f; }
                } else if (key == "vertical_sensitivity") {
                    try { vertical_sensitivity = std::stof(val); } catch (...) { vertical_sensitivity = 0.0025f; }
                }
            }
            if (binds.moveForward == sfKeyUnknown || binds.moveLeft == sfKeyUnknown ||
                binds.moveBack == sfKeyUnknown || binds.moveRight == sfKeyUnknown ||
                binds.crouch == sfKeyUnknown || binds.reload == sfKeyUnknown ||
                binds.shoot == sfMouseButtonCount || binds.aim == sfMouseButtonCount) {
                writeDefaults = true;
            }
        }
        input.close();
    }
    if (writeDefaults) {
        writeDefaultBinds(path, binds, horizontal_sensitivity, vertical_sensitivity);
    }
}

Settings::~Settings() {}

void Settings::changeSettings(Window& window, Menu& menu)
{
    sfFont* font = sfFont_createFromFile("assets/fonts/menu.ttf");
    if (!font) return;
    sfRenderWindow* renderWindow = window.getWindow();
    sfText* title = sfText_create();
    sfText_setFont(title, font); sfText_setCharacterSize(title, 30);
    sfText_setString(title, "Settings"); sfText_setPosition(title, (sfVector2f){100, 50}); sfText_setColor(title, sfWhite);
    sfText* textArray[10];
    for (int i = 0; i < 10; ++i) {
        textArray[i] = sfText_create();
        sfText_setFont(textArray[i], font);
        sfText_setCharacterSize(textArray[i], 20);
        sfText_setPosition(textArray[i], (sfVector2f){100, 150.f + (i * 50.f)});
        sfText_setColor(textArray[i], sfWhite);
    }
    sfText* waitingText = sfText_create();
    sfText_setFont(waitingText, font); sfText_setCharacterSize(waitingText, 20);
    sfText_setPosition(waitingText, (sfVector2f){100, 670}); sfText_setColor(waitingText, sfYellow);
    float minHSens = 0.0001f, maxHSens = 0.0100f;
    float minVSens = 0.9f, maxVSens = 2.0f;
    float trackX = 450.f;
    float trackWidth = 200.f;
    sfRectangleShape* hTrack = sfRectangleShape_create();
    sfRectangleShape_setSize(hTrack, (sfVector2f){trackWidth, 6});
    sfRectangleShape_setPosition(hTrack, (sfVector2f){trackX, 560.f});
    sfRectangleShape_setFillColor(hTrack, sfColor_fromRGB(100, 100, 100));
    sfRectangleShape* vTrack = sfRectangleShape_create();
    sfRectangleShape_setSize(vTrack, (sfVector2f){trackWidth, 6});
    sfRectangleShape_setPosition(vTrack, (sfVector2f){trackX, 610.f});
    sfRectangleShape_setFillColor(vTrack, sfColor_fromRGB(100, 100, 100));
    sfRectangleShape* hKnob = sfRectangleShape_create();
    sfRectangleShape_setSize(hKnob, (sfVector2f){10, 20}); sfRectangleShape_setFillColor(hKnob, sfRed); sfRectangleShape_setOrigin(hKnob, (sfVector2f){5, 10});
    sfRectangleShape* vKnob = sfRectangleShape_create();
    sfRectangleShape_setSize(vKnob, (sfVector2f){10, 20}); sfRectangleShape_setFillColor(vKnob, sfRed); sfRectangleShape_setOrigin(vKnob, (sfVector2f){5, 10});
    updateTexts(textArray[0], textArray[1], textArray[2], textArray[3], textArray[4], textArray[5], textArray[6], textArray[7], textArray[8], textArray[9]);
    enum State { Normal, WaitingKey, WaitingMouse };
    State state = Normal;
    int waitingFor = -1;
    int draggingSlider = 0;
    while (sfRenderWindow_isOpen(renderWindow)) {
        sfEvent event;
        float hRatio = std::clamp((horizontal_sensitivity - minHSens) / (maxHSens - minHSens), 0.f, 1.f);
        float vRatio = std::clamp((vertical_sensitivity   - minVSens) / (maxVSens - minVSens), 0.f, 1.f);
        sfRectangleShape_setPosition(hKnob, (sfVector2f){trackX + (hRatio * trackWidth), 563.f});
        sfRectangleShape_setPosition(vKnob, (sfVector2f){trackX + (vRatio * trackWidth), 613.f});

        while (sfRenderWindow_pollEvent(renderWindow, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(renderWindow);
                break;
            }
            if (event.type == sfEvtKeyPressed) {
                if (state == Normal) {
                    if (event.key.code >= sfKeyNum1 && event.key.code <= sfKeyNum5) {
                        state = WaitingKey;
                        waitingFor = event.key.code - sfKeyNum1 + 1;
                        std::string actions[] = {"Move Forward", "Move Left", "Move Back", "Move Right", "Crouch"};
                        sfText_setString(waitingText, ("Press new key for " + actions[waitingFor - 1]).c_str());
                    } else if (event.key.code == sfKeyNum6) {
                        state = WaitingMouse; waitingFor = 6; sfText_setString(waitingText, "Press new mouse button for Shoot");
                    } else if (event.key.code == sfKeyNum7) {
                        state = WaitingMouse; waitingFor = 7; sfText_setString(waitingText, "Press new mouse button for Aim");
                    } else if (event.key.code == sfKeyNum8) {
                        state = WaitingKey; waitingFor = 8; sfText_setString(waitingText, "Press new key for Reload");
                    } else if (event.key.code == sfKeyEscape) {
                        sfText_destroy(title); sfText_destroy(waitingText);
                        for (int i = 0; i < 10; ++i) sfText_destroy(textArray[i]);
                        sfRectangleShape_destroy(hTrack); sfRectangleShape_destroy(hKnob);
                        sfRectangleShape_destroy(vTrack); sfRectangleShape_destroy(vKnob);
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
                        else if (waitingFor == 8) binds.reload = event.key.code;
                        updateTexts(textArray[0], textArray[1], textArray[2], textArray[3], textArray[4], textArray[5], textArray[6], textArray[7], textArray[8], textArray[9]);
                        saveSettings();
                        state = Normal; sfText_setString(waitingText, "");
                    }
                }
            } else if (event.type == sfEvtMouseButtonPressed) {
                if (state == Normal && event.mouseButton.button == sfMouseLeft) {
                    int x = event.mouseButton.x;
                    int y = event.mouseButton.y;
                    if (x >= trackX - 10 && x <= trackX + trackWidth + 10) {
                        if (y >= 545 && y <= 575) draggingSlider = 1;
                        else if (y >= 595 && y <= 625) draggingSlider = 2;
                        if (draggingSlider != 0) {
                            float newRatio = std::clamp((x - trackX) / trackWidth, 0.f, 1.f);
                            if (draggingSlider == 1) horizontal_sensitivity = minHSens + newRatio * (maxHSens - minHSens);
                            else                     vertical_sensitivity   = minVSens + newRatio * (maxVSens - minVSens);
                            updateTexts(textArray[0], textArray[1], textArray[2], textArray[3], textArray[4], textArray[5], textArray[6], textArray[7], textArray[8], textArray[9]);
                        }
                    }
                    if (draggingSlider == 0) {
                        if      (y >= 150 && y < 200) { state = WaitingKey;   waitingFor = 1; sfText_setString(waitingText, "Press new key for Move Forward"); }
                        else if (y >= 200 && y < 250) { state = WaitingKey;   waitingFor = 2; sfText_setString(waitingText, "Press new key for Move Left"); }
                        else if (y >= 250 && y < 300) { state = WaitingKey;   waitingFor = 3; sfText_setString(waitingText, "Press new key for Move Back"); }
                        else if (y >= 300 && y < 350) { state = WaitingKey;   waitingFor = 4; sfText_setString(waitingText, "Press new key for Move Right"); }
                        else if (y >= 350 && y < 400) { state = WaitingKey;   waitingFor = 5; sfText_setString(waitingText, "Press new key for Crouch"); }
                        else if (y >= 400 && y < 450) { state = WaitingMouse; waitingFor = 6; sfText_setString(waitingText, "Press new mouse button for Shoot"); }
                        else if (y >= 450 && y < 500) { state = WaitingMouse; waitingFor = 7; sfText_setString(waitingText, "Press new mouse button for Aim"); }
                        else if (y >= 500 && y < 550) { state = WaitingKey;   waitingFor = 8; sfText_setString(waitingText, "Press new key for Reload"); }
                    }
                } else if (state == WaitingMouse) {
                    if (waitingFor == 6) binds.shoot = event.mouseButton.button;
                    else if (waitingFor == 7) binds.aim = event.mouseButton.button;
                    updateTexts(textArray[0], textArray[1], textArray[2], textArray[3], textArray[4], textArray[5], textArray[6], textArray[7], textArray[8], textArray[9]);
                    saveSettings();
                    state = Normal; sfText_setString(waitingText, "");
                }
            } else if (event.type == sfEvtMouseButtonReleased) {
                if (event.mouseButton.button == sfMouseLeft && draggingSlider != 0) {
                    draggingSlider = 0;
                    saveSettings();
                }
            } else if (event.type == sfEvtMouseMoved) {
                if (draggingSlider != 0) {
                    float newRatio = std::clamp((event.mouseMove.x - trackX) / trackWidth, 0.f, 1.f);
                    if (draggingSlider == 1) horizontal_sensitivity = minHSens + newRatio * (maxHSens - minHSens);
                    else                     vertical_sensitivity   = minVSens + newRatio * (maxVSens - minVSens);
                    updateTexts(textArray[0], textArray[1], textArray[2], textArray[3], textArray[4], textArray[5], textArray[6], textArray[7], textArray[8], textArray[9]);
                }
            }
        }
        if (!sfRenderWindow_isOpen(renderWindow)) break;
        sfRenderWindow_clear(renderWindow, sfBlack);
        menu.display(renderWindow, 2);
        sfRenderWindow_drawText(renderWindow, title, NULL);
        for (int i = 0; i < 10; ++i)
            sfRenderWindow_drawText(renderWindow, textArray[i], NULL);
        sfRenderWindow_drawRectangleShape(renderWindow, hTrack, NULL);
        sfRenderWindow_drawRectangleShape(renderWindow, hKnob, NULL);
        sfRenderWindow_drawRectangleShape(renderWindow, vTrack, NULL);
        sfRenderWindow_drawRectangleShape(renderWindow, vKnob, NULL);
        if (state != Normal)
            sfRenderWindow_drawText(renderWindow, waitingText, NULL);
        sfRenderWindow_display(renderWindow);
    }
    sfText_destroy(title); sfText_destroy(waitingText);
    for (int i = 0; i < 10; ++i) sfText_destroy(textArray[i]);
    sfRectangleShape_destroy(hTrack); sfRectangleShape_destroy(hKnob);
    sfRectangleShape_destroy(vTrack); sfRectangleShape_destroy(vKnob);
    sfFont_destroy(font);
}

void Settings::updateTexts(sfText* mf, sfText* ml, sfText* mb, sfText* mr, sfText* c, sfText* s, sfText* a, sfText* r, sfText* hSens, sfText* vSens)
{
    sfText_setString(mf, ("1. Move Forward: " + keyboardKeyToString(binds.moveForward)).c_str());
    sfText_setString(ml, ("2. Move Left: "    + keyboardKeyToString(binds.moveLeft)).c_str());
    sfText_setString(mb, ("3. Move Back: "    + keyboardKeyToString(binds.moveBack)).c_str());
    sfText_setString(mr, ("4. Move Right: "   + keyboardKeyToString(binds.moveRight)).c_str());
    sfText_setString(c,  ("5. Crouch: "       + keyboardKeyToString(binds.crouch)).c_str());
    sfText_setString(s,  ("6. Shoot: "        + mouseButtonToString(binds.shoot)).c_str());
    sfText_setString(a,  ("7. Aim: "          + mouseButtonToString(binds.aim)).c_str());
    sfText_setString(r,  ("8. Reload: "       + keyboardKeyToString(binds.reload)).c_str());
    float minHSens = 0.0001f, maxHSens = 0.0100f;
    float minVSens = 0.9f,    maxVSens = 2.0f;
    int hDisplay = std::clamp((int)(((horizontal_sensitivity - minHSens) / (maxHSens - minHSens)) * 100.f), 0, 100);
    int vDisplay = std::clamp((int)(((vertical_sensitivity   - minVSens) / (maxVSens - minVSens)) * 100.f), 0, 100);
    sfText_setString(hSens, ("9. Horizontal Sens: "  + std::to_string(hDisplay)).c_str());
    sfText_setString(vSens, ("10. Vertical Sens: " + std::to_string(vDisplay)).c_str());
}

void Settings::saveSettings()
{
    std::string path("settings.conf");
    writeDefaultBinds(path, binds, horizontal_sensitivity, vertical_sensitivity);
}