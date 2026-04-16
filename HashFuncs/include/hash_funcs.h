#include "my_list.h"
#include <string.h>
#include <assert.h>

uint64_t HashAlwaysOne                      (const char* key);
uint64_t HashIsASCIICodeOfFirstLetter       (const char* key);
uint64_t HashIsLengthOfWord                 (const char* key);
uint64_t HashIsSumOfASCIICodesOfAllLetters  (const char* key);
uint64_t HashIsResOfRotateLeft              (const char* key);
uint64_t HashIsResOfRotateRight             (const char* key);
uint64_t GnuHash                            (const char* key);
uint32_t Crc32Hash                          (const char* key);
