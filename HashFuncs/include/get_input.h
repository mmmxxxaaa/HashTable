#ifndef GET_INPUT_H_
#define GET_INPUT_H_

uint64_t GetSizeOfFile(const char* input_filename);
HashTableErrorType ReadTextFromFile(FILE* input_file);

#endif // GET_INPUT_H_
