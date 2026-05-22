/*
** EPITECH PROJECT, 2026
** POULER OR NOT
** File description:
** my_str_to_word_array
*/

#include <stdlib.h>

void free_array(char **arr)
{
    for (int i = 0; arr[i] != NULL; i++)
        free(arr[i]);
    free(arr);
}

int len_array(char **arr)
{
    int i = 0;

    for (; arr[i] != NULL; i++);
    return i;
}

static int in_sep(char *str, int i, char *seps)
{
    for (int j = 0; seps[j] != '\0'; j++) {
        if (str[i] == seps[j])
            return 0;
    }
    return 1;
}

int verif_spaces(char *str, int i, char *seps)
{
    if (i > 0) {
        if ((in_sep(str, i, seps) == 0) &&
            (in_sep(str, i - 1, seps) != 0)) {
                return 0;
        }
    }
    return 1;
}

int count_words_tot(char *str, char *seps)
{
    int nb_spaces = 1;

    for (int i = 0; str[i] != '\0'; i++) {
        if (verif_spaces(str, i, seps) == 0)
            nb_spaces++;
    }
    return nb_spaces;
}

int size_word(char *str, int i, char *seps)
{
    int j = 0;

    for (; str[i + j] != '\0'; j++) {
        if (in_sep(str, i + j, seps) == 0)
            break;
    }
    return j;
}

char *allocate_word(char *str, int i, char *seps)
{
    int size = size_word(str, i, seps);
    char *word = (char *)malloc(sizeof(char) * (size + 1));
    int j = 0;

    if (word == NULL)
        return NULL;
    while (j != size) {
        word[j] = str[i + j];
        j++;
    }
    i += size;
    word[size] = '\0';
    return word;
}

char **my_str_to_word_array(char *str, char *seps)
{
    int i = 0;
    int nb_word = count_words_tot(str, seps);
    char **arr = (char **)malloc(sizeof(char *) * (nb_word + 1));
    int id_word = 0;

    if (!arr)
        return NULL;
    while (id_word < nb_word) {
        for (; in_sep(str, i, seps) == 0; i++);
        arr[id_word] = allocate_word(str, i, seps);
        if (!arr[id_word]) {
            free_array(arr);
            return NULL;
        }
        for (; (in_sep(str, i, seps) != 0) &&
            str[i] != '\0'; i++);
        id_word++;
    }
    arr[id_word] = NULL;
    return arr;
}