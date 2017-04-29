/*
 * Node.h
 *
 *  Created on: Apr 26, 2017
 *      Author: Clark
 */

#ifndef NODE_H_
#define NODE_H_


#define LEAF_NODE 0
#define NORMAL_NODE 1
#define HEAD_NODE 2

#include <vector>
using namespace std;
class Elem;

class Node {
public:
	Node(int nodeType, int order);
	void print();
	void printNodeInfo();
	Elem *head;
	int nodeType;
	int size;
	int listSize;
	Node *parent;

	Node *prev;
	Node *next;
private:
	int order;
};

#endif /* NODE_H_ */
