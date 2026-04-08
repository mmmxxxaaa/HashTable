#include <stdio.h>
#include <stdlib.h>
#include "my_list.h"

const int kHashTableSize = 4000;

List** CreateHashTable(int size, ssize_t init_capacity);
void DestroyHashTable(List** hash_table, int size);

int main()
{
    List** hash_table = CreateHashTable(4000, 8);
    if (!hash_table)
        return 1;

    // здесь будет работа с хэш функциями

    DestroyHashTable(hash_table, 4000);
    return 0;
}

List** CreateHashTable(int size, ssize_t init_capacity)
{
    List** hash_table = (List**)calloc(size, sizeof(List*));
    if (!hash_table)
        return NULL;
    for (int i = 0; i < size; i++)
    {
        hash_table[i] = (List*)calloc(1, sizeof(List));
        if (!hash_table[i] || ListCtorWithSpecifiedCapacity(hash_table[i], init_capacity) != LIST_ERROR_NO)
        {
            for (int j = 0; j < i; ++j)
            {
                ListDtor(hash_table[j]);
                free(hash_table[j]);
            }
            free(hash_table);
            return NULL;
        }
    }
    return hash_table;
}

void DestroyHashTable(List** hash_table, int size)
{
    if (!hash_table)
        return;
    for (int i = 0; i < size; ++i)
    {
        if (hash_table[i])
        {
            ListDtor(hash_table[i]);
            free(hash_table[i]);
        }
    }
    free(hash_table);
}
