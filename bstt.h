/*
 * File: bstt.h
 * Name: Hiba Mirza
 * NetID: hmirz4
 * Course: CS 251 Data Structures (21814) 2024 Summer
 * Program Overview: Threaded binary search tree implements a Binary Search Tree with threading support.
 * The tree allows for insertions, searches, in-order traversal, and other BST operations.
 */

#pragma once

#include <iostream>

using namespace std;

template<typename KeyT, typename ValueT>
class bstt {
private:
  struct NODE {
    KeyT   Key;
    ValueT Value;
    NODE*  Left;
    NODE*  Right;
    bool   isThreaded;
  };

  NODE* Root;  // pointer to root node of tree (nullptr if empty)
  int   Size;  // # of nodes in the tree (0 if empty)
  NODE* currNode; // pointer to the current node for the in-order traversal


//
// Returns the leftmost node starting from a given node
// Parameters: node - starting node
// Returns: point to the leftmost node
//
  NODE* leftMost(NODE* node) {
    if (!node) return nullptr;
    while (node->Left) {
        node = node->Left;
    }
    return node;
  }

//
// Inserts nodes in the tree using pre-order traversal
// Parameters: other - node to start pre-order traversal
//
  void preOrder(NODE* other) {
    if (!other) return;
    insert(other->Key,other->Value);
    preOrder(other->Left);
    if (!other->isThreaded) {
        preOrder(other->Right);
    }
  }

//
// Outputs the tree in in-order traversal to the given stream
// Parameters: node - starting node for in-order traversal, output - output stream
//
  void inOrder(NODE* node, ostream& output) const {
    if (!node) return;
    inOrder(node->Left, output);
    if (node->isThreaded && node->Right) {
        output << "(" << node->Key << "," << node->Value << "," << node->Right->Key << ")" << endl;
        node->Right = nullptr;
    } else {
        output << "(" << node->Key << "," << node->Value << ")" << endl;
    }
    inOrder(node->Right, output);
  }

  //
  // Deletes all nodes in the tree recursively
  // Parameters: node - starting node for deletion
  //
  void clearNodes(NODE* node) {
    if (!node) return;
    clearNodes(node->Left);
    if (node->isThreaded) {
        node->Right = nullptr;
    }
    clearNodes(node->Right);
    delete node;
  }

public:
  //
  // default constructor:
  //
  // Creates an empty tree.
  //
  bstt() {
    Root = nullptr;
    Size = 0;
    currNode = nullptr;
  }

  //
  // copy constructor
  // Creates a deep copy of the given tree
  // Parameters: other - the tree to copy
  //
  bstt(const bstt& other) {
    Root = nullptr;
    Size = 0;
    currNode = nullptr;
    if (other.Root){
        preOrder(other.Root);
    }
  }

  //
  // destructor:
  //
  // Called automatically by system when tree is about to be destroyed;
  // this is our last chance to free any resources / memory used by
  // this tree.
  //
  virtual ~bstt() {
    clearNodes(Root);
  }

  //
  // operator=
  //
  // Clears "this" tree and then makes a copy of the "other" tree.
  // Parameters: other - the tree to copy
  // Returns: reference to the current tree
  //
  bstt& operator=(const bstt& other) {
    if (this == &other) return *this;
    clear();
    preOrder(other.Root);
    Size = other.Size;
    return *this;
  }

  //
  // clear:
  //
  // Clears the contents of the tree, resetting the tree to empty.
  //
  void clear() {
    clearNodes(Root);
    Root = nullptr;
    Size = 0;
  }

  // 
  // size:
  //
  // Returns the # of nodes in the tree, 0 if empty.
  //
  // Time complexity:  O(1) 
  // Returns: integer size of the tree
  //
  int size() const {
    return Size;
  }

  // 
  // search:
  //
  // Searches the tree for the given key, returning true if found
  // and false if not.  If the key is found, the corresponding value
  // is returned via the reference parameter.
  //
  // Time complexity:  O(lgN) on average
  // Parameters: key - key to search for, value - reference to store found value
  // Returns: boolean indicating if key was found
  //
  bool search(KeyT key, ValueT& value) const {
    NODE* node = Root;
    while (node) {
        if(key == node->Key) {
            value = node->Value;
            return true;
        }
        if (key < node->Key) {
            node = node->Left;
        } else {
            if (node->isThreaded) {
                node = nullptr;
            } else {
                node = node->Right;
            }
        }
    }
    return false;
  }

  //
  // insert
  //
  // Inserts the given key into the tree; if the key has already been insert then
  // the function returns without changing the tree.
  //
  // Time complexity:  O(lgN) on average
  // Parameters: key - key to insert, value - value associated with the key
  //
  void insert(KeyT key, ValueT value) {
    NODE* parent = nullptr;
    NODE* node = Root;
    while (node) {
        if (key == node->Key) return;
        if (key < node->Key) {
            parent =  node;
            node = node->Left;
        } else {
            parent = node;
            if (node->isThreaded) {
                node = nullptr;
            } else {
                node = node->Right;
            }
        }
    }
    NODE* newNode = new NODE{key, value, nullptr, nullptr, true};
    if (!parent) {
        Root = newNode;
    } else if (key < parent->Key) {
        newNode->Right = parent;
        parent->Left = newNode;
    } else {
        newNode->Right = parent->Right;
        parent->isThreaded = false;
        parent->Right = newNode;
    }
    ++Size;
  }

  //
  // []
  //
  // Returns the value for the given key; if the key is not found,
  // the default value ValueT{} is returned.
  //
  // Time complexity:  O(lgN) on average
  // Parameters: key - key to search for
  // Returns: value associated with the key or default value
  //
  ValueT operator[](KeyT key) const {
    NODE* node = Root;
    while (node) {
        if (key == node->Key) return node->Value;
        if (key < node->Key) {
            node = node->Left;
        } else {
            if (node->isThreaded) {
                node = nullptr;
            } else {
                node = node->Right;
            }
        }
    }
    return ValueT{};
  }

  //
  // ()
  //
  // Finds the key in the tree, and returns the key to the "right".
  // If the right is threaded, this will be the next inorder key.
  // if the right is not threaded, it will be the key of whatever
  // node is immediately to the right.
  //
  // If no such key exists, or there is no key to the "right", the
  // default key value KeyT{} is returned.
  //
  // Time complexity:  O(lgN) on average
  // Parameters: key - key to search for
  // Returns: key to the right or default key value
  //
  KeyT operator()(KeyT key) const {
    NODE* node = Root;
    while (node) {
        if (key == node->Key) {
            if (node->isThreaded && !node->Right) return KeyT{};
            if (node->isThreaded && node->Right) {
                return node->Right->Key;
            }
            if (!node->isThreaded) {
                return node->Right->Key;
            }
        }
        if (key < node->Key) {
            node = node->Left;
        } else {
            if (node->isThreaded) {
                node = nullptr;
            } else {
                node = node->Right;
            }
        }
    }
    return KeyT{ };
  }

  //
  // begin
  //
  // Resets internal state for an inorder traversal.  After the 
  // call to begin(), the internal state denotes the first inorder
  // key; this ensure that first call to next() function returns
  // the first inorder key.
  //
  // Space complexity: O(1)
  // Time complexity:  O(lgN) on average
  //
  void begin() {
    currNode = leftMost(Root);
  }

  //
  // next
  //
  // Uses the internal state to return the next inorder key, and 
  // then advances the internal state in anticipation of future
  // calls.  If a key is in fact returned (via the reference 
  // parameter), true is also returned.
  //
  // False is returned when the internal state has reached null,
  // meaning no more keys are available.  This is the end of the
  // inorder traversal.
  //
  // Space complexity: O(1)
  // Time complexity:  O(lgN) on average
  //
  // Parameters: key - reference to store the next inorder key
  // Returns: boolean indicating if there are more keys
  //
  bool next(KeyT& key) {
    if (!currNode) return false;
    key = currNode->Key;
    if (!currNode->Left && currNode-> isThreaded) {
        currNode = currNode->Right;
    } else if (!currNode->Left && !currNode->isThreaded) {
        currNode = currNode->Right;
        currNode = leftMost(currNode);
    } else if (currNode->Left && !currNode->isThreaded) {
        currNode = currNode->Right;
        currNode = leftMost(currNode);
    } else if (currNode->Left && currNode->isThreaded) {
        currNode = currNode->Right;
    }
    return true;
  }

  //
  // dump
  // 
  // Dumps the contents of the tree to the output stream, using a
  // recursive inorder traversal.
  // Parameters: output - output stream
  //
  void dump(ostream& output) const {
    output << "**************************************************" << endl;
    output << "********************* BSTT ***********************" << endl;

    output << "** size: " << this->size() << endl;

    //
    // inorder traversal, with one output per line: either 
    // (key,value) or (key,value,THREAD)
    //
    inOrder(Root, output);

    output << "**************************************************" << endl;
  }

};