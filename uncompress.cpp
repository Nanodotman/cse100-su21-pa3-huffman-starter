#include "HCTree.hpp"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv) {
	cout << "Executing compress..." << endl;
	if (argc != 2)
	{
		cout << "This program requires 2 aruments!" << endl;
		return 1;
	}
	
	// Create a vector of frequencies to store the header
	vector<int> freqs(256);

	// Open infile
	ifstream infile;
	infile.open(argv[1], ios::binary);
	// read infile
	string line;
	int value;
	if (infile.is_open()) {
		// Read header into freqs
		for (size_t i = 0; i < 256; i++) {
			getline(infile, line);
			value = stoi(line);
			//infile.getline();
			//stringstream value(line);
			//int x = 0;
			//value >> x;
			freqs[i] = value;
		}
	}
	else {
		cout << "Unable to open file";
		return 1;
	}

	// Build Huffman Tree
	HCTree* HTree = new HCTree();
	//HTree->build(freqs);

	// Open outfile
	ofstream outfile;
	outfile.open(argv[2], ios::binary);
	// Decode using Huffman tree
	int asciiVal;
	char c;
	if (infile.is_open()) {
		while (infile.good()) {
			asciiVal = HTree->decode(infile);
			c = char(asciiVal);
			outfile << c;
		}
	}
	else {
		cout << "Unable to open file";
		return 1;
	}

	// Close files
	infile.close();
	outfile.close();
}