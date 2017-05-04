/*
 * Node.cpp
 *
 *  Created on: Apr 26, 2017
 *      Author: Clark
 */

#include "Node.h"
#include "Elem.h"
#include <stdlib.h>
#include <iostream>

Node::Node(int nodeType, int order) {
	// TODO Auto-generated constructor stub
	this->nodeType = nodeType;
	prev = NULL;
	next = NULL;
	this->order = order;
	if (nodeType == LEAF_NODE) {
		this->size = order;
	} else {
		this->size = 2 * order - 1;
		//this->size = 5;
	}
	head = new Elem();
	this->listSize = 0;
	this->parent = NULL;
}

void Node::print() {
	// TODO print out all the key node contains
	Elem *tmp = head;
	cout << " Node: ";
	while(tmp->next != NULL) {
		tmp = tmp->next;
		tmp->print();
	}
	cout << " | " ;
	//cout << "NULL     list size: " << this->listSize << " || ";
}

void Node::printNodeInfo() {
	cout << "NodeInfo--NodeType: " << (this->nodeType == LEAF_NODE ? "Leaf Node ": "Internal Node") << "|";
	cout << "Node max length: " << this->listSize << "|" << "Current size: " << this->size << "|" <<endl;
	print();
	cout << endl;
}
