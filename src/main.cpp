/*
** EPITECH PROJECT, 2026
** POULER OR NOT
** File description:
** main
*/

#include "ErrorHandler.hpp"
#include "GameData.hpp"
#include "Window.hpp"
#include "Menu.hpp"
#include "Core.hpp"
#include "Weapons.hpp"

int main(int argc, char *argv[])
{
    ErrorHandler errorHandler(argc, argv);
    GameData gameData = GameData(errorHandler.getFilepath());
    if (gameData.is_err) {
        std::cerr << "Error: Failed to load game data from " << errorHandler.getFilepath() << std::endl;
        return 84;
    }
    Window window((char *)"POULER OR NOT", errorHandler.isDebug());
    Menu menu(gameData.game_array, errorHandler.isDebug());
    std::vector<Weapons *> weapons = create_weapons(gameData.game_array, errorHandler.isDebug());
    Settings settings = Settings();
    menu.setMusicVolume(settings.musicVolume);
    Core core(&window, &menu, &gameData, &settings, weapons);
    sfRenderWindow_setFramerateLimit(window.getWindow(), settings.fps);
    
    while (sfRenderWindow_isOpen(window.getWindow()))
    {
        sfEvent event;
        int buttonId = 0;
        while (sfRenderWindow_pollEvent(window.getWindow(), &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window.getWindow());
            sfVector2i mousePos = sfMouse_getPositionRenderWindow(window.getWindow());
            if (event.type == sfEvtMouseButtonPressed) {
                buttonId = menu.handleClick(mousePos.x, mousePos.y);
                if (buttonId == 1) {
                    sfMusic_stop(menu.getMusic());
                    if (errorHandler.isDebug())
                        std::cout << "Starting game..." << std::endl;
                    return core.chooseLevel();
                } else if (buttonId == 2) {
                    sfMusic_stop(menu.getMusic());
                    if (errorHandler.isDebug())
                        std::cout << "Opening options..." << std::endl;
                    settings.changeSettings(window, menu);
                    menu.setMusicVolume(settings.musicVolume);
                    sfMusic_play(menu.getMusic());
                    buttonId = 0;
                } else if (buttonId == 3) {
                    if (errorHandler.isDebug())
                        std::cout << "Quitting..." << std::endl;
                    sfRenderWindow_close(window.getWindow());
                }
            }
            menu.handleHover(mousePos.x, mousePos.y);
        }
        sfRenderWindow_clear(window.getWindow(), sfBlack);
        menu.display(window.getWindow(), buttonId);
        sfRenderWindow_display(window.getWindow());
    }
    return 0;
}
