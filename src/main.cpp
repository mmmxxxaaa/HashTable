#include <stdio.h>
#include "read_input.h"
#include "hash_table.h"
#include "hash_funcs.h"

int main()
{
    HashFuncInfo hash_funcs[] = {
        #define X(func) {#func, (HashFunc_t) func }
        X(HashAlwaysOne),
        X(HashIsASCIICodeOfFirstLetter),
        X(HashIsLengthOfWord),
        X(HashIsSumOfASCIICodesOfAllLetters),
        X(HashIsResOfRotateLeft),
        X(HashIsResOfRotateRight),
        X(GnuHash),
        X(Crc32Hash)
        #undef X
    };
    const size_t hash_funcs_count = sizeof(hash_funcs) / sizeof(hash_funcs[0]);

    HashTable hash_table = {};

    for (int number_of_hash_func = 0; number_of_hash_func < hash_funcs_count; number_of_hash_func++)
    {
        HashTableCtor(&hash_table, 4000, 1, hash_funcs[number_of_hash_func].pointer); //FIXME

        const char* filepath = "data/book-war-and-peace.txt";
        WordArray words_array = ReadWordsFromFile(filepath);

        if (words_array.words == NULL)
        {
            fprintf(stderr, "Ошибка при чтении файла %s\n", filepath);
            return 1;
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
            return 1;
        }
    
        size_t found_count = 0;
        for (int i = 0; i < words_to_find.words_count; i++)
        {   
            int table_pos = 0, list_pos = 0;
            if (HashTableFindElement(&hash_table, words_to_find.words[i], &table_pos, &list_pos) == HASH_TABLE_ERROR_NO)
                found_count++;
        }

        printf("[%s] Найдено слов: %zu из %d\n",
                hash_funcs[number_of_hash_func].name,
                found_count,
                words_to_find.words_count);
        FreeWordArray(&words_to_find);
//FIXME исследовать нужно только одну функцию (сrc32), а цикл нужно оставить под условной компиляцией для построения гистограмм
#ifdef ENABLE_HISTOGRAMS 
        HashTableDrawHistogram(&hash_table, hash_funcs[number_of_hash_func].name, hash_funcs[number_of_hash_func].name);
#else 
        volatile HashTable *dummy = &hash_table;
        (void)dummy;
#endif // ENABLE_HISTOGRAMS

        FreeWordArray(&words_array);
        HashTableDtor(&hash_table);
    }
    return 0;
}
