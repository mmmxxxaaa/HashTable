#include "read_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

static long GetFileSize(FILE* fp)
{
    long pos = ftell(fp);
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, pos, SEEK_SET);
    return size;
}

static int IsSeparator(int c)
{
    return !isalnum(c);
}

WordArray ReadWordsFromFile(const char* filename)
{
    WordArray result = {};
    FILE* fp = fopen(filename, "rb"); //FIXME
    if (!fp)
    {
        fprintf(stderr, "Cannot open file \"%s\"\n", filename);
        return result;
    }

    long file_size = GetFileSize(fp);
    if (file_size <= 0)
    {
        fclose(fp);
        return result;
    }

    char* buffer = (char*)calloc(file_size + 1, sizeof(char)); // буфер под все содержимое и '\0'
    if (!buffer)
    {
        fprintf(stderr, "Memory allocation failed for file buffer\n");
        fclose(fp);
        return result;
    }

    size_t read_bytes = fread(buffer, 1, file_size, fp);
    fclose(fp);
    buffer[read_bytes] = '\0';

    char* first_pass_buffer_pointer = buffer;           // первый проход - считаем количество слов
    size_t words_count = 0;
    int in_word = 0;
    while (*first_pass_buffer_pointer)
    {
        if (!IsSeparator(*first_pass_buffer_pointer))
        {
            if (!in_word)
            {
                in_word = 1;
                words_count++;
            }
        }
        else
        {
            in_word = 0;
        }
        first_pass_buffer_pointer++;
    }

    result.words = (char**)calloc(words_count, sizeof(char*));  //массив указателей
    if (!result.words)
    {
        fprintf(stderr, "Memory allocation failed for word pointers\n");
        free(buffer);
        return result;
    }

    char* second_pass_buffer_pointer = buffer;           // второй проход - записываем указатели
    in_word = 0;
    size_t idx = 0;
    while (*second_pass_buffer_pointer)
    {
        if (!IsSeparator(*second_pass_buffer_pointer))
        {
            if (!in_word)
            {
                in_word = 1;
                result.words[idx++] = second_pass_buffer_pointer;   // начало слова
            }
        }
        else
        {
            if (in_word)
            {
                *second_pass_buffer_pointer = '\0';                 // завершаем слово
                in_word = 0;
            }
        }
        second_pass_buffer_pointer++;
    }

    result.words_count = words_count;
    result.buffer = buffer;   // сохраняем для освобождения
    return result;
}

void FreeWordArray(WordArray* word_array)
{
    if (word_array)
    {
        free(word_array->buffer);
        free(word_array->words);

        word_array->words  = NULL;
        word_array->buffer = NULL;
        word_array->words_count  = 0;
    }
}
