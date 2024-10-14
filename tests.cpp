// tests.cpp
#include "bstt.h"
#include <gtest/gtest.h>
using namespace std;

// Write test cases for all member functions
// You need many assertions for each component tested
// Make sure to test the generality of the templated class (many types)
// Sample test case on default constructor for int keys and int values
TEST(bsst, empty_constructor) {
  // assertion syntax (will pass if x and y are equal):
  // ASSERT_EQ(x, y);

    bstt<int, int>  tree;

    ASSERT_EQ(tree.size(), 0);
}

TEST(bsst, insert) {
  // assertion syntax (will pass if x and y are equal):
  // ASSERT_EQ(x, y);

    bstt<int, int>  tree2; //insert keys 1-9, in random order
    tree2.insert(5, 200);
    tree2.insert(1, 1);
    tree2.insert(2, 2);
    tree2.insert(3, 3);
    tree2.insert(6, 110);
    tree2.insert(7, 107);
    tree2.insert(9, 109);
    tree2.insert(8, 108);
    tree2.insert(4, 108);

    ASSERT_EQ(tree2.size(), 9); //check size

    //check that keys are sorted 1-9 by traversing the threaded BST
    int key;
    tree2.begin();
    int i = 1;
    while (tree2.next(key)) { 
        ASSERT_EQ(key,i);
        ++i;
    }
}

// Test case for searching elements
TEST(bstt, search) {
    bstt<int, int> tree;
    tree.insert(5, 200);
    tree.insert(1, 1);
    tree.insert(2, 2);
    tree.insert(3, 3);
    tree.insert(6, 110);

    int value;
    ASSERT_TRUE(tree.search(1, value));
    ASSERT_EQ(value, 1);

    ASSERT_TRUE(tree.search(5, value));
    ASSERT_EQ(value, 200);

    ASSERT_FALSE(tree.search(10, value));
}

// Test case for operator[]
TEST(bstt, operator_bracket) {
    bstt<int, string> tree;
    tree.insert(5, "five");
    tree.insert(1, "one");
    tree.insert(2, "two");
    tree.insert(3, "three");

    ASSERT_EQ(tree[1], "one");
    ASSERT_EQ(tree[5], "five)");
    ASSERT_EQ(tree[2], "two");
    ASSERT_EQ(tree[3], "three");
    ASSERT_EQ(tree[10], "");
}

// Test case for operator()
TEST(bstt, operator_parenthesis) {
    bstt<int, int> tree;
    tree.insert(5, 200);
    tree.insert(1, 1);
    tree.insert(2, 2);
    tree.insert(6, 110);

    ASSERT_EQ(tree(5), 6);
    ASSERT_EQ(tree(3), 5);
    ASSERT_EQ(tree(2), 3);
    ASSERT_EQ(tree(1), 2);
    ASSERT_EQ(tree(6), 0); // no node to the right
}

// Test case for clearing the tree
TEST(bstt, clear) {
    bstt<int, int> tree;
    tree.insert(5, 200);
    tree.insert(1, 1);
    tree.insert(2, 2);
    tree.insert(3, 3);
    tree.insert(6, 110);

    tree.clear();
    ASSERT_EQ(tree.size(), 0);

    int value;
    ASSERT_FALSE(tree.search(1, value));
    ASSERT_FALSE(tree.search(5, value));
}

// Test case for copy constructor
TEST(bstt, copy_constructor) {
    bstt<int, int> tree;
    tree.insert(5, 200);
    tree.insert(1, 1);
    tree.insert(2, 2);
    tree.insert(3, 3);
    tree.insert(6, 110);

    bstt<int, int> tree_copy(tree);
    ASSERT_EQ(tree_copy.size(), 5);

    int value;
    ASSERT_TRUE(tree_copy.search(1, value));
    ASSERT_EQ(value, 1);
    ASSERT_TRUE(tree_copy.search(5, value));
    ASSERT_EQ(value, 200);
}

// Test case for assignment operator
TEST(bstt, assignment_operator) {
    bstt<int, int> tree;
    tree.insert(5, 200);
    tree.insert(1, 1);
    tree.insert(2, 2);
    tree.insert(3, 3);
    tree.insert(6, 110);

    bstt<int, int> tree_assign;
    tree_assign = tree;
    ASSERT_EQ(tree_assign.size(), 5);

    int value;
    ASSERT_TRUE(tree_assign.search(1, value));
    ASSERT_EQ(value, 1);
    ASSERT_TRUE(tree_assign.search(5, value));
    ASSERT_EQ(value, 200);
}

// Test case for in-order traversal using next
TEST(bstt, inorder_traversal) {
    bstt<int, int> tree;
    tree.insert(5, 200);
    tree.insert(1, 1);
    tree.insert(2, 2);
    tree.insert(3, 3);
    tree.insert(6, 110);

    int key;
    tree.begin();
    vector<int> inorder_keys;
    while (tree.next(key)) {
        inorder_keys.push_back(key);
    }
    vector<int> expected_keys = {1, 2, 3, 5, 6};
    ASSERT_EQ(inorder_keys, expected_keys);
}

// Test case for different data types (string keys, double values)
TEST(bstt, different_data_types) {
    bstt<string, double> tree;
    tree.insert("five", 5.0);
    tree.insert("one", 1.0);
    tree.insert("two", 2.0);
    tree.insert("three", 3.0);

    ASSERT_EQ(tree["one"], 1.0);
    ASSERT_EQ(tree["five"], 5.0);
    ASSERT_EQ(tree["two"], 2.0);
    ASSERT_EQ(tree["three"], 3.0);
    ASSERT_EQ(tree["ten"], 0.0);

    string key;
    tree.begin();
    vector<string> inorder_keys;
    while (tree.next(key)) {
        inorder_keys.push_back(key);
    }
    vector<string> expected_keys = {"five", "one", "three", "two"};
    ASSERT_EQ(inorder_keys, expected_keys);
}