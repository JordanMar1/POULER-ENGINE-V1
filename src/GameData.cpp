/*
** EPITECH PROJECT, 2026
** POULER OR NOT
** File description:
** parser
*/

#include "../include/array_lib.hpp"
#include "GameData.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

char ***create_game_array(std::ifstream &file)
{
    std::stringstream buffer;
    std::string file_content;
    char *** game_array;

    buffer << file.rdbuf();
    file_content = buffer.str();
    char *file_content_cstr = const_cast<char *>(file_content.c_str());
    char **file_content_array = my_str_to_word_array(file_content_cstr, (char *)"#");
    if (file_content_array == NULL)
        return NULL;
    int size = len_array(file_content_array);
    game_array = (char ***)malloc(sizeof(char **) * (size + 1));
    if (game_array == NULL)
        return NULL;
    for (int i = 0; i < size; i++)
        game_array[i] = my_str_to_word_array(file_content_array[i], (char*)":");
    game_array[size] = NULL;
    free_array(file_content_array);
    return game_array;
}

GameData::GameData(char *filepath)
{
    this->game_array = NULL;
    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filepath << std::endl;
        is_err = 1;
        return;
    }
    this->game_array = create_game_array(file);
    file.close();
    is_err = 0;
}

GameData::~GameData()
{
    if (this->game_array == NULL || this->is_err == 1)
        return;
    for (int i = 0; this->game_array[i] != NULL; i++)
        free_array(this->game_array[i]);
    free(this->game_array);
}
