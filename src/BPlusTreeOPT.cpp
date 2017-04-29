//============================================================================
// Name        : BPlusTreeOPT.cpp
// Author      : Clark
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Tree.h"
#include <assert.h>
using namespace std;

void towLevelInsertTest() {
	int val = 1;
	int keys[] = {20, 18, 22, 28, 25, 41, 23, 42, 53, 32, 33, 38};
	Tree *tree = new Tree(4);
	for (int i = 0; i < 12; i++) {
		//tree->root->printNodeInfo();
		tree->insert(new Storage(keys[i], &val));
		//tree->root->printNodeInfo();
		tree->printTree();
	}
}

void threeLevelInsertTest() {
	int val = 1;
	int keys[] = {20, 18, 22, 28, 25, 41, 23, 42, 53, 32, 33, 38, 24, 35, 62, 63, 84, 73, 99, 64, 65, 66};
	int x = 10; // all good below 7
	Tree *tree = new Tree(4);
	for (int i = 0; i < 12; i++) {
		tree->insert(new Storage(keys[i], &val));
	}
	tree->printTree();
	for (int i = 12; i < 12 + x; i++) {
		Storage *newSto = new Storage(keys[i], &val);
		if (i == 11 + x) {
			tree->testPrint();
		}
		tree->insert(newSto);
		tree->testPrint();
		tree->printTree();
		tree->testPrint();
	}
}

void testNonLeafBorrowRight() {
	int val = 1;
	int keys[] = {20, 18, 22, 28, 25, 41, 23, 42, 53, 32, 33, 38, 35, 24, 25, 25, 25};
	Tree *tree = new Tree(4);
	for (int i = 0; i < 17; i++) {
		//tree->root->printNodeInfo();
		tree->insert(new Storage(keys[i], &val));
		//tree->root->printNodeInfo();
		tree->printTree();
	}
}

int main() {
	//towLevelInsertTest();
	//threeLevelInsertTest();
	testNonLeafBorrowRight();
	cout << "!!!B+Tree!!!" << endl; // prints !!!B+Tree!!!
	return 0;
}
