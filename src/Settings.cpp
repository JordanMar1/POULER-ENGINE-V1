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
    if (str == "A") return sfKeyA; else if (str == "B") return sfKeyB; else if (str == "C") return sfKeyC;
    else if (str == "D") return sfKeyD; else if (str == "E") return sfKeyE; else if (str == "F") return sfKeyF;
    else if (str == "G") return sfKeyG; else if (str == "H") return sfKeyH; else if (str == "I") return sfKeyI;
    else if (str == "J") return sfKeyJ; else if (str == "K") return sfKeyK; else if (str == "L") return sfKeyL;
    else if (str == "M") return sfKeyM; else if (str == "N") return sfKeyN; else if (str == "O") return sfKeyO;
    else if (str == "P") return sfKeyP; else if (str == "Q") return sfKeyQ; else if (str == "R") return sfKeyR;
    else if (str == "S") return sfKeyS; else if (str == "T") return sfKeyT; else if (str == "U") return sfKeyU;
    else if (str == "V") return sfKeyV; else if (str == "W") return sfKeyW; else if (str == "X") return sfKeyX;
    else if (str == "Y") return sfKeyY; else if (str == "Z") return sfKeyZ;
    else if (str == "Num0") return sfKeyNum0; else if (str == "Num1") return sfKeyNum1;
    else if (str == "Num2") return sfKeyNum2; else if (str == "Num3") return sfKeyNum3;
    else if (str == "Num4") return sfKeyNum4; else if (str == "Num5") return sfKeyNum5;
    else if (str == "Num6") return sfKeyNum6; else if (str == "Num7") return sfKeyNum7;
    else if (str == "Num8") return sfKeyNum8; else if (str == "Num9") return sfKeyNum9;
    else if (str == "Escape") return sfKeyEscape; else if (str == "LControl") return sfKeyLControl;
    else if (str == "LShift") return sfKeyLShift; else if (str == "LAlt") return sfKeyLAlt;
    else if (str == "LSystem") return sfKeyLSystem; else if (str == "RControl") return sfKeyRControl;
    else if (str == "RShift") return sfKeyRShift; else if (str == "RAlt") return sfKeyRAlt;
    else if (str == "RSystem") return sfKeyRSystem; else if (str == "Menu") return sfKeyMenu;
    else if (str == "LBracket") return sfKeyLBracket; else if (str == "RBracket") return sfKeyRBracket;
    else if (str == "Semicolon") return sfKeySemicolon; else if (str == "Comma") return sfKeyComma;
    else if (str == "Period") return sfKeyPeriod; else if (str == "Quote") return sfKeyQuote;
    else if (str == "Slash") return sfKeySlash; else if (str == "Backslash") return sfKeyBackslash;
    else if (str == "Tilde") return sfKeyTilde; else if (str == "Equal") return sfKeyEqual;
    else if (str == "Hyphen") return sfKeyHyphen; else if (str == "Space") return sfKeySpace;
    else if (str == "Enter") return sfKeyEnter; else if (str == "Backspace") return sfKeyBackspace;
    else if (str == "Tab") return sfKeyTab; else if (str == "PageUp") return sfKeyPageUp;
    else if (str == "PageDown") return sfKeyPageDown; else if (str == "End") return sfKeyEnd;
    else if (str == "Home") return sfKeyHome; else if (str == "Insert") return sfKeyInsert;
    else if (str == "Delete") return sfKeyDelete; else if (str == "Add") return sfKeyAdd;
    else if (str == "Subtract") return sfKeySubtract; else if (str == "Multiply") return sfKeyMultiply;
    else if (str == "Divide") return sfKeyDivide; else if (str == "Left") return sfKeyLeft;
    else if (str == "Right") return sfKeyRight; else if (str == "Up") return sfKeyUp;
    else if (str == "Down") return sfKeyDown; else if (str == "Numpad0") return sfKeyNumpad0;
    else if (str == "Numpad1") return sfKeyNumpad1; else if (str == "Numpad2") return sfKeyNumpad2;
    else if (str == "Numpad3") return sfKeyNumpad3; else if (str == "Numpad4") return sfKeyNumpad4;
    else if (str == "Numpad5") return sfKeyNumpad5; else if (str == "Numpad6") return sfKeyNumpad6;
    else if (str == "Numpad7") return sfKeyNumpad7; else if (str == "Numpad8") return sfKeyNumpad8;
    else if (str == "F1") return sfKeyF1; else if (str == "F2") return sfKeyF2; else if (str == "F3") return sfKeyF3;
    else if (str == "F4") return sfKeyF4; else if (str == "F5") return sfKeyF5; else if (str == "F6") return sfKeyF6;
    else if (str == "F7") return sfKeyF7; else if (str == "F8") return sfKeyF8; else if (str == "F9") return sfKeyF9;
    else if (str == "F10") return sfKeyF10; else if (str == "F11") return sfKeyF11; else if (str == "F12") return sfKeyF12;
    else if (str == "F13") return sfKeyF13; else if (str == "F14") return sfKeyF14; else if (str == "F15") return sfKeyF15;
    return sfKeyUnknown;
}

static void writeDefaultBinds(const std::string &path, const Settings::Binds &binds, float hSens, float vSens, int fps, int windowWidth, int windowHeight, bool fullscreen, int sVol, int mVol)
{
    std::ofstream output(path, std::ios::trunc);
    if (!output.is_open())
        return;
    output << "move_forward=" << keyboardKeyToString(binds.moveForward) << "\n";
    output << "move_left=" << keyboardKeyToString(binds.moveLeft) << "\n";
    output << "move_back=" << keyboardKeyToString(binds.moveBack) << "\n";
    output << "move_right=" << keyboardKeyToString(binds.moveRight) << "\n";
    output << "crouch=" << keyboardKeyToString(binds.crouch) << "\n";
    output << "lean_left=" << keyboardKeyToString(binds.leanLeft) << "\n";
    output << "lean_right=" << keyboardKeyToString(binds.leanRight) << "\n";
    output << "reload=" << keyboardKeyToString(binds.reload) << "\n";
    output << "shoot=" << mouseButtonToString(binds.shoot) << "\n";
    output << "aim=" << mouseButtonToString(binds.aim) << "\n";
    output << "horizontal_sensitivity=" << hSens << "\n";
    output << "vertical_sensitivity=" << vSens << "\n";
    output << "fps=" << fps << "\n";
    output << "window_width="  << windowWidth  << "\n";
    output << "window_height=" << windowHeight << "\n";
    output << "fullscreen=" << (fullscreen ? "1" : "0") << "\n";
    output << "sound_volume=" << sVol << "\n";
    output << "music_volume=" << mVol << "\n";
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
                else if (key == "lean_left")
                    binds.leanLeft = keyboardKeyFromString(val);
                else if (key == "lean_right")
                    binds.leanRight = keyboardKeyFromString(val);
                else if (key == "reload")
                    binds.reload = keyboardKeyFromString(val);
                else if (key == "shoot") {
                    auto m = mouseButtonFromString(val);
                    if (m != sfMouseButtonCount) binds.shoot = m;
                } else if (key == "aim") {
                    auto m = mouseButtonFromString(val);
                    if (m != sfMouseButtonCount) binds.aim = m;
                } else if (key == "sensitivity") {
                    try {
                        horizontal_sensitivity = std::stof(val);
                        vertical_sensitivity = std::stof(val);
                    } catch (...) {}
                } else if (key == "horizontal_sensitivity") {
                    try { horizontal_sensitivity = std::stof(val); } catch (...) { horizontal_sensitivity = 0.0025f; }
                } else if (key == "vertical_sensitivity") {
                    try { vertical_sensitivity = std::stof(val); } catch (...) { vertical_sensitivity = 1.25f; }
                } else if (key == "fps") {
                    try { fps = std::stoi(val); } catch (...) { fps = 60; }
                } else if (key == "window_width") {
                    try { windowWidth  = std::stoul(val); } catch (...) { windowWidth  = 1920; }
                } else if (key == "window_height") {
                    try { windowHeight = std::stoul(val); } catch (...) { windowHeight = 1080; }
                } else if (key == "fullscreen") {
                    fullscreen = (val == "1" || val == "true");
                } else if (key == "sound_volume") {
                    try { soundVolume = std::stoi(val); } catch (...) { soundVolume = 100; }
                } else if (key == "music_volume") {
                    try { musicVolume = std::stoi(val); } catch (...) { musicVolume = 100; }
                }
            }
            if (binds.moveForward == sfKeyUnknown || binds.moveLeft == sfKeyUnknown ||
                binds.moveBack == sfKeyUnknown || binds.moveRight == sfKeyUnknown ||
                binds.crouch == sfKeyUnknown || binds.leanLeft == sfKeyUnknown || 
                binds.leanRight == sfKeyUnknown || binds.reload == sfKeyUnknown ||
                binds.shoot == sfMouseButtonCount || binds.aim == sfMouseButtonCount) {
                writeDefaults = true;
            }
        }
        input.close();
    }
    if (writeDefaults) {
        writeDefaultBinds(path, binds, horizontal_sensitivity, vertical_sensitivity, fps, windowWidth, windowHeight, fullscreen, soundVolume, musicVolume);
    }
}

Settings::~Settings() {}

static void recreateWindow(sfRenderWindow *&rw, Window &win,
                           unsigned int w, unsigned int h, bool fs)
{
    sfVideoMode vm = {w, h, 32};
    sfRenderWindow_destroy(rw);
    rw = sfRenderWindow_create(vm, "pouler_or_not",
                               fs ? sfFullscreen : sfDefaultStyle, NULL);
    win.setWindow(rw);
}

void Settings::handleFpsClick(sfRenderWindow *&rw)
{
    int fpsOptions[] = {30, 60, 120, 144, 240, 0};
    bool found = false;
    for (int i = 0; i < 6; i++) {
        if (fps == fpsOptions[i]) {
            fps = fpsOptions[(i + 1) % 6];
            found = true;
            break;
        }
    }
    if (!found) fps = 60;
    sfRenderWindow_setFramerateLimit(rw, fps);
    saveSettings();
}

void Settings::handleResolutionClick(sfRenderWindow *&rw, Window &win)
{
    struct Res { unsigned int w, h; };
    Res resOptions[] = {{1280,720},{1600,900},{1920,1080},{2560,1440},{3840,2160}};
    int count = 5;
    bool found = false;
    for (int i = 0; i < count; i++) {
        if ((unsigned int)windowWidth  == resOptions[i].w &&
            (unsigned int)windowHeight == resOptions[i].h) {
            windowWidth  = resOptions[(i + 1) % count].w;
            windowHeight = resOptions[(i + 1) % count].h;
            found = true;
            break;
        }
    }
    if (!found) { windowWidth = 1920; windowHeight = 1080; }
    recreateWindow(rw, win, windowWidth, windowHeight, fullscreen);
    saveSettings();
}

void Settings::handleFullscreenClick(sfRenderWindow *&rw, Window &win)
{
    fullscreen = !fullscreen;
    recreateWindow(rw, win, windowWidth, windowHeight, fullscreen);
    saveSettings();
}

void Settings::handleMouseClick(int x, int y, sfRenderWindow *&rw, Window &win,
                                sfText *waitingText, int &draggingSlider,
                                float trackX, float trackWidth,
                                float hSliderY, float vSliderY,
                                float sSliderY, float mSliderY,
                                float minHSens, float maxHSens,
                                float minVSens, float maxVSens,
                                State &state, int &waitingFor,
                                sfText **textArray)
{
    if (x >= trackX - 10 && x <= trackX + trackWidth + 10) {
        if (y >= hSliderY - 15 && y <= hSliderY + 15)      draggingSlider = 1;
        else if (y >= vSliderY - 15 && y <= vSliderY + 15) draggingSlider = 2;
        else if (y >= sSliderY - 15 && y <= sSliderY + 15) draggingSlider = 3;
        else if (y >= mSliderY - 15 && y <= mSliderY + 15) draggingSlider = 4;
        
        if (draggingSlider != 0) {
            float newRatio = std::clamp((x - trackX) / trackWidth, 0.f, 1.f);
            if (draggingSlider == 1) horizontal_sensitivity = minHSens + newRatio * (maxHSens - minHSens);
            else if (draggingSlider == 2) vertical_sensitivity   = minVSens + newRatio * (maxVSens - minVSens);
            else if (draggingSlider == 3) soundVolume = (int)(newRatio * 100.f);
            else if (draggingSlider == 4) musicVolume = (int)(newRatio * 100.f);
            updateTexts(textArray);
            return;
        }
    }
    auto setWaitKey   = [&](int id, const char *msg){ state = WaitingKey;   waitingFor = id; sfText_setString(waitingText, msg); };
    auto setWaitMouse = [&](int id, const char *msg){ state = WaitingMouse; waitingFor = id; sfText_setString(waitingText, msg); };
    if      (y >= 120 && y < 165) setWaitKey  (1,  "Press new key for Move Forward");
    else if (y >= 165 && y < 210) setWaitKey  (2,  "Press new key for Move Left");
    else if (y >= 210 && y < 255) setWaitKey  (3,  "Press new key for Move Back");
    else if (y >= 255 && y < 300) setWaitKey  (4,  "Press new key for Move Right");
    else if (y >= 300 && y < 345) setWaitKey  (5,  "Press new key for Crouch");
    else if (y >= 345 && y < 390) setWaitKey  (6,  "Press new key for Lean Left");
    else if (y >= 390 && y < 435) setWaitKey  (7,  "Press new key for Lean Right");
    else if (y >= 435 && y < 480) setWaitMouse(8,  "Press new mouse button for Shoot");
    else if (y >= 480 && y < 525) setWaitMouse(9,  "Press new mouse button for Aim");
    else if (y >= 525 && y < 570) setWaitKey  (10, "Press new key for Reload");
    else if (y >= 660 && y < 705) { handleFpsClick(rw);              updateTexts(textArray); }
    else if (y >= 705 && y < 750) { handleResolutionClick(rw, win);  updateTexts(textArray); }
    else if (y >= 750 && y < 795) { handleFullscreenClick(rw, win);  updateTexts(textArray); }
}

void Settings::handleKeyPress(sfKeyCode code, State &state, int &waitingFor,
                              sfText *waitingText, sfText **textArray,
                              sfText *title, 
                              sfRectangleShape *hTrack, sfRectangleShape *hKnob, 
                              sfRectangleShape *vTrack, sfRectangleShape *vKnob, 
                              sfRectangleShape *sTrack, sfRectangleShape *sKnob, 
                              sfRectangleShape *mTrack, sfRectangleShape *mKnob, 
                              sfFont *font, bool &shouldReturn)
{
    if (state == Normal) {
        if (code >= sfKeyNum1 && code <= sfKeyNum5) {
            state = WaitingKey;
            waitingFor = code - sfKeyNum1 + 1;
            std::string actions[] = {"Move Forward","Move Left","Move Back","Move Right","Crouch"};
            sfText_setString(waitingText, ("Press new key for " + actions[waitingFor - 1]).c_str());
        } else if (code == sfKeyNum6) { state = WaitingKey;   waitingFor = 6;  sfText_setString(waitingText, "Press new key for Lean Left"); }
        else if (code == sfKeyNum7)   { state = WaitingKey;   waitingFor = 7;  sfText_setString(waitingText, "Press new key for Lean Right"); }
        else if (code == sfKeyNum8)   { state = WaitingMouse; waitingFor = 8;  sfText_setString(waitingText, "Press new mouse button for Shoot"); }
        else if (code == sfKeyNum9)   { state = WaitingMouse; waitingFor = 9;  sfText_setString(waitingText, "Press new mouse button for Aim"); }
        else if (code == sfKeyEscape) {
            sfText_destroy(title); sfText_destroy(waitingText);
            for (int i = 0; i < 17; ++i) sfText_destroy(textArray[i]);
            sfRectangleShape_destroy(hTrack); sfRectangleShape_destroy(hKnob);
            sfRectangleShape_destroy(vTrack); sfRectangleShape_destroy(vKnob);
            sfRectangleShape_destroy(sTrack); sfRectangleShape_destroy(sKnob);
            sfRectangleShape_destroy(mTrack); sfRectangleShape_destroy(mKnob);
            sfFont_destroy(font);
            shouldReturn = true;
        }
    } else if (state == WaitingKey && code != sfKeyUnknown) {
        if      (waitingFor == 1)  binds.moveForward = code;
        else if (waitingFor == 2)  binds.moveLeft    = code;
        else if (waitingFor == 3)  binds.moveBack    = code;
        else if (waitingFor == 4)  binds.moveRight   = code;
        else if (waitingFor == 5)  binds.crouch      = code;
        else if (waitingFor == 6)  binds.leanLeft    = code;
        else if (waitingFor == 7)  binds.leanRight   = code;
        else if (waitingFor == 10) binds.reload      = code;
        updateTexts(textArray);
        saveSettings();
        state = Normal; sfText_setString(waitingText, "");
    }
}

void Settings::changeSettings(Window& window, Menu& menu)
{
    sfFont* font = sfFont_createFromFile("assets/fonts/menu.ttf");
    if (!font) return;
    sfRenderWindow* rw = window.getWindow();
    sfText* title = sfText_create();
    sfText_setFont(title, font); sfText_setCharacterSize(title, 30);
    sfText_setString(title, "Settings");
    sfText_setPosition(title, (sfVector2f){100, 50});
    sfText_setColor(title, sfWhite);
    sfText* textArray[17];
    for (int i = 0; i < 17; ++i) {
        textArray[i] = sfText_create();
        sfText_setFont(textArray[i], font);
        sfText_setCharacterSize(textArray[i], 20);
        sfText_setPosition(textArray[i], (sfVector2f){100, 120.f + (i * 45.f)});
    }
    sfText* waitingText = sfText_create();
    sfText_setFont(waitingText, font); sfText_setCharacterSize(waitingText, 20);
    sfText_setPosition(waitingText, (sfVector2f){100, 900});
    sfText_setColor(waitingText, sfYellow);
    float minHSens = 0.0001f, maxHSens = 0.0100f;
    float minVSens = 0.9f,    maxVSens = 2.0f;
    float trackX = 450.f, trackWidth = 200.f;
    float hSliderY = 120.f + (10 * 45.f) + 13.f;
    float vSliderY = 120.f + (11 * 45.f) + 13.f;
    float sSliderY = 120.f + (15 * 45.f) + 13.f;
    float mSliderY = 120.f + (16 * 45.f) + 13.f;
    
    auto createTrack = [&](float y) {
        sfRectangleShape* t = sfRectangleShape_create();
        sfRectangleShape_setSize(t, (sfVector2f){trackWidth, 6});
        sfRectangleShape_setPosition(t, (sfVector2f){trackX, y});
        sfRectangleShape_setFillColor(t, sfColor_fromRGB(100, 100, 100));
        return t;
    };
    auto createKnob = [&]() {
        sfRectangleShape* k = sfRectangleShape_create();
        sfRectangleShape_setSize(k, (sfVector2f){10, 20});
        sfRectangleShape_setFillColor(k, sfRed);
        sfRectangleShape_setOrigin(k, (sfVector2f){5, 10});
        return k;
    };
    sfRectangleShape* hTrack = createTrack(hSliderY);
    sfRectangleShape* vTrack = createTrack(vSliderY);
    sfRectangleShape* sTrack = createTrack(sSliderY);
    sfRectangleShape* mTrack = createTrack(mSliderY);
    sfRectangleShape* hKnob = createKnob();
    sfRectangleShape* vKnob = createKnob();
    sfRectangleShape* sKnob = createKnob();
    sfRectangleShape* mKnob = createKnob();
    updateTexts(textArray);
    State state = Normal;
    int waitingFor = -1;
    int draggingSlider = 0;
    while (sfRenderWindow_isOpen(rw)) {
        float hRatio = std::clamp((horizontal_sensitivity - minHSens) / (maxHSens - minHSens), 0.f, 1.f);
        float vRatio = std::clamp((vertical_sensitivity   - minVSens) / (maxVSens - minVSens), 0.f, 1.f);
        float sRatio = std::clamp(soundVolume / 100.f, 0.f, 1.f);
        float mRatio = std::clamp(musicVolume / 100.f, 0.f, 1.f);
        sfRectangleShape_setPosition(hKnob, (sfVector2f){trackX + hRatio * trackWidth, hSliderY});
        sfRectangleShape_setPosition(vKnob, (sfVector2f){trackX + vRatio * trackWidth, vSliderY});
        sfRectangleShape_setPosition(sKnob, (sfVector2f){trackX + sRatio * trackWidth, sSliderY});
        sfRectangleShape_setPosition(mKnob, (sfVector2f){trackX + mRatio * trackWidth, mSliderY});
        
        sfEvent event;
        while (sfRenderWindow_pollEvent(rw, &event)) {
            if (event.type == sfEvtClosed) { sfRenderWindow_close(rw); break; }

            if (event.type == sfEvtKeyPressed) {
                bool shouldReturn = false;
                handleKeyPress(event.key.code, state, waitingFor, waitingText, textArray, title, 
                               hTrack, hKnob, vTrack, vKnob, sTrack, sKnob, mTrack, mKnob, font, shouldReturn);
                if (shouldReturn) return;
            }
            else if (event.type == sfEvtMouseButtonPressed
                     && event.mouseButton.button == sfMouseLeft) {
                if (state == Normal) {
                    handleMouseClick(event.mouseButton.x, event.mouseButton.y,
                                     rw, window, waitingText, draggingSlider,
                                     trackX, trackWidth, hSliderY, vSliderY, sSliderY, mSliderY,
                                     minHSens, maxHSens, minVSens, maxVSens,
                                     state, waitingFor, textArray);
                } else if (state == WaitingMouse) {
                    if (waitingFor == 8) binds.shoot = event.mouseButton.button;
                    else if (waitingFor == 9) binds.aim = event.mouseButton.button;
                    updateTexts(textArray);
                    saveSettings();
                    state = Normal; sfText_setString(waitingText, "");
                }
            }
            else if (event.type == sfEvtMouseButtonReleased
                     && event.mouseButton.button == sfMouseLeft
                     && draggingSlider != 0) {
                draggingSlider = 0;
                saveSettings();
            }
            else if (event.type == sfEvtMouseMoved && draggingSlider != 0) {
                float newRatio = std::clamp((event.mouseMove.x - trackX) / trackWidth, 0.f, 1.f);
                if (draggingSlider == 1) horizontal_sensitivity = minHSens + newRatio * (maxHSens - minHSens);
                else if (draggingSlider == 2) vertical_sensitivity   = minVSens + newRatio * (maxVSens - minVSens);
                else if (draggingSlider == 3) soundVolume = (int)(newRatio * 100.f);
                else if (draggingSlider == 4) musicVolume = (int)(newRatio * 100.f);
                updateTexts(textArray);
            }
        }
        if (!sfRenderWindow_isOpen(rw)) break;
        sfRenderWindow_clear(rw, sfBlack);
        menu.display(rw, 2);
        sfRenderWindow_drawText(rw, title, NULL);
        for (int i = 0; i < 17; ++i)
            sfRenderWindow_drawText(rw, textArray[i], NULL);    
        sfRenderWindow_drawRectangleShape(rw, hTrack, NULL); sfRenderWindow_drawRectangleShape(rw, hKnob,  NULL);
        sfRenderWindow_drawRectangleShape(rw, vTrack, NULL); sfRenderWindow_drawRectangleShape(rw, vKnob,  NULL);
        sfRenderWindow_drawRectangleShape(rw, sTrack, NULL); sfRenderWindow_drawRectangleShape(rw, sKnob,  NULL);
        sfRenderWindow_drawRectangleShape(rw, mTrack, NULL); sfRenderWindow_drawRectangleShape(rw, mKnob,  NULL);
        if (state != Normal)
            sfRenderWindow_drawText(rw, waitingText, NULL);
        sfRenderWindow_display(rw);
    }
    sfText_destroy(title); sfText_destroy(waitingText);
    for (int i = 0; i < 17; ++i) sfText_destroy(textArray[i]);
    sfRectangleShape_destroy(hTrack); sfRectangleShape_destroy(hKnob);
    sfRectangleShape_destroy(vTrack); sfRectangleShape_destroy(vKnob);
    sfRectangleShape_destroy(sTrack); sfRectangleShape_destroy(sKnob);
    sfRectangleShape_destroy(mTrack); sfRectangleShape_destroy(mKnob);
    sfFont_destroy(font);
}

void Settings::updateTexts(sfText** textArray)
{
    sfText_setString(textArray[0],  ("1. Move Forward: " + keyboardKeyToString(binds.moveForward)).c_str());
    sfText_setString(textArray[1],  ("2. Move Left: "    + keyboardKeyToString(binds.moveLeft)).c_str());
    sfText_setString(textArray[2],  ("3. Move Back: "    + keyboardKeyToString(binds.moveBack)).c_str());
    sfText_setString(textArray[3],  ("4. Move Right: "   + keyboardKeyToString(binds.moveRight)).c_str());
    sfText_setString(textArray[4],  ("5. Crouch: "       + keyboardKeyToString(binds.crouch)).c_str());
    sfText_setString(textArray[5],  ("6. Lean Left: "    + keyboardKeyToString(binds.leanLeft)).c_str());
    sfText_setString(textArray[6],  ("7. Lean Right: "   + keyboardKeyToString(binds.leanRight)).c_str());
    sfText_setString(textArray[7],  ("8. Shoot: "        + mouseButtonToString(binds.shoot)).c_str());
    sfText_setString(textArray[8],  ("9. Aim: "          + mouseButtonToString(binds.aim)).c_str());
    sfText_setString(textArray[9],  ("10. Reload: "      + keyboardKeyToString(binds.reload)).c_str());
    float minHSens = 0.0001f, maxHSens = 0.0100f;
    float minVSens = 0.9f,    maxVSens = 2.0f;
    int hDisplay = std::clamp((int)(((horizontal_sensitivity - minHSens) / (maxHSens - minHSens)) * 100.f), 0, 100);
    int vDisplay = std::clamp((int)(((vertical_sensitivity   - minVSens) / (maxVSens - minVSens)) * 100.f), 0, 100);
    sfText_setString(textArray[10], ("11. Horizontal Sens: " + std::to_string(hDisplay)).c_str());
    sfText_setString(textArray[11], ("12. Vertical Sens: "   + std::to_string(vDisplay)).c_str());
    std::string fpsStr = (fps == 0) ? "Unlimited" : std::to_string(fps);
    sfText_setString(textArray[12], ("13. FPS Limit: " + fpsStr + "  [click to change]").c_str());
    sfText_setString(textArray[13], ("14. Resolution: " + std::to_string(windowWidth) + "x" + std::to_string(windowHeight) + "  [click to change]").c_str());
    sfText_setString(textArray[14], ("15. Fullscreen: " + std::string(fullscreen ? "On" : "Off") + "  [click to toggle]").c_str());
    sfText_setString(textArray[15], ("16. Sound Volume: " + std::to_string(soundVolume) + "%").c_str());
    sfText_setString(textArray[16], ("17. Music Volume: " + std::to_string(musicVolume) + "%").c_str());
}

void Settings::saveSettings()
{
    std::string path("settings.conf");
    writeDefaultBinds(path, binds, horizontal_sensitivity, vertical_sensitivity, fps, windowWidth, windowHeight, fullscreen, soundVolume, musicVolume);
}