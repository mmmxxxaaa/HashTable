#include "read_input.h"
#include <stdio.h>

int main()
{
    const char* filepath = "data/book-war-and-peace.txt";
    WordArray words_array = ReadWordsFromFile(filepath);

    if (words_array.words == NULL)
    {
        fprintf(stderr, "Ошибка при чтении файла %s\n", filepath);
        return 1;
    }

    size_t words_to_print = words_array.words_count < 30 ? words_array.words_count : 30;
    printf("Первые %zu слов из файла '%s':\n", words_to_print, filepath);
    for (size_t i = 0; i < words_to_print; i++)
    {
        printf("%zu: %s\n", i + 1, words_array.words[i]);
    }

    FreeWordArray(&words_array);
    return 0;
}
