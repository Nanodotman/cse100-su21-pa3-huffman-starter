#include "HCTree.hpp"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv) {
	//cout << "Executing uncompress..." << endl;
	if (argc != 3)
	{
		cout << "This program requires 2 aruments!" << endl;
		return 1;
	}
	
	// Create a vector of frequencies to store the header
	vector<int> freqs(256);

	// Open infile
	ifstream infile;
	infile.open(argv[1], ios::binary);

	if (infile.peek() == std::ifstream::traits_type::eof()) {
		ofstream outfile;
		outfile.open(argv[2], ios::out);
		outfile.close();
		return -1;
	}

	// read infile
	string line;
	int value;
	if (infile.is_open()) {
		// Read header into freqs
		for (size_t i = 0; i < 256; i++) {
			getline(infile, line);
			value = stoi(line);
			freqs[i] = value;
		}
	}
	else {
		cout << "Unable to open file";
		return 1;
	}

	// Build Huffman Tree
	HCTree* HTree = new HCTree();
	HTree->build(freqs);

	// Open outfile
	ofstream outfile;
	outfile.open(argv[2], ios::binary);
	// Decode using Huffman tree
	int asciiVal;
	char c;
	if (infile.is_open()) {
		while (infile.is_open()) {
			asciiVal = HTree->decode(infile);
			if (asciiVal == -2) { 
				break; 
			}
			c = char(asciiVal);
			outfile << c;
		}
	} else {
		cout << "Unable to open file";
		return 1;
	}

	// Close files
	infile.close();
	outfile.close();
}