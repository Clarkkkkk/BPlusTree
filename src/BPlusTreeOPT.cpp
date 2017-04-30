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

void deleteTest() {
	int val = 1;
	int keys[] = {97, 94, 93, 91, 90, 72, 64, 52, 48, 47, 42, 34, 27, 21, 20, 14, 8, 7, 98};
	Tree *tree = new Tree(3);
	int x = 19;
	for (int i = 0; i < x; i++) {
		Storage *elem = new Storage(keys[i], &val);
		cout << "insert " << keys[i] << endl;
		if (keys[i] == 47) {
			cout << "";
		}
		tree->insert(elem);
		tree->printTree();
		if (keys[i] <= 47) {
			//tree->printNonLeaf(((Ref*)(tree->root->head->next))->ref);
		}
		//cout << tree->root->head->key << endl;
	}
	tree->printTree();
	tree->printNonLeaf(((Ref*)(tree->root->head->next))->ref);
	//borrow elem from left test
	bool ans = tree->deletion(97);
	cout << ans << endl;
	//tree->printTree();
	tree->printNonLeaf(((Ref*)(tree->root->head->next))->ref);

	/*ans = tree->deletion(52);
	tree->printTree();
	cout << ans << endl;
	ans = tree->deletion(64);
	cout << ans << endl;
	tree->printTree();

	// borrow elem from right test
	 ans = tree->deletion(8);
	cout << ans << endl;
	ans = tree->deletion(14);
	cout << ans << endl;
	tree->printTree();
	*/

}

int main() {
	//towLevelInsertTest();
	//threeLevelInsertTest();
	//testNonLeafBorrowRight();
	deleteTest();
	cout << "!!!B+Tree!!!" << endl; // prints !!!B+Tree!!!
	return 0;
}
