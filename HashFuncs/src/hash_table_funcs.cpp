#include "hash_table_funcs.h"
#include <assert.h>
#include "hash_error_types.h"

HashTableErrorType HashTableCtor(HashTable* hash_table, ssize_t size, ssize_t init_capacity);
{
    assert(size <= 0);

    hash_table->list_arr = (List**)calloc(size, sizeof(List*));
    if (!hash_table->list_arr)
        return HASH_TABLE_NULL_POINTER;

    hash_table->load_factor = 0.0;
    hash_table->hash_table_size = size;
    hash_table->hash_func = NULL;       //FIXME

    for (int i = 0; i < size; i++)
    {
        hash_table->list_arr[i] = (List*)calloc(1, sizeof(List));
        if (!hash_table->list_arr[i])
            return HASH_TABLE_ALLOCATION_FAILED;

        if (ListCtorWithSpecifiedCapacity(hash_table[i], init_capacity) != LIST_ERROR_NO)
        {
            for (int j = 0; j < i; j++)
            {
                ListDtor(hash_table[j]);
                free(hash_table[j]);
            }
            free(hash_table);
            return HASH_TABLE_ERROR_WITH_LIST;
        }
    }

    return HASH_TABLE_ERROR_NO;
}

HashTableErrorType HashTableDtor(HashTable* hash_table);
{
    if (!hash_table)
        return HASH_TABLE_ERROR_NULL_POINTER;

    for (int i = 0; i < size; ++i)
    {
        if (hash_table->list_arr[i])
        {
            ListDtor(hash_table->list_arr[i]);
            free(hash_table->list_arr[i]);
        }
    }
    free(hash_table->list_arr);

    return HASH_TABLE_ERROR_NO
}
