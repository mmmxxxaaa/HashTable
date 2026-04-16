#ifndef HASH_TABLE_FUNCS_H_
#define HASH_TABLE_FUNCS_H_

#include "my_list.h"

typedef uint64_t *(hash_func_t)(const char* key)

typedef struct {
    List** list_array;
    uint64_t hash_table_size;
    double load_factor;
    hash_func_t hash_func;
} HashTable;

typedef struct {
    char* word;
    int length;
    int rep_counter;
} ElementInHashTable

HashTableErrorType HashTableCtor(HashTable* hash_table, ssize_t size, ssize_t init_capacity);
HashTableErrorType HashTableDtor(HashTable* hash_table);


#endif // HASH_TABLE_FUNCS_H_
