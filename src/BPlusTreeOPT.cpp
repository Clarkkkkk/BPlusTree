//============================================================================
// Name        : BPlusTreeOPT.cpp
// Author      : Clark
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Tree.h"
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
	int keys[] = {20, 18, 22, 28, 25, 41, 23, 42, 53, 32, 33, 38, 24, 35, 62, 63, 84, 73, 99};
	int x = 7;
	Tree *tree = new Tree(4);
	for (int i = 0; i < 12; i++) {
		tree->insert(new Storage(keys[i], &val));
	}
	tree->printTree();
	for (int i = 12; i < 12 + x; i++) {
		Storage *newSto = new Storage(keys[i], &val);
		tree->insert(newSto);
		tree->printTree();
	}
}

int main() {
	//towLevelInsertTest();
	threeLevelInsertTest();
	cout << "!!!B+Tree!!!" << endl; // prints !!!B+Tree!!!
	return 0;
}
