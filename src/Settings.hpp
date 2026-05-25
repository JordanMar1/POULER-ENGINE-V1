/*
** EPITECH PROJECT, 2026
** MY_GZDOOM
** File description:
** Settings
*/

#pragma once

#include <SFML/Window.h>
#include <SFML/Graphics.h>
#include <string>

class Window;
class Menu;

class Settings {
    public:
        struct Binds {
            sfKeyCode moveForward = sfKeyZ;
            sfKeyCode moveLeft = sfKeyQ;
            sfKeyCode moveBack = sfKeyS;
            sfKeyCode moveRight = sfKeyD;
            sfKeyCode crouch = sfKeyLControl;
            sfKeyCode reload = sfKeyR;
            sfKeyCode leanLeft = sfKeyA;
            sfKeyCode leanRight = sfKeyE;
            sfMouseButton shoot = sfMouseLeft;
            sfMouseButton aim = sfMouseRight;
        } binds;
        float horizontal_sensitivity;
        float vertical_sensitivity;
        int fps = 60;
        int Pfov = 90;
        unsigned int windowWidth  = 1920;
        unsigned int windowHeight = 1080;
        bool fullscreen = false;
        int soundVolume = 100;
        int musicVolume = 100;

        Settings();
        ~Settings();
        void changeSettings(Window& window, Menu& menu);
        
    private:
        enum State { Normal, WaitingKey, WaitingMouse };
        void handleFpsClick(sfRenderWindow *&rw);
        void handleResolutionClick(sfRenderWindow *&rw, Window &win);
        void handleFullscreenClick(sfRenderWindow *&rw, Window &win);
        void handleMouseClick(int x, int y, sfRenderWindow *&rw, Window &win,
                            sfText *waitingText, int &draggingSlider,
                            float trackX, float trackWidth,
                            float hSliderY, float vSliderY,
                            float sSliderY, float mSliderY, float fSliderY,
                            float minFSens, float maxFSens,
                            float minHSens, float maxHSens,
                            float minVSens, float maxVSens,
                            State &state, int &waitingFor, sfText **textArray);
        void handleKeyPress(sfKeyCode code, State &state, int &waitingFor,
                            sfText *waitingText, sfText **textArray,
                            sfText *title, 
                            sfRectangleShape *hTrack, sfRectangleShape *hKnob, 
                            sfRectangleShape *vTrack, sfRectangleShape *vKnob, 
                            sfRectangleShape *sTrack, sfRectangleShape *sKnob, 
                            sfRectangleShape *mTrack, sfRectangleShape *mKnob, 
                            sfFont *font, bool &shouldReturn);
        void updateTexts(sfText** textArray);
        void saveSettings();
};