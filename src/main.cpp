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
        HashTableCtor(&hash_table, 4000, 3, hash_funcs[number_of_hash_func].pointer); //FIXME

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
