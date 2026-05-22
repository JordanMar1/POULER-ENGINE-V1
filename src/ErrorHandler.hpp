/*
** EPITECH PROJECT, 2026
** POULER OR NOT
** File description:
** ErrorHandler
*/

#pragma once

#include <string>
#include <iostream>

class ErrorHandler
{
    public:
        ErrorHandler(int argc, char **argv);
        ~ErrorHandler() = default;
        bool isDebug() const;
        char *getFilepath() const { return filepath; }
    private:
        bool debug;
        char *filepath;
};