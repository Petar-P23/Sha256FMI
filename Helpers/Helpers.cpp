#include "Helpers.h";

uint64_t strLen(const char* str) {
	uint64_t counter = 0;

	while (*(str++) != '\0') {
		counter++;
	}

	return counter;
}

std::string toHexadecimal(uint32_t n) {
	std::string ans = "";

	while (n != 0) {
		uint32_t rem = 0;

		char ch;
		rem = n % 16;

		if (rem < 10) {
			ch = rem + 48;
		}
		else {
			ch = rem + 55;
		}

		ans += ch;
		n = n / 16;
	}

	uint32_t i = 0, j = ans.size() - 1;
	while (i <= j)
	{
		std::swap(ans[i], ans[j]);
		i++;
		j--;
	}
	return ans;
}

void initializeArray(bool* arr, const size_t size) {
	for (size_t i = 0; i < size; i++)
	{
		arr[i] = 0;
	}
}

void copyArr(const bool* from, bool* to, const size_t size) {
	for (size_t i = 0; i < size; i++)
	{
		*to = *from;
		from++;
		to++;
	}
}

void copyArr(const char* from, char* to, const size_t size) {
	for (size_t i = 0; i < size; i++)
	{
		*to = *from;
		from++;
		to++;
	}
}

bool* charToBits(const char chr) {
	bool* bitsResult = new bool[CHAR_BITS_SIZE];

	for (size_t i = 1; i <= CHAR_BITS_SIZE; i++)
	{
		char mask = 1;
		char copyOfChr = chr;
		mask <<= (CHAR_BITS_SIZE - i);
		copyOfChr = copyOfChr & mask;
		*(bitsResult++) = copyOfChr != 0;
	}

	return (bitsResult - CHAR_BITS_SIZE);
}

bool* uint64ToBits(const uint64_t num) {
	bool* bitsResult = new bool[UINT64_BITS_SIZE];

	for (size_t i = 1; i <= UINT64_BITS_SIZE; i++)
	{
		uint64_t mask = 1u;
		uint64_t copyOfChr = num;
		mask <<= (UINT64_BITS_SIZE - i);
		copyOfChr = copyOfChr & mask;
		*(bitsResult++) = copyOfChr != 0;
	}

	return (bitsResult - UINT64_BITS_SIZE);
}

bool* uint32ToBits(const uint32_t num) {
	bool* bitsResult = new bool[UINT32_BITS_SIZE];

	for (size_t i = 1; i <= UINT32_BITS_SIZE; i++)
	{
		uint32_t mask = 1u;
		uint32_t copyOfChr = num;
		mask <<= (UINT32_BITS_SIZE - i);
		copyOfChr = copyOfChr & mask;
		*(bitsResult++) = copyOfChr != 0;
	}

	return (bitsResult - UINT32_BITS_SIZE);
}

bool* strToBits(const char* str, const uint64_t strBitLength) {
	bool* bitsResult = new bool[strBitLength];

	while (*str != '\0') {
		bool* charBits = charToBits(*str);
		copyArr(charBits, bitsResult, CHAR_BITS_SIZE);
		delete[] charBits;

		str++;
		bitsResult += CHAR_BITS_SIZE;
	}

	return (bitsResult - strBitLength);
}

uint64_t bitsToUint64(const bool* arr, const size_t size) {
	uint64_t res = 0;
	arr += size - 1;
	uint64_t base = 1;

	for (int i = 0; i < size; i++) {
		bool last = *(arr--);

		res += last * base;

		base = base << 1;
	}

	return res;
}

uint32_t bitsToUint32(const bool* arr, const size_t size) {
	uint64_t res = 0;
	arr += size - 1;
	uint64_t base = 1;

	for (int i = 0; i < size; i++) {
		bool last = *(arr--);

		res += last * base;

		base = base << 1;
	}

	return res;
}

void deleteMatrixBoolArray(bool**& arr, const size_t size) {
	for (size_t i = 0; i < size; i++)
	{
		delete[] arr[i];
	}

	delete[] arr;
}
