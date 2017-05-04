/*
 * Elem.cpp
 *
 *  Created on: Apr 26, 2017
 *      Author: Clark
 */

#include "Elem.h"
#include "Node.h"
#include <stdlib.h>
#include <iostream>

Elem::Elem() {
	this->key = -1;
	this->next = NULL;
	this->elemType = NOR_ELEM;
}

void Elem::print() {
	if (elemType == KEY_ELEM) {
		((Key*)this)->print();
	} else if (elemType == REF_ELEM) {
		((Ref*)this)->print();
	} else {
		((Storage*)this)->print();
	}
}

Key::Key(int key) {
	this->elemType = KEY_ELEM;
	this->key = key;
	this->next = NULL;}

void Key::print() {
	//cout << "Type: key, Keyvalue:" << key << ".";
	//cout << "key:" << key << "-> ";
	//cout << "key:" << key << " -> ";
	cout << " " << key << " ";
}

Ref::Ref(int key, Node* ref) {
	this->elemType = REF_ELEM;
	this->key = key;
	this->ref = ref;
	this->next = NULL;
}

void Ref::print() {
	//cout << "Type:Reference, Keyvalue:" << key << ".";
	//cout << "key:" << key << "-> ";
	//cout << "Ref -> ";
}

Storage::Storage(int key, void* value) {
	this->elemType = STO_ELEM;
	this->key = key;
	this->value = value;
	this->next = NULL;
}

void Storage::print() {
	//cout << "Type: Storage, Keyvalue:" << key << ".";
	//cout << "key:" << key << "->";
	//cout << "storage:" << key << " -> ";
	cout << " " << key << " ";
}
