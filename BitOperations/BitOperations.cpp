#include "BitOperations.h";

bool* rotateRight(const bool* bits, const int times, const size_t size) {
	bool* rotatedBits = new bool[size];
	initializeArray(rotatedBits, size);

	for (size_t i = 0; i < size; i++)
	{
		int idx = (i + times) % size;
		rotatedBits[idx] = bits[i];
	}

	return rotatedBits;
}

bool* shiftRight(const bool* bits, const int times, const size_t size) {
	bool* shiftedBits = new bool[size];
	for (size_t i = 0; i < size; i++)
	{
		shiftedBits[i] = 0;
	}

	int shiftsCount = size - times;
	for (size_t i = 0; i < shiftsCount; i++)
	{
		shiftedBits[i + times] = bits[i];
	}

	return shiftedBits;
}

bool* sigmaZero(const bool* bits, const size_t size) {
	bool* first = rotateRight(bits, 7, size);
	bool* second = rotateRight(bits, 18, size);
	bool* third = shiftRight(bits, 3, size);

	bool* arrays[] = { first,second,third };

	bool* added = XOR(arrays, size, 3);

	delete[] first;
	delete[] second;
	delete[] third;

	return added;
}

bool* sigmaOne(const bool* bits, const size_t size) {
	bool* first = rotateRight(bits, 17, size);
	bool* second = rotateRight(bits, 19, size);
	bool* third = shiftRight(bits, 10, size);

	bool* arrays[] = { first,second,third };

	bool* added = XOR(arrays, size, 3);

	delete[] first;
	delete[] second;
	delete[] third;

	return added;
}

bool* capitalSigmaZero(const bool* bits, const size_t size) {
	bool* first = rotateRight(bits, 2, size);
	bool* second = rotateRight(bits, 13, size);
	bool* third = rotateRight(bits, 22, size);

	bool* arrays[] = { first,second,third };

	bool* added = XOR(arrays, size, 3);

	delete[] first;
	delete[] second;
	delete[] third;

	return added;
}

bool* capitalSigmaOne(const bool* bits, const size_t size) {
	bool* first = rotateRight(bits, 6, size);
	bool* second = rotateRight(bits, 11, size);
	bool* third = rotateRight(bits, 25, size);

	bool* arrays[] = { first,second,third };

	bool* added = XOR(arrays, size, 3);

	delete[] first;
	delete[] second;
	delete[] third;

	return added;
}

bool* chooseFunc(const bool* first, const bool* second, const bool* third, const size_t size) {
	bool* result = new bool[size];

	for (size_t i = 0; i < size; i++)
	{
		*result = (*first & *second) ^ (!(*first) & *third);

		first++;
		second++;
		third++;
		result++;
	}

	return result - size;
}

bool* majorityFunc(const bool* first, const bool* second, const bool* third, const size_t size) {
	bool* result = new bool[size];
	int zeroes = 0;
	int ones = 0;
	for (size_t i = 0; i < size; i++)
	{
		if (*(first++)) {
			ones++;
		}
		else {
			zeroes++;
		}

		if (*(second++)) {
			ones++;
		}
		else {
			zeroes++;
		}

		if (*(third++)) {
			ones++;
		}
		else {
			zeroes++;
		}

		*(result++) = ones > zeroes ? 1 : 0;
		ones = 0;
		zeroes = 0;
	}

	return (result - size);
}

bool* calculateTempOne(const bool* h, const bool* e, const bool* f, const bool* g, const bool* word, const size_t size, const int idx) {
	bool* capitalSigmaRes = capitalSigmaOne(e, size);
	bool* chooseResult = chooseFunc(e, f, g, size);
	bool* KValueBits = uint32ToBits(K_VALUES[idx]);

	const bool* arrays[] = { h, capitalSigmaRes, chooseResult, KValueBits, word };

	bool* result = bitwiseAddition2Mod32(arrays, size, 5);

	delete[] capitalSigmaRes;
	delete[] chooseResult;
	delete[] KValueBits;

	return result;
}

bool* calculateTempTwo(const bool* a, const bool* b, const bool* c, const size_t size) {
	bool* capitalSigmaRes = capitalSigmaZero(a, size);
	bool* majorityRes = majorityFunc(a, b, c, size);

	const bool* arrays[] = { capitalSigmaRes,majorityRes };

	bool* result = bitwiseAddition2Mod32(arrays, size, 2);

	delete[] capitalSigmaRes;
	delete[] majorityRes;

	return result;
}