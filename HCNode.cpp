#include "HCTree.hpp"
#include <iostream>
#include <queue>
#include <stack>

using namespace std;

bool comp(HCNode* one, HCNode* other) {
        // if counts are different, just compare counts
        cout << "Comparing less than" << endl;
        if (one->count != other->count) {
            return one->count > other->count;
        }
        // counts are equal. use symbol value to break tie.
        return one->symbol < other->symbol;
};

