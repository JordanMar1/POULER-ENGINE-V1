/*
** EPITECH PROJECT, 2026
** POULER OR NOT
** File description:
** ErrorHandler
*/

#include "ErrorHandler.hpp"
#include <cstring>

ErrorHandler::ErrorHandler(int argc, char **argv)
{
    filepath = (char *)"maps/example.wac";
    debug = false;
    if (argc > 3) {
        std::cout << "error: arguments parsing error (skill issue)" << std::endl;
        exit(84);
    }
    if (argc == 2) {
        std::string arg = argv[1];
        if (arg == "-d" || arg == "--debug")
            debug = true;
        else if (arg == "-h" || arg == "--help") {
            std::cout << "Usage: " << argv[0] << " [options]\n\nOptions:\n  -d, --debug   Enable debug mode\n  -h, --help    Show this help message\n";
            exit(0);
        } else {
            filepath = argv[1];
        }
    }
    if (argc == 3) {
        char *arg1 = argv[1];
        char *arg2 = argv[2];
        if (strcmp(arg1, "-d") == 0 || strcmp(arg1, "--debug") == 0) {
            debug = true;
            filepath = arg2;
        }
        else if (strcmp(arg2, "-d") == 0 || strcmp(arg2, "--debug") == 0) {
            debug = true;
            filepath = arg1;
        }
        else {
            std::cout << "error: invalid flag" << std::endl;
            exit(84);
        }
    }
}


bool ErrorHandler::isDebug() const
{
    return debug;
}
