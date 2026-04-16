#include "my_list.h"
#include <string.h>
#include <assert.h>

uint64_t HashAlwaysOne(const char* key)
{
    assert(key != NULL);

    return 1;
}

uint64_t HashIsASCIICodeOfFirstLetter(const char* key)
{
    assert(key != NULL);

    return (uint64_t)*key;
}

uint64_t HashIsLengthOfWord(const char* key)
{
    assert(key != NULL);
    uint64_t hash = 0;
    for( ; *key; key++)
        hash++;

    return hash;
}

uint64_t HashIsSumOfASCIICodesOfAllLetters(const char* key)
{
    assert(key != NULL);

    uint64_t hash = 0;
    for( ; *key; key++)
        hash += (uint64_t)*key;

    return hash;
}

uint64_t HashIsResOfRotateLeft(const char* key)
{
    assert(key != NULL);

    uint8_t ch = (uint8_t)*key;
    uint8_t rotated = (ch << 1) | (ch >> 7);

    return (uint64_t)rotated;
}

uint64_t HashIsResOfRotateRight(const char* key)
{
    assert(key != NULL);

    uint8_t ch = (uint8_t)*key;
    uint8_t rotated = (ch >> 1) | (ch << 7);

    return (uint64_t) rotated;
}

uint64_t GnuHash(const char* key)
{
    assert(key != NULL);

    uint64_t hash = 5381;

    for ( ; *key; key++)
        hash = (hash << 5) + hash + (uint8_t)(*key);

    return hash;
}

uint32_t Crc32Hash(const char* key)
{
    assert(key != NULL);

    uint32_t crc = 0xFFFFFFFF;

    while (*key)
    {
        crc ^= (uint8_t)(*key);

        for (int i = 0; i < 8; i++)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xEDB88320;
            else
                crc >>= 1;
        }
        key++;
    }

    return ~crc;
}
