#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include "my_list.h"
#include <stdint.h>
#include "hash_error_types.h"

typedef uint64_t (*HashFunc_t)(const char* key);

typedef struct {
    List** list_array;
    uint64_t hash_table_size;
    double load_factor;
    HashFunc_t hash_func;
} HashTable;

typedef struct {
    char* word;
    int length;
    int rep_counter;
} ElementInHashTable;

typedef struct {
    const char* name;
    HashFunc_t  pointer;
} HashFuncInfo;

HashTableErrorType HashTableCtor       (HashTable* hash_table, ssize_t size, ssize_t init_capacity, HashFunc_t hash_func);
HashTableErrorType HashTableDtor       (HashTable* hash_table);
HashTableErrorType HashTablePutElement (HashTable* hash_table, const char* element);
HashTableErrorType HashTableVerify     (HashTable* hash_table);
HashTableErrorType HashTableDump       (HashTable* hash_table);
HashTableErrorType HashTableFindElement(HashTable* hash_table, const char* element, int* hash_table_position, int* list_position);

HashTableErrorType HashTableDrawHistogram(HashTable* hash_table, const char*  hist_name, const char* hist_title);

#endif // HASH_TABLE_H_
