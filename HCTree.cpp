#include "HCTree.hpp"
#include <iostream>
#include <queue>
#include <stack>

using namespace std;

/** Use the Huffman algorithm to build a Huffman coding trie.
*  PRECONDITION: freqs is a vector of ints, such that freqs[i] is
*  the frequency of occurrence of byte i in the message.
*  POSTCONDITION:  root points to the root of the trie,
*  and leaves[i] points to the leaf node containing byte i.
*/
void HCTree::build(const vector<int>& freqs) {
	priority_queue<HCNode> pq;


	// add all nodes
	cout << freqs.size() << endl;
	for (unsigned int i = 0; i < freqs.size(); i++) {
		cout << "letter " << i << endl;
		// if letter has a frequency
		if (freqs[i] != 0) {
			// Make into node
			HCNode newNode = HCNode(freqs[i], i);
			// add node pointer to priority queue
			pq.push(newNode);
			// add node pointer to leaves
			leaves[i] = &newNode;
		}
	}
	// TODO: Check if pq is empty or already contains only one element
	if (pq.empty()) {
		return;
	}
	if (pq.size() == 1) {
		HCNode rootNode = pq.top();
		root = &rootNode;
		return;
	}

	//bool kk = *leaves[98] < *leaves[99];
	
	// Make tree
	while (pq.size() > 1) {
		// Get lowest frequency nodes
		HCNode left = pq.top();
		pq.pop();
		HCNode right = pq.top();
		pq.pop();
		// Add their frequencies to a new internal node
		HCNode iNode = HCNode(left.count + right.count,NULL);
		
		pq.push(iNode);

		left.p = &iNode;
		right.p = &iNode;
		iNode.c0 = &left;
		iNode.c1 = &right;
	}
	HCNode rootNode = pq.top();
	root = &rootNode;
}

/** Write to the given ofstream
*  the sequence of bits (as ASCII) coding the given symbol.
*  PRECONDITION: build() has been called, to create the coding
*  tree, and initialize root pointer and leaves vector.
*/
void HCTree::encode(byte symbol, ofstream& out) const {
	// Find element in leaves
	stack<int> retStack;
	HCNode* curr = NULL;
	byte i = (byte)symbol;
	if (leaves[i] != NULL) {
		curr = leaves[i];
	
		// Write code to Stack
		while (curr->p != NULL) {
			if (curr->p->c0 == curr) {
				retStack.push(0);
			}
			else {
				retStack.push(1);
			}
			curr = curr->p;
		}
		// Using stack write code to out
		while (!retStack.empty()) {
			out << retStack.top();
			retStack.pop();
		}
	}
}

/** Return the symbol coded in the next sequence of bits (represented as
*  ASCII text) from the ifstream.
*  PRECONDITION: build() has been called, to create the coding
*  tree, and initialize root pointer and leaves vector.
*/
int HCTree::decode(ifstream& in) const {
	// create a pointer that starts at the root
	HCNode* curr = this->root;
	
	char bit;
	int asciiVal;
	while ((bit = in.get()) != EOF) {
		// If current is a leaf, return asciiVal
		if (curr->c0 == NULL) {
			asciiVal = curr->symbol;
			return asciiVal;
		}
		if ((int)bit == 0) {
			curr = curr->c0;
		} else {
			curr = curr->c1;
		}
	}

	return 1;
}