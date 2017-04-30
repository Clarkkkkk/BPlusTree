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

int getMinSize(Node *node) {
	int minSize = node->size / 2;
	if (node->nodeType == LEAF_NODE) {
		if (node->size % 2 == 1) {
			minSize++;
		}
		return minSize;
	} else {
		minSize++;
		return minSize;
	}
}

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
		oversize = borrowPosition(nodeToInsert, nodeToInsert->prev);
	}
	if (oversize) {
		oversize = borrowPosition(nodeToInsert, nodeToInsert->next);
	}
	if (oversize) {
		cout << "Need split" << endl;
		splitLeafNode(nodeToInsert);
		mergeNodes(nodeToInsert, nodeToInsert->next);
	}
}

Elem *checkTheElem(Node *node, int key) {
	Elem *tmpElem = node->head->next;
	while (tmpElem != NULL) {
		if (tmpElem->key == key) {
			return tmpElem;
		}
		tmpElem = tmpElem->next;
	}
	return NULL;
}

bool Tree::deletion(int key) {
	Node *deleteNode = findDeleteNode(key);
	Elem *deleteElem = checkTheElem(deleteNode, key);
	if (deleteElem == NULL) {
		return false;
	}
	bool belowSize = this->deleteNode((Storage*)deleteElem, deleteNode);
	if (deleteNode == root) {
		return true;
	}
	if (belowSize) {
		belowSize = borrowLeafElement(deleteNode, deleteNode->prev);
	}
	if (belowSize) {
		belowSize = borrowLeafElement(deleteNode, deleteNode->next);
	}
	if (belowSize) {
		cout << "drop one node" << endl;
		if (deleteNode->prev != NULL && deleteNode->prev->listSize + deleteNode->listSize <= deleteNode->prev->size) {
			cleanNode(deleteNode, deleteNode->prev);
		} else {
			cleanNode(deleteNode, deleteNode->next);
		}
	}
	return true;
}

// return true if size is too small
bool Tree::deleteNode(Storage *elem, Node *node) {
	Elem *ptr = node->head;
	Elem *ptr1 = ptr->next;
	while (ptr1 != NULL) {
		if (ptr1 == elem) {
			break;
		}
		ptr = ptr1;
		ptr1 = ptr1->next;
	}
	ptr->next = ptr1->next;
	delete(elem);
	node->listSize--;

	if ((double)node->listSize >= (double)node->size / 2.0) {
		return false;
	}
	return true;
}
/*
bool Tree::tryInsertNode(Storage *elem, Node *node) {
	if (node->listSize == node->size) {
		return false;
	}
	insertNode(elem, node);
	return true;
}
*/

void Tree::resetParentKey(Node *node) {
	Node *parent = node->parent;
	Elem *ptr = parent->head;
	Elem *ptr1 = ptr->next;
	while (ptr1 != NULL) {
		if (ptr1->elemType == REF_ELEM && ((Ref*) ptr1)->ref == node) {
			if (ptr != parent->head) {
				((Key*) ptr)->key = this->findFirstKey(node);
			}
			break;
		}
		ptr = ptr1;
		ptr1 = ptr1->next;
	}
	if (parent->parent != NULL) {
		resetParentKey(parent);
	}
}

bool Tree::borrowPosition(Node *orgNode, Node *borrowNode) {
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
		if (borrowNode == orgNode->prev) {
			cout << "Try to borrow from left sibling non leaf" << endl;
			Elem *ref = orgNode->head->next;
			Elem *key = ref->next;
			orgNode->head->next = key->next;
			delete(key);
			Elem *ptr = borrowNode->head;
			while (ptr->next != NULL) {
				ptr = ptr->next;
			}
			ptr->next = new Key(this->findFirstKey(((Ref*)ref)->ref));
			ptr->next->next = ref;
			ref->next = NULL;
			// assign the parent of referenced node to borrowNode
			((Ref*)ref)->ref->parent = borrowNode;
			resetParentKey(orgNode);
			borrowNode->listSize += 2;
			orgNode->listSize -= 2;
		} else {
			// Not tested
			cout << "Try to borrow from right sibling non leaf" << endl;
			Elem *ptr = orgNode->head;
			Elem *ptr1 = ptr->next->next;
			while (ptr1->next != NULL) {
				ptr1 = ptr1->next;
				ptr = ptr->next;
			}
			Elem *key = ptr->next;
			ptr->next = NULL;
			delete(key);
			Elem *orgFirst = borrowNode->head->next;
			borrowNode->head->next = ptr1;
			int newKeyNumber = findFirstKey(((Ref*)orgFirst)->ref);
			Elem *newKey = new Key(newKeyNumber);
			ptr1->next = newKey;
			newKey->next = orgFirst;
			// assign the parent of referenced node to borrowNode
			((Ref*)ptr1)->ref->parent = borrowNode;
			resetParentKey(borrowNode);
			borrowNode->listSize += 2;
			orgNode->listSize -= 2;		}
	}

	return false;
}

bool Tree::borrowNonLeafElement(Node *orgNode, Node *borrowNode) {
	if (borrowNode == NULL) {
		return true;
	}
	int minSize = getMinSize(borrowNode);
	if (borrowNode->listSize <= minSize) {
		return true;
	}
	if (borrowNode == orgNode->prev) {
		// borrow ref and create key from left node
		Elem *ptr = borrowNode->head;
		Elem *ptr1 = ptr->next->next;
		while (ptr1->next != NULL) {
			ptr = ptr->next;
			ptr1 = ptr1->next;
		}
		Elem *lastRef = ptr1;
		Elem *newLastRef = ptr;
		delete(newLastRef->next);
		newLastRef->next = NULL;
		Ref *orgFirstRef = (Ref*)(orgNode->head->next);
		Key *newKey = new Key(findFirstKey(orgFirstRef->ref));
		orgNode->head->next = lastRef;
		lastRef->next = newKey;
		newKey->next = orgFirstRef;
		borrowNode->listSize -= 2;
		orgNode->listSize += 2;
	}
	return false;
}

bool Tree::borrowLeafElement(Node *orgNode, Node *borrowNode) {
	if (borrowNode == NULL) {
		return true;
	}
	int minSize = getMinSize(borrowNode);
	if (borrowNode->listSize <= minSize) {
		return true;
	}
	// TO-DO borrow element from node
	if (borrowNode == orgNode->prev) {
		cout << "Borrow an element from left Node" << endl;
		Elem *ptr = borrowNode->head;
		Elem *ptr1 = ptr->next;
		while (ptr1->next != NULL) {
			ptr = ptr1;
			ptr1 = ptr1->next;
		}
		Elem *borrowElem = ptr1;
		ptr->next = NULL;
		borrowElem->next = orgNode->head->next;
		orgNode->head->next = borrowElem;
		borrowNode->listSize--;
		orgNode->listSize++;
		resetParentKey(orgNode);
	} else {
		cout << "Borrow an element from right Node" << endl;
		Elem *borrowElem = borrowNode->head->next;
		borrowNode->head->next = borrowElem->next;
		borrowElem->next = NULL;
		Elem *ptr = orgNode->head->next;
		while (ptr->next != NULL) {
			ptr = ptr->next;
		}
		ptr->next = borrowElem;
		resetParentKey(borrowNode);
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

Node *Tree::findDeleteNode(int key) {
	Node *tmp = root;
	while (tmp->nodeType != LEAF_NODE) {
		tmp = findTheRef(key, tmp);
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
	newNode->parent = orgNode->parent;
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
	insertKeyRefPair(parent, key, ref);
	bool oversize = true;
	if (oversize) {
		oversize = borrowPosition(parent, parent->prev);
	}
	if (oversize) {
		oversize = borrowPosition(parent, parent->next);
	}
	if (oversize) {
		newNode->parent = parent;
		Node *newParent = makeNonLeafNode();
		splitNonLeafNode(parent, newParent);
		mergeNodes(parent, newParent);
	}
}

void Tree::cleanNode(Node *trashNode, Node *keepNode) {
	if (trashNode->prev == keepNode) {
		// move elements to new node
		Elem *trashNodeElem = trashNode->head->next;
		Elem *ptr = keepNode->head;
		while (ptr->next != NULL) {
			ptr = ptr->next;
		}
		ptr->next = trashNodeElem;

		// clean the parent part
		Node *parent = trashNode->parent;
		ptr = parent->head;
		Elem *ptr1 = ptr->next;
		if (((Ref*)ptr1)->ref == trashNode) {
			Elem *key = ptr1->next;
			Elem *nextRef = key->next;
			ptr->next = nextRef;
			delete(ptr1);
			delete(key);
			parent->listSize -= 2;
			if ((double)parent->listSize >= parent->size / 2.0) {
				return;
			} else {
				// if it is the root
				if (parent == root) {
					return;
				}
				bool notDone = true;
				// borrow ref and key from left non-leaf node
				if (notDone) {
					notDone = this->borrowNonLeafElement(parent, parent->prev);
				}
				// borrow ref and key from right non-leaf node
				if (notDone) {
					notDone = this->borrowNonLeafElement(parent, parent->next);
				}
				if (notDone) {
					if (parent->prev != NULL) {
						// clean self with leaf node
						this->cleanNode(parent, parent->prev);
					} else {
						// clean self with right node
						this->cleanNode(parent, parent->next);
					}
				}
			}

		}

	}
	if (trashNode->next == keepNode) {

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
	newNode->prev = orgNode;
	if (newNode->next != NULL) {
		newNode->next->prev = newNode;
	}

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

void Tree::testPrint() {
	Node *ptr = head->next->parent;
	while (ptr != NULL) {
		ptr->print();
		ptr = ptr->next;
	}
	cout << endl;
}

//print
void Tree::printLeaf() {
	cout << "Leaf level" << endl;
	Node *ptr = head;
	while (ptr->next != NULL) {
		ptr = ptr->next;
		ptr->print();
	}
	cout << endl;
}
