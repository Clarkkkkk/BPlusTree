/*
 * Tree.h
 *
 *  Created on: Apr 26, 2017
 *      Author: Clark
 */

#ifndef TREE_H_
#define TREE_H_

#include "Node.h"
#include "Elem.h"

class Tree {
public:
	Tree(int order);
	void *lookup(int key);
	void insert(Storage *elem);
	bool deletion(int key);
	void printLeaf();
	void printTree();
	void testPrint();
	Node *root;
private:
	int order;
	Node *head;
	void mergeNodes(Node *orgNode, Node *newNode);
	void cleanNode(Node *trashNode, Node *keepNode);
	bool insertNode(Storage *elem, Node *node);
	bool deleteNode(Storage *elem, Node *node);
	bool borrowPosition(Node *orgNode, Node *borrowNode);
	bool borrowLeafElement(Node *org, Node *borrowNode);
	bool borrowNonLeafElement(Node *org, Node *borrowNode);
	Node *findInsertedNode(Storage *elem);
	Node *findDeleteNode(int key);
	Node *findTheRef(int key, Node *node);
	void splitLeafNode(Node *node);
	void splitNonLeafNode(Node *orgNode, Node *newNode);
	Node *makeLeafNode();
	Node *makeNonLeafNode();
	int findFirstKey(Node *node);
	void resetParentKey(Node *node);
	//bool tryInsertNode(Storage *elem, Node *node);

	void printNonLeaf(Node *node);
};

#endif /* TREE_H_ */
