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

HashTableErrorType HashTableVerify(HashTable* hash_table)
{
    //СИГМА СКИБИДИ
    return HASH_TABLE_ERROR_NO;
}

HashTableErrorType HashTableDump(HashTable* hash_table)
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

    return (pos != -1) ? HASH_TABLE_ERROR_NO : HASH_TABLE_ELEMENT_NOT_FOUND;
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

//FIXME мало передавать указатель на функцию, надо сделать какую-то защиту от перепутывания хэш-функции
//FIXME идея: в массиве структур хранить указатель на функцию и название файла

//FIXME
HashTableErrorType HashTableDrawHistogram(HashTable* hash_table, const char* hist_name, const char* hist_title) //FIXME возможно, нужно оставить только одно название из двух
{
    assert(hash_table);
    assert(hist_name);
    assert(hist_title);

    system("mkdir -p hist");

    char data_filename[256] = {};
    snprintf(data_filename, sizeof(data_filename), "hist/%s.dat", hist_name);

    FILE* data_file = fopen(data_filename, "w");
    if (!data_file)
    {
        fprintf(stderr, "Cannot open %s for writing\n", data_filename);
        return HASH_TABLE_ERROR_OPENING_FILE;
    }

    for (uint64_t i = 0; i < hash_table->hash_table_size; i++)
    {
        fprintf(data_file, "%lu %ld\n", (unsigned long)i, hash_table->list_array[i]->size);
    }
    fclose(data_file);

    char command[1024] = {};
    snprintf(command, sizeof(command),
        "gnuplot -persist -e \""
        "set terminal png size 1200,600; "
        "set output 'hist/%s.png'; "
        "set title '%s'; "
        "set xlabel 'Bucket index'; "
        "set ylabel 'Number of words'; "
        "set style fill solid 0.5; "
        "set boxwidth 0.8; "
        "set xrange [-1:%lu]; "
        "plot '%s' using 1:2 with boxes title ''\"",
        hist_name, hist_title, (unsigned long)(hash_table->hash_table_size - 1), data_filename);

    int ret = system(command);
    if (ret != 0)
    {
        fprintf(stderr, "Gnuplot command failed\n");
        return HASH_TABLE_HISTOGRAM_ERROR;
    }

    return HASH_TABLE_ERROR_NO;
}
