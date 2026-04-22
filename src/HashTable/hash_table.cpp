#include "hash_table.h"
#include <stdlib.h>
#include <assert.h>
#include "hash_error_types.h"

static List* FindListByKey(HashTable* hash_table, const char* key)
{
    uint64_t hash = hash_table->hash_func(key);
    size_t index = hash % hash_table->hash_table_size;
    return hash_table->list_array[index];
}

HashTableErrorType HashTableCtor(HashTable* hash_table, ssize_t size, ssize_t init_capacity, HashFunc_t hash_func)
{
    assert(hash_table != NULL);
    assert(size > 0);
    assert(hash_func != NULL);

    hash_table->list_array = (List**)calloc(size, sizeof(List*));
    if (!hash_table->list_array)
        return HASH_TABLE_ALLOCATION_FAILED;

    hash_table->load_factor = 0.0;
    hash_table->hash_table_size = size;
    hash_table->hash_func = hash_func;

    for (uint64_t i = 0; i < size; i++)
    {
        hash_table->list_array[i] = (List*)calloc(1, sizeof(List));
        if (!hash_table->list_array[i])
        {
            for (uint64_t j = 0; j < i; j++)
            {
                ListDtor(hash_table->list_array[j]);
                free(hash_table->list_array[j]);
            }
        free(hash_table->list_array);
        return HASH_TABLE_ALLOCATION_FAILED;
        }

        if (ListCtorWithSpecifiedCapacity(hash_table->list_array[i], init_capacity) != LIST_ERROR_NO)
        {
            ListDtor(hash_table->list_array[i]);
            free(hash_table->list_array[i]);
            for (uint64_t j = 0; j < i; j++)
            {
                ListDtor(hash_table->list_array[j]);
                free(hash_table->list_array[j]);
            }
            free(hash_table->list_array);
            hash_table->list_array = NULL;
            return HASH_TABLE_LIST_ERROR;
        }
    }
    return HASH_TABLE_ERROR_NO;
}

HashTableErrorType HashTablePutElement(HashTable* hash_table, const char* element)
{
    assert(hash_table != NULL);
    assert(element != NULL);

    List* list = FindListByKey(hash_table, element);
    if (list->capacity == 0)
    {
        if (ListInsertAfter(list, 0, element) != LIST_ERROR_NO) //FIXME вроде есть же функция, которая в самое начало вставляет
            return HASH_TABLE_LIST_ERROR;

        return HASH_TABLE_ERROR_NO;
    }

    int pos = -1;

    if (ListFindElement(list, element, &pos) != LIST_ERROR_NO)
        return HASH_TABLE_LIST_ERROR;

    if (pos != -1)
        return HASH_TABLE_ERROR_NO;

    if (ListInsertAfterTail(list, element) != LIST_ERROR_NO)
        return HASH_TABLE_LIST_ERROR;

    return HASH_TABLE_ERROR_NO;
}

HashTableErrorType HashTableVerify     (HashTable* hash_table)
{
    //СИГМА СКИБИДИ
    return HASH_TABLE_ERROR_NO;
}

HashTableErrorType HashTableDump       (HashTable* hash_table)
{
    //СИГМА СКИБИДИ
    return HASH_TABLE_ERROR_NO;
}

HashTableErrorType HashTableFindElement(HashTable* hash_table, const char* element, int* hash_table_position, int* list_position)
{
    uint64_t hash_value = hash_table->hash_func(element);

    if (hash_table->hash_table_size == 0)
    {
        fprintf(stderr, "Хэш таблица пуста");
        return HASH_TABLE_CAPACITY_IS_ZERO;
    }

    size_t element_index = hash_value % hash_table->hash_table_size;

    List* list = FindListByKey(hash_table, element);
    int pos = -1;
    if (ListFindElement(list, element, &pos) != LIST_ERROR_NO)
        return HASH_TABLE_LIST_ERROR;

    *hash_table_position = element_index;
    *list_position       = pos;

    return HASH_TABLE_ERROR_NO;
}

HashTableErrorType HashTableDtor(HashTable* hash_table)
{
    if (!hash_table)
        return HASH_TABLE_NULL_POINTER;

    if (hash_table->list_array)
    {
        for (uint64_t i = 0; i < hash_table->hash_table_size; i++)
        {
            if (hash_table->list_array[i])
            {
                ListDtor(hash_table->list_array[i]);
                free(hash_table->list_array[i]);
            }
        }
        free(hash_table->list_array);
        hash_table->list_array = NULL;
    }
    return HASH_TABLE_ERROR_NO;
}
