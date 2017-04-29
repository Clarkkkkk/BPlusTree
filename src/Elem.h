/*
 * Elem.h
 *
 *  Created on: Apr 26, 2017
 *      Author: Clark
 */

#ifndef ELEM_H_
#define ELEM_H_

#define NOR_ELEM 3
#define KEY_ELEM 4
#define REF_ELEM 5
#define STO_ELEM 6

class Node;

class Elem {
public:
	Elem();
	int elemType;
	int key;
	Elem *next;
	void print();
};

class Key: public Elem {
public:
	Key(int key);
	void print();
};

class Ref: public Elem {
public:
	Ref(int key, Node* ref);
	Node* ref;
	void print();
};

class Storage: public Elem {
public:
	Storage(int key, void* value);
	void* value;
	void print();
};

#endif /* ELEM_H_ */
