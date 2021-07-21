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
	priority_queue<HCNode*> pq;

	// add all nodes
	for each (int i in freqs) {
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
	
	// Make tree
	while (pq.size() > 1) {
		// Get lowest frequency nodes
		HCNode* left = pq.top();
		pq.pop();
		HCNode* right = pq.top();
		pq.pop();
		// Add their frequencies to a new internal node
		HCNode* iNode = new HCNode(left->count + right->count,NULL);

		left->p = iNode;
		right->p = iNode;
		iNode->c0 = left;
		iNode->c1 = right;
	}
	root = pq.top();
}

/** Write to the given ofstream
*  the sequence of bits (as ASCII) coding the given symbol.
*  PRECONDITION: build() has been called, to create the coding
*  tree, and initialize root pointer and leaves vector.
*/
void HCTree::encode(byte symbol, ofstream& out) const {
	// Find element in leaves
	stack<int> retStack;
	HCNode* curr;
	for (size_t i = 0; i < leaves.size(); i++){
		if (leaves[i]->symbol == symbol) {
			curr = leaves[i];
			break;
		}
	}
	
	// Write code to Stack
	while (curr->p != NULL) {
		if (curr->p->c0 == curr) {
			retStack.push(0);
		} else {
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

/** Return the symbol coded in the next sequence of bits (represented as
*  ASCII text) from the ifstream.
*  PRECONDITION: build() has been called, to create the coding
*  tree, and initialize root pointer and leaves vector.
*/
int HCTree::decode(ifstream& in) const {
	// create a pointer that starts at the root
	HCNode* curr = this->root;
	
	char byte = 0;
	while (byte = in.get()) {
		// If current is a leaf, return code and reset curr to root.
		if (curr->c0 == NULL) {
			curr = this->root;
			return curr->symbol;
		}
		if (byte == 0) {
			curr = curr->c0;
		} else {
			curr = curr->c1;
		}
	}

	return 1;
}