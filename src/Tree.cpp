/*
 * Tree.cpp
 *
 *  Created on: Apr 26, 2017
 *      Author: Clark
 */

#include "Tree.h"
#include "stdlib.h"
#include <assert.h>
#include <iostream>
#include <queue>

Tree::Tree(int order) {
	// TODO Auto-generated constructor stub
	this->order = order;
	this->root = makeLeafNode();
	this->head = new Node(HEAD_NODE, order);
	this->head->next = root;
}

void Tree::insert(Storage *elem) {
	bool oversize = false;
	Node *nodeToInsert = findInsertedNode(elem);
	oversize = insertNode(elem, nodeToInsert);
	if (oversize) {
		oversize = borrow(nodeToInsert, nodeToInsert->prev);
	}
	if (oversize) {
		oversize = borrow(nodeToInsert, nodeToInsert->next);
	}
	if (oversize) {
		cout << "Need split" << endl;
		splitLeafNode(nodeToInsert);
		mergeNodes(nodeToInsert, nodeToInsert->next);
	}
}

bool Tree::tryInsertNode(Storage *elem, Node *node) {
	if (node->listSize == node->size) {
		return false;
	}
	insertNode(elem, node);
	return true;
}

void Tree::resetParentKey(Node *node) {
	Node *parent = node->parent;
	Elem *ptr = parent->head;
	Elem *ptr1 = ptr->next;
	while (ptr1 != NULL) {
		if (ptr1->elemType == REF_ELEM && ((Ref*) ptr1)->ref == node) {
			((Key*) ptr)->key = this->findFirstKey(node);
			break;
		}
		ptr = ptr1;
		ptr1 = ptr1->next;
	}
	if (parent->parent != NULL) {
		resetParentKey(parent);
	}
}

bool Tree::borrow(Node *orgNode, Node *borrowNode) {
	if (borrowNode == NULL) {
		return true;
	}
	if (borrowNode->listSize == borrowNode->size) {
		return true;
	}
	if (orgNode->nodeType == LEAF_NODE) {
		if (borrowNode == orgNode->prev) {
			cout << "Try to borrow from left sibling" << endl;
			Elem *tmp = orgNode->head->next;
			orgNode->head->next = tmp->next;
			tmp->next = NULL;
			Elem *ptr = borrowNode->head;
			while (ptr->next != NULL) {
				ptr = ptr->next;
			}
			ptr->next = tmp;
			borrowNode->listSize++;
			orgNode->listSize--;
			// Update the key
			resetParentKey(orgNode);
		} else {
			cout << "Try to borrow from right sibling" << endl;
			Elem *ptr = orgNode->head;
			Elem *ptr1 = ptr->next;
			while (ptr1->next != NULL) {
				ptr = ptr1;
				ptr1 = ptr1->next;
			}
			ptr1->next = borrowNode->head->next;
			borrowNode->head->next = ptr1;
			ptr->next = NULL;
			borrowNode->listSize++;
			orgNode->listSize--;
			// Update the key
			resetParentKey(borrowNode);
		}
	} else {

	}

	return false;
}

Node *Tree::findInsertedNode(Storage *elem) {
	Node *tmp = root;
	while (tmp->nodeType != LEAF_NODE) {
		tmp = findTheRef(elem->key, tmp);
	}
	return tmp;
}

// find the right place to insert by key
Node *Tree::findTheRef(int key, Node *node) {
	Elem *key1 = node->head;
	Elem *key2 = key1->next->next;
	while (key2 != NULL) {
		if (key1->key <= key && key2->key > key) {
			Node *res = ((Ref*) (key1->next))->ref;
			return res;
		}
		key1 = key2;
		assert(key2->next != NULL);
		key2 = key2->next->next;
	}
	Node *res = ((Ref*) (key1->next))->ref;
	return res;
}

void Tree::splitLeafNode(Node *orgNode) {
	Node *newNode = makeLeafNode();
	Elem *ptr1 = orgNode->head;
	for (int i = 0; i < orgNode->listSize / 2; i++) {
		ptr1 = ptr1->next;
	}
	Elem *ptr2 = ptr1->next;
	ptr1->next = NULL;
	newNode->head->next = ptr2;
	newNode->next = orgNode->next;
	orgNode->next = newNode;
	newNode->listSize = orgNode->listSize - orgNode->listSize / 2;
	orgNode->listSize = orgNode->listSize / 2;
	if (newNode->next != NULL) {
		newNode->next->prev = newNode;
	}
	newNode->prev = orgNode;
	//newNode->printNodeInfo();
}
void insertKeyRefPair(Node *parent, Elem *key, Elem *ref) {
	Elem *ptr1 = parent->head;
	assert(ptr1->next != NULL);
	Elem *ptr2 = ptr1->next->next;
	while (ptr2 != NULL) {
		if (ptr1->key <= key->key && ptr2->key > key->key) {
			Elem* tmp = ptr1->next->next;
			ptr1->next->next = key;
			ref->next = tmp;
			parent->listSize += 2;
			return;
		}
		ptr1 = ptr2;
		ptr2 = ptr2->next->next;
	}
	ptr1->next->next = key;
	ref->next = NULL;
	parent->listSize += 2;
	return;
}
void Tree::mergeNodes(Node *orgNode, Node *newNode) {
	Node *parent = orgNode->parent;
	Elem *key = new Key(findFirstKey(newNode));
	Elem *ref = new Ref(-1, newNode);
	key->next = ref;
	if (parent == NULL) {
		// TO-DO create new root node
		root = makeNonLeafNode();
		root->listSize = 3;
		Elem *oldRef = new Ref(-1, orgNode);
		root->head->next = oldRef;
		oldRef->next = key;
		key->next = ref;
		ref->next = NULL;
		orgNode->parent = root;
		newNode->parent = root;
		return;
	}
	if (parent->listSize < parent->size) {
		newNode->parent = parent;
		insertKeyRefPair(parent, key, ref);
		return;
	}
	bool oversize = true;
	if (oversize) {
		oversize = borrow(parent, parent->prev);
	}
	if (oversize) {
		oversize = borrow(parent, parent->next);
	}
	if (oversize) {
		insertKeyRefPair(parent, key, ref);
		newNode->parent = parent;
		Node *newParent = makeNonLeafNode();
		splitNonLeafNode(parent, newParent);
		mergeNodes(parent, newParent);
	}
}

int Tree::findFirstKey(Node *node) {
	int res;
	while (node->nodeType != LEAF_NODE) {
		Elem *elem = node->head->next;
		node = ((Ref*) elem)->ref;
	}
	res = node->head->next->key;
	return res;
}

void Tree::splitNonLeafNode(Node *orgNode, Node *newNode) {
	Elem *ptr1 = orgNode->head;
	for (int i = 0; i < orgNode->listSize / 2; i++) {
		ptr1 = ptr1->next;
	}
	Elem *trashKey = ptr1->next;
	Elem *newFirstRef = trashKey->next;
	delete (trashKey);
	ptr1->next = NULL;
	newNode->head->next = newFirstRef;
	orgNode->listSize = orgNode->listSize / 2; // not sure
	// the two list should have same length TO-DO
	newNode->listSize = orgNode->listSize;

	// make the new node in the level list
	newNode->next = orgNode->next;
	orgNode->next = newNode;
	if (newNode->next != NULL) {
		newNode->next->prev = newNode;
	}
	// assign the parent to the elements
	Elem *tmp = newNode->head->next;
	while (tmp != NULL) {
		((Ref*) tmp)->ref->parent = newNode;
		if (tmp->next == NULL) {
			break;
		}
		tmp = tmp->next->next;
	}
}

bool Tree::insertNode(Storage *elem, Node *node) {
	Elem *head = node->head;
	Elem *ptr1 = head;
	Elem *ptr2 = head->next;
	node->listSize++;
	while (ptr2 != NULL) {
		if (ptr1->key <= elem->key && ptr2->key > elem->key) {
			ptr1->next = elem;
			elem->next = ptr2;
			return (node->listSize > node->size ? true : false);
		}
		ptr1 = ptr2;
		ptr2 = ptr2->next;
	}
	ptr1->next = elem;
	elem->next = NULL;
	return (node->listSize > node->size ? true : false);
}

// make new node
Node *Tree::makeLeafNode() {
	return new Node(LEAF_NODE, order);
}

Node *Tree::makeNonLeafNode() {
	return new Node(NORMAL_NODE, order);
}

void Tree::printTree() {
	std::queue<Node*> q1;
	q1.push(root);
	int level = 0;
	while (q1.size() != 0) {
		int size = q1.size();
		cout << "level" << level << endl;
		for (int i = 0; i < size; i++) {
			Node *tmp = q1.front();
			q1.pop();
			if (tmp->nodeType != LEAF_NODE) {
				tmp->print();
				Elem *ptr = tmp->head->next;
				while (ptr != NULL) {
					q1.push(((Ref*) ptr)->ref);
					if (ptr->next == NULL) {
						break;
					}
					ptr = ptr->next->next;
				}
			}
			if (tmp->nodeType == LEAF_NODE) {
				tmp->print();
			}
		}
		cout << endl;
		level++;
	}
	//printLeaf();
}

