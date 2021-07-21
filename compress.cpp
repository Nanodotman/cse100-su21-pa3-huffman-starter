#include "HCTree.hpp"
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
	if (argc != 2) {
		cout << "This program requires 2 aruments!" << endl;
		return 1;
	}

	// Create vector with freqs
	vector<int> freqs(256);
	for (size_t i = 0; i < 256; i++) {
		freqs[i] = 0;
	}

	// Open infile
	ifstream infile;
	infile.open(argv[1], ios::binary);
	// read infile
	int input;
	if (infile.is_open()) {
		while (input = infile.get()) {
			freqs[input]++;
		}
	}
	else { 
		cout << "Unable to open file"; 
		return 1;
	}
	// Close infile
	infile.close();

	// Build Huffman tree
	HCTree* HTree = new HCTree();
	HTree->build(freqs);

	// Open outfile
	ofstream outfile;
	outfile.open(argv[2], ios::binary);
	// Write header for outfile (256 ints)
	for (int i : freqs) {
		outfile << i << endl;
	}
	// Write to the outfile using Huffman encoding
	ifstream infile;
	infile.open(argv[1], ios::binary);
	
	if (infile.is_open()) {
		while (input = infile.get()) {
			HTree->encode(input, outfile);
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