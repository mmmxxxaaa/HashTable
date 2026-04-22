#include <stdio.h>
#include "hash_table.h"
#include "hash_funcs.h"
#include "hash_error_types.h"

const int kHashTableSize = 4000;

int main()
{
    HashTable hash_table = {};
    HashTableErrorType err = HashTableCtor(&hash_table, 10, 1, HashAlwaysOne);
    if (err != HASH_TABLE_ERROR_NO)
    {
        fprintf(stderr, "HashTableCtor failed with error %d\n", err);
        return -1;
    }

    const char* const strings[] = {"raz", "dva", "tri", "chetire", "pyat", "shest", "sem"};
    const size_t size     = sizeof(strings) / sizeof(strings[0]);

    for (size_t i = 0; i < size; i++)
    {
        err = HashTablePutElement(&hash_table, strings[i]);
        if (err != HASH_TABLE_ERROR_NO)
        {
            fprintf(stderr, "PutElement failed for %s, error %d\n", strings[i], err);
            HashTableDtor(&hash_table);
            return -2;
        }
    }

    for (size_t i = 0; i < size; i++)
    {
        int hash_table_pos = -42;
        int list_pos       = -52;

        err = HashTableFindElement(&hash_table, strings[i], &hash_table_pos, &list_pos);
        if (err != HASH_TABLE_ERROR_NO)
        {
            printf("FindElement failed for %s, error %d\n", strings[i], err);
            break;
        }
        printf("Found %s at [%d][%d]\n", strings[i], hash_table_pos, list_pos);
    }

    HashTableDtor(&hash_table);
    return 0;
}
