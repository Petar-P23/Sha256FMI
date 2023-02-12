#include "Addition.h";

bool checkIfIsMax(const bool* arr, const size_t size) {
	for (size_t i = 0; i < size; i++)
	{
		if (!*(arr++)) {
			return false;
		}
	}

	return true;
}

bool* XOR(bool* arrays[3], const size_t arrSize, const size_t arraysCount) {
	bool* addedArray = new bool[arrSize];
	for (size_t i = 0; i < arrSize; i++)
	{
		bool currentBit = arrays[0][i];
		for (size_t j = 1; j < arraysCount; j++)
		{
			currentBit = arrays[j][i] ^ currentBit;
		}
		*(addedArray++) = currentBit;
	}

	return addedArray - arrSize;
}

bool* addTwoArrays2Mod32(const bool* first, const bool* second, const size_t arrSize) {
	uint64_t result = bitsToUint64(first, arrSize) + bitsToUint64(second, arrSize);
	uint32_t max = (uint32_t)0 - 1;
	if (result > max)
		result -= max + 1;

	bool* bits = uint64ToBits(result);
	bool* resultBits = new bool[arrSize];
	for (size_t i = 0; i < arrSize; i++)
	{
		resultBits[arrSize - i - 1] = bits[UINT64_BITS_SIZE - i - 1];
	}

	delete[] bits;
	return resultBits;
}

uint32_t addTwoNumbers2Mod32(const uint32_t first, const uint32_t second) {
	uint64_t result = (uint64_t)first + second;
	uint32_t max = (uint32_t)0 - 1;
	if (result > max)
		result -= max + 1;

	return result;
}

bool* bitwiseAddition2Mod32(const bool** arrays, const size_t arrSize, const size_t arraysCount) {
	bool* addedArray = new bool[arrSize];
	copyArr(arrays[0], addedArray, BLOCK_SIZE);

	for (size_t j = 1; j < arraysCount; j++)
	{
		bool* addition = addTwoArrays2Mod32(addedArray, arrays[j], arrSize);
		delete[] addedArray;
		addedArray = nullptr;
		addedArray = addition;
	}
	return addedArray;
}

uint32_t numberAddition2Mod32(const uint32_t* numbers, const size_t numbersCount) {
	uint32_t result = numbers[0];
	for (size_t j = 1; j < numbersCount; j++)
	{
		uint32_t addition = addTwoNumbers2Mod32(result, numbers[j]);
		result = addition;
	}

	return result;
}