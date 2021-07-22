#include "HCTree.hpp"
#include <iostream>
#include <queue>
#include <stack>
#include <functional>
#include <string>

using namespace std;

/** Use the Huffman algorithm to build a Huffman coding trie.
*  PRECONDITION: freqs is a vector of ints, such that freqs[i] is
*  the frequency of occurrence of byte i in the message.
*  POSTCONDITION:  root points to the root of the trie,
*  and leaves[i] points to the leaf node containing byte i.
*/
void HCTree::build(const vector<int>& freqs) {
	priority_queue<HCNode*, vector<HCNode*>, std::function<bool(HCNode*, HCNode*)>> pq(comp);

	// add all nodes
	// cout << freqs.size() << endl;
	for (unsigned int i = 0; i < freqs.size(); i++) {	
		//cout << "letter " << i << endl;
		// if letter has a frequency
		if (freqs[i] != 0) {
			// Make into node
			HCNode* newNode = new HCNode(freqs[i], i);
			// add node pointer to priority queue
			pq.push(newNode);
			// add node pointer to leaves
			leaves[i] = newNode;
		}
	}
	// TODO: Check if pq is empty or already contains only one element
	if (pq.empty()) {
		return;
	}
	if (pq.size() == 1) {
		root = pq.top();
		return;
	}

	//bool kk = *leaves[98] < *leaves[99];

	// Make tree
	while (pq.size() > 1) {
		// Get lowest frequency nodes
		//cout << "making tree.." << endl;
		HCNode* left = pq.top();
		pq.pop();
		HCNode* right = pq.top();
		pq.pop();
		// Add their frequencies to a new internal node
		HCNode* iNode = new HCNode(left->count + right->count, 0);
		left->p = iNode;
		right->p = iNode;

		iNode->c0 = left;
		iNode->c1 = right;

		//if (left->symbol != 0) { // not an internal node
		//	leaves[left->symbol] = left;
		//}
		//if (right->symbol != 0) { // not an internal node
		//	leaves[right->symbol] = right;
		//}
		pq.push(iNode);
	}
	root = pq.top();
	pq.pop();
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
		//cout << "encoding..." << endl;
		curr = leaves[i];

		if (curr == root) {
			out << 0;
			return;
		}

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
	while ((bit = in.get()) != -1) {
		if (curr->c0 == NULL) {
			return curr->symbol;
		}
		// If current is a leaf, return asciiVal
		if (bit == '0') {
			curr = curr->c0;
		}
		else {
			curr = curr->c1;
		}
		if (curr->c0 == NULL) {
			return curr->symbol;
		}
	}
	return -2;
}

void deleteTree(HCNode* rootNode) {
	if (rootNode != NULL) {
		deleteTree(rootNode->c0);
		deleteTree(rootNode->c1);
		delete rootNode;
	}
}

HCTree::~HCTree() {
	deleteTree(root);
};