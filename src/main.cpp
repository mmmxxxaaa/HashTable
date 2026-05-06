#include <stdio.h>
#include <x86intrin.h>  
#include <assert.h>
#include "read_input.h"
#include "hash_table.h"
#include "hash_funcs.h"

#ifdef ENABLE_HISTOGRAMS
static void WriteDispersionHeader(FILE* md_file) 
{
    assert(md_file);

    fprintf(md_file, "# Дисперсия распределения слов по ячейкам\n\n");
    fprintf(md_file, "| Хэш-функция | Дисперсия |\n");
    fprintf(md_file, "|------------|-----------|\n");
}

static void ProcessHistogramMode(HashFuncInfo* hash_funcs, size_t hash_funcs_count)
{
    system("mkdir -p assets");

    FILE* md_file = fopen("assets/dispersion.md", "w");
    if (!md_file) 
    {
        fprintf(stderr, "Ошибка: не удалось создать assets/dispersion.md\n");
        return;
    }
    WriteDispersionHeader(md_file);

    for (int number_of_hash_func = 0; number_of_hash_func < hash_funcs_count; number_of_hash_func++)
    {
        HashTable hash_table = {};
        HashTableCtor(&hash_table, kHashTableCapacity, kInitListCapacityAtCell, hash_funcs[number_of_hash_func].pointer); //FIXME

        const char* filepath = "data/book-war-and-peace.txt";
        WordArray words_array = ReadWordsFromFile(filepath);

        if (words_array.words == NULL)
        {
            fprintf(stderr, "Ошибка при чтении файла %s\n", filepath);
            return;
        }
        for (int i = 0; i < words_array.words_count; i++)
        {
            HashTablePutElement(&hash_table, words_array.words[i]);
        }
        HashTableDrawHistogram(&hash_table, hash_funcs[number_of_hash_func].name, hash_funcs[number_of_hash_func].name);
        double disp = ComputeDispersion(&hash_table);
        fprintf(md_file, "| %s | %.2f |\n", hash_funcs[number_of_hash_func].name, disp);

        FreeWordArray(&words_array);
        HashTableDtor(&hash_table);
    }
    fclose(md_file);
}
#endif // ENABLE_HISTOGRAMS

#ifndef ENABLE_HISTOGRAMS
static void ProcessBenchmarkMode(HashFuncInfo* hash_funcs, size_t count)
{
    int number_of_hash_func = hash_funcs_count - 1; 
    printf("Используется хэш-функция: %s\n", hash_funcs[idx].name);

    HashTable hash_table = {};
    HashTableCtor(&hash_table, kHashTableCapacity, kInitListCapacityAtCell, hash_funcs[number_of_hash_func].pointer); 

    const char* filepath = "data/book-war-and-peace.txt";
    WordArray words_array = ReadWordsFromFile(filepath);
    if (words_array.words == NULL)
    {
        fprintf(stderr, "Ошибка при чтении файла %s\n", filepath);
        return;
    }
    for (int i = 0; i < words_array.words_count; i++)
    {
        HashTablePutElement(&hash_table, words_array.words[i]);
    }

    WordArray words_to_find = ReadWordsFromFile("data/words_to_find.txt");
    if (words_to_find.words == NULL)
    {
        fprintf(stderr, "Ошибка при чтении файла data/words_to_find.txt");
        FreeWordArray(&words_array);
        HashTableDtor(&hash_table);
        return;
    }

    const int repetitions = 200;
    size_t found_count = 0;
    unsigned long long start_ticks = __rdtsc();
    for (int rep = 0; rep < repetitions; rep++)
    {
        for (size_t i = 0; i < words_to_find.words_count; i++)
        {
            int table_pos = 0, list_pos = 0;
            if (HashTableFindElement(&hash_table, words_to_find.words[i], &table_pos, &list_pos) == HASH_TABLE_ERROR_NO)
                found_count++;
        }
    }
    unsigned long long end_ticks = __rdtsc();
    unsigned long long total_ticks = end_ticks - start_ticks;
    size_t total_searches = words_to_find.words_count * repetitions;
    printf("[%s] Найдено слов: %zu из %zu\n",
            hash_funcs[number_of_hash_func].name, found_count, total_searches); //FIXME
    printf("Циклов процессора: %llu\n", total_ticks);
    FreeWordArray(&words_to_find);

    FreeWordArray(&words_array);
    HashTableDtor(&hash_table);
}
#endif // !ENABLE_HISTOGRAMS

int main()
{
    HashFuncInfo hash_funcs[] = {
        #define INIT_HASH_FUNC_IN_MASSIVE(func) {#func, (HashFunc_t) func }
        INIT_HASH_FUNC_IN_MASSIVE(HashAlwaysOne),
        INIT_HASH_FUNC_IN_MASSIVE(HashIsASCIICodeOfFirstLetter),
        INIT_HASH_FUNC_IN_MASSIVE(HashIsLengthOfWord),
        INIT_HASH_FUNC_IN_MASSIVE(HashIsSumOfASCIICodesOfAllLetters),
        INIT_HASH_FUNC_IN_MASSIVE(HashIsResOfRotateLeft),
        INIT_HASH_FUNC_IN_MASSIVE(HashIsResOfRotateRight),
        INIT_HASH_FUNC_IN_MASSIVE(GnuHash),
        INIT_HASH_FUNC_IN_MASSIVE(Crc32Hash)
        #undef INIT_HASH_FUNC_IN_MASSIVE
    };
    const size_t hash_funcs_count = sizeof(hash_funcs) / sizeof(hash_funcs[0]);

#ifdef ENABLE_HISTOGRAMS
    ProcessHistogramMode(hash_funcs, hash_funcs_count);
#else
    ProcessBenchmarkMode(hash_funcs, hash_funcs_count);
#endif 
    return 0;
}
