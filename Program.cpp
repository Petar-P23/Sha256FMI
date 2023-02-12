#include "SHA256.h";

#include <fstream>
#include <iostream>

using namespace std;

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
