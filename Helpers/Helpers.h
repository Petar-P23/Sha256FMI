#include <string>;
#include <stdint.h>;
#include "Constants.h";

uint64_t strLen(const char* str);

std::string toHexadecimal(uint32_t n);

void initializeArray(bool* arr, const size_t size);

void copyArr(const bool* from, bool* to, const size_t size);

void copyArr(const char* from, char* to, const size_t size);

bool* charToBits(const char chr);

bool* uint64ToBits(const uint64_t num);

bool* uint32ToBits(const uint32_t num);

bool* strToBits(const char* str, const uint64_t strBitLength);

uint64_t bitsToUint64(const bool* arr, const size_t size);

uint32_t bitsToUint32(const bool* arr, const size_t size);

void deleteMatrixBoolArray(bool**& arr, const size_t size);