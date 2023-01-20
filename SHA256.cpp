#include <iostream>
#include <stdint.h>
#include <string>
#include <fstream>

using namespace std;



const size_t CHAR_BITS_SIZE = 8;
//
const size_t H_VALUES_COUNT = 8;
//
const size_t BLOCKS_COUNT = 16;
const size_t BLOCK_SIZE = 32;
//
const size_t UINT32_BITS_SIZE = 32;
const size_t UINT64_BITS_SIZE = 64;
//
const size_t MESSAGE_SIZE = 64;
const size_t CHUNK_SIZE = 512;
//
const uint32_t K_VALUES[] =
{ 0x428a2f98 ,0x71374491 ,0xb5c0fbcf ,0xe9b5dba5 ,0x3956c25b ,0x59f111f1 ,0x923f82a4 ,0xab1c5ed5 ,
  0xd807aa98 ,0x12835b01 ,0x243185be ,0x550c7dc3 ,0x72be5d74 ,0x80deb1fe ,0x9bdc06a7 ,0xc19bf174 ,
  0xe49b69c1 ,0xefbe4786 ,0x0fc19dc6 ,0x240ca1cc ,0x2de92c6f ,0x4a7484aa ,0x5cb0a9dc ,0x76f988da ,
  0x983e5152 ,0xa831c66d ,0xb00327c8 ,0xbf597fc7 ,0xc6e00bf3 ,0xd5a79147 ,0x06ca6351 ,0x14292967 ,
  0x27b70a85 ,0x2e1b2138 ,0x4d2c6dfc ,0x53380d13 ,0x650a7354 ,0x766a0abb ,0x81c2c92e ,0x92722c85 ,
  0xa2bfe8a1 ,0xa81a664b ,0xc24b8b70 ,0xc76c51a3 ,0xd192e819 ,0xd6990624 ,0xf40e3585 ,0x106aa070 ,
  0x19a4c116 ,0x1e376c08 ,0x2748774c ,0x34b0bcb5 ,0x391c0cb3 ,0x4ed8aa4a ,0x5b9cca4f ,0x682e6ff3 ,
  0x748f82ee ,0x78a5636f ,0x84c87814 ,0x8cc70208 ,0x90befffa ,0xa4506ceb ,0xbef9a3f7 ,0xc67178f2 };

//helper methods
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
//ok
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
//ok
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


//addition methods
bool checkIfIsMax(const bool* arr, const size_t size) {
	for (size_t i = 0; i < size; i++)
	{
		if (!*(arr++)) {
			return false;
		}
	}

	return true;
}
//ok
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
//ok
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
//ok
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

//bit operations
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

//processing

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

void writeToFile(string content, string filename) {
	ofstream ofs;
	ofs.open(filename);

	ofs << content;
	ofs.close();
}

string readFromFile(string filename) {
	string readFromStream;
	ifstream ifs;
	ifs.open(filename);
	getline(ifs, readFromStream);
	ifs.close();

	return readFromStream;
}
int main()
{
	cout << "Enter filename: " << endl;
	string filename;
	cin >> filename;

	string str = readFromFile(filename);

	const char* input = str.c_str();

	std::string encrypted = Sha256(input);
	string newFileName = "encrypted_by_sha_" + filename;

	writeToFile(encrypted, newFileName);

	string readFromStream = readFromFile(newFileName);
	cout << readFromStream;
}
