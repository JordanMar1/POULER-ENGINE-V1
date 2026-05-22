/*
** EPITECH PROJECT, 2026
** POULER OR NOT
** File description:
** my_str_to_word_array
*/

#pragma once

extern "C" {
    char **my_str_to_word_array(char *str, char *seps);
    void free_array(char **arr);
    int len_array(char **arr);
}