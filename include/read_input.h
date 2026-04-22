#ifndef READ_INPUT_H_
#define READ_INPUT_H_

#include <stddef.h>

typedef struct {
    char** words;      // массив указателей на слова
    size_t words_count;      // количество слов
    char*  buffer;     // исходный буфер (для последующего освобождения)
} WordArray;

WordArray ReadWordsFromFile(const char* filename);
void FreeWordArray(WordArray* wa);

#endif // READ_INPUT_H_
