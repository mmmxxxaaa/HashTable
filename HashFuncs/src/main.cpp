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
