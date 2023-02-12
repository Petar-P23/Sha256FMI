#include "SHA256.h";

void initializeMessage(bool**& message, bool** chunkBits) {
	for (size_t i = 0; i < BLOCKS_COUNT; i++)//first 16
	{
		message[i] = new bool[BLOCK_SIZE];
		copyArr(chunkBits[i], message[i], BLOCK_SIZE);
	}

	for (size_t i = 16; i < MESSAGE_SIZE; i++)//the rest of message
	{
		bool* sigmaOneRes = sigmaOne(message[i - 2], BLOCK_SIZE);
		bool* sigmaZeroRes = sigmaZero(message[i - 15], BLOCK_SIZE);

		const bool* arrays[] = { sigmaZeroRes, sigmaOneRes, message[i - 7], message[i - 16] };
		bool* calculatedBits = bitwiseAddition2Mod32(arrays, BLOCK_SIZE, 4);
		message[i] = calculatedBits;

		delete[] sigmaOneRes;
		delete[] sigmaZeroRes;
	}
}

uint32_t* initializeHValues() {
	uint32_t* hValues = new uint32_t[H_VALUES_COUNT];

	hValues[0] = 0x6a09e667;
	hValues[1] = 0xbb67ae85;
	hValues[2] = 0x3c6ef372;
	hValues[3] = 0xa54ff53a;
	hValues[4] = 0x510e527f;
	hValues[5] = 0x9b05688c;
	hValues[6] = 0x1f83d9ab;
	hValues[7] = 0x5be0cd19;

	return hValues;
}

bool** getNextChunk(bool*& paddedInput) {
	bool** inputBlocks = new bool* [BLOCKS_COUNT];
	for (size_t i = 0; i < BLOCKS_COUNT; i++)
	{
		*inputBlocks = new bool[BLOCK_SIZE];
		copyArr(paddedInput, *inputBlocks, BLOCK_SIZE);
		paddedInput += BLOCK_SIZE;
		inputBlocks++;
	}

	inputBlocks -= BLOCKS_COUNT;

	return inputBlocks;
}

void processHValues(bool** hValuesBinary, bool** message) {
	bool* a = hValuesBinary[0];
	bool* b = hValuesBinary[1];
	bool* c = hValuesBinary[2];
	bool* d = hValuesBinary[3];
	bool* e = hValuesBinary[4];
	bool* f = hValuesBinary[5];
	bool* g = hValuesBinary[6];
	bool* h = hValuesBinary[7];

	for (size_t i = 0; i < MESSAGE_SIZE; i++)
	{
		bool* tempOne = calculateTempOne(h, e, f, g, message[i], BLOCK_SIZE, i);
		bool* tempTwo = calculateTempTwo(a, b, c, BLOCK_SIZE);//ok
		delete[] h;
		h = nullptr;

		h = g;
		g = f;
		f = e;
		const bool* firstToSum[] = { d, tempOne };
		e = bitwiseAddition2Mod32(firstToSum, BLOCK_SIZE, 2);

		d = c;
		c = b;
		b = a;

		const bool* secondToSum[] = { tempOne, tempTwo };
		a = bitwiseAddition2Mod32(secondToSum, BLOCK_SIZE, 2);
	}

	hValuesBinary[0] = a;
	hValuesBinary[1] = b;
	hValuesBinary[2] = c;
	hValuesBinary[3] = d;
	hValuesBinary[4] = e;
	hValuesBinary[5] = f;
	hValuesBinary[6] = g;
	hValuesBinary[7] = h;
}

void fillNewHValues(uint32_t*& newHValues, const uint32_t* hValues, bool** hValuesBinary) {
	for (size_t i = 0; i < H_VALUES_COUNT; i++)
	{
		uint32_t modifiedHValue = bitsToUint32(hValuesBinary[i], BLOCK_SIZE);
		uint32_t* numbersToAdd = new uint32_t[2];
		numbersToAdd[0] = modifiedHValue;
		numbersToAdd[1] = hValues[i];

		newHValues[i] = numberAddition2Mod32(numbersToAdd, 2);

		delete[] numbersToAdd;
	}
}

bool* applyPreprocessingToBits(const bool* bits, const uint64_t bitsSize, const int nearestMultiple) {
	bool* resultBits = new bool[nearestMultiple];

	uint64_t currentIdx = bitsSize;
	copyArr(bits, resultBits, bitsSize);
	resultBits += currentIdx;
	*(resultBits++) = 1;

	while (currentIdx++ != (nearestMultiple - 64)) {
		*(resultBits++) = 0;
	}
	resultBits--;

	bool* lengthBits = uint64ToBits(bitsSize);
	copyArr(lengthBits, resultBits, UINT64_BITS_SIZE);
	delete[] lengthBits;

	return (resultBits - nearestMultiple + UINT64_BITS_SIZE);
}

uint32_t* processChunk(bool** chunkBits, const uint32_t* hValues) {
	bool** message = new bool* [MESSAGE_SIZE];

	initializeMessage(message, chunkBits);//ok

	bool** hValuesBinary = new bool* [H_VALUES_COUNT];
	for (size_t i = 0; i < H_VALUES_COUNT; i++)
	{
		hValuesBinary[i] = uint32ToBits(hValues[i]);
	}
	//ok
	processHValues(hValuesBinary, message);

	uint32_t* newHValues = new uint32_t[H_VALUES_COUNT];
	//ok
	fillNewHValues(newHValues, hValues, hValuesBinary);

	deleteMatrixBoolArray(message, MESSAGE_SIZE);
	deleteMatrixBoolArray(hValuesBinary, H_VALUES_COUNT);

	return newHValues;
}

std::string Sha256(const char* input) {
	uint64_t strBitLength = strLen(input) * CHAR_BITS_SIZE;
	bool* bits = strToBits(input, strBitLength);

	uint64_t paddedInputSize = ((strBitLength + 64 + CHUNK_SIZE - 1) / CHUNK_SIZE) * CHUNK_SIZE;
	bool* paddedInput = applyPreprocessingToBits(bits, strBitLength, paddedInputSize);
	delete[] bits;

	uint64_t chunksCount = paddedInputSize / CHUNK_SIZE;
	uint32_t* hValues = initializeHValues();

	for (size_t i = 1; i <= chunksCount; i++)
	{
		bool** inputChunk = getNextChunk(paddedInput);
		uint32_t* newHValues = processChunk(inputChunk, hValues);

		delete[] hValues;
		hValues = nullptr;
		hValues = newHValues;
	}

	std::string result = "";

	for (size_t i = 0; i < H_VALUES_COUNT; i++)
	{
		result = result + toHexadecimal(hValues[i]);
	}

	return result;
}