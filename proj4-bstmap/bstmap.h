// Krrish Karwal

#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

using namespace std;

template <typename KeyT, typename ValT>
class BSTMap {
 private:
  struct BSTNode {
    const KeyT key;  // This member should never change after initialization!
    ValT value;
    BSTNode* parent;
    BSTNode* left;
    BSTNode* right;

    // This is fancy constructor syntax
    // Don't worry too much about it -- it's just necesssary for the const
    // member.
    BSTNode(KeyT key, ValT value, BSTNode* parent)
        : key(key),
          value(value),
          parent(parent),
          left(nullptr),
          right(nullptr) {
    }
  };

  BSTNode* root;
  size_t sz;

  // Utility pointer for begin and next.
  BSTNode* curr;

  // Searches for and returns a node with the given key using recursion
  BSTNode* findBSTNode(BSTNode* node, const KeyT& key) const
  {
    // If the given node is nullptr, return nullptr
    if (node == nullptr)
    {
      return nullptr;
    }

    if (node->key == key)
    {
      return node;
    }
    else if (key < node->key)
    {
      return findBSTNode(node->left, key);
    }
    else
    {
      return findBSTNode(node->right, key);
    }
  }

  // Deletes a tree using recursion
  void deleteBST(BSTNode* node)
  {
    if (node == nullptr)
    {
      return;
    }
    deleteBST(node->left);
    deleteBST(node->right);
    delete node;
  }

  // Prints out a BSTMap using recursion in string format, where each node is printed as "<key>: <value>"
  // Each node is printed on its own line
  void printBST(BSTNode* node, ostringstream& oss) const
  {
    if (node == nullptr)
    {
      return;
    }
    printBST(node->left, oss);
    oss << node->key << ": " << node->value << endl;
    printBST(node->right, oss);
  }

  // Copies a BSTMap using recursion
  BSTNode* copyBST(BSTNode* otherNode, BSTNode* parentNode)
  {
    if (otherNode == nullptr)
    {
      return nullptr;
    }

    BSTNode* newNode = new BSTNode(otherNode->key, otherNode->value, parentNode);
    newNode->left = copyBST(otherNode->left, newNode);
    newNode->right = copyBST(otherNode->right, newNode);
    return newNode;
  }

  // Compares two BSTMaps to see if their contents are equal using recursion
  bool compareBST(BSTNode* nodeA, BSTNode* nodeB) const
  {
    if ((nodeA == nullptr) && (nodeB == nullptr))
    {
      return true;
    }

    if ((nodeA == nullptr) || (nodeB == nullptr))
    {
      return false;
    }

    if ((nodeA->key != nodeB->key) || (nodeA->value != nodeB->value))
    {
      return false;
    }

    return compareBST(nodeA->left, nodeB->left) && compareBST(nodeA->right, nodeB->right);
  }

  // Finds the smallest node in a BST (Moves left until there are no more nodes on the left)
  BSTNode* findMin(BSTNode* node) const
  {
    if (node == nullptr)
    {
      return nullptr;
    }

    while (node->left != nullptr)
    {
      node = node->left;
    }

    return node;
  }

  // Reconnects a child node to a new parent node
  void reconnectChildToParent(BSTNode* node, BSTNode* newChildNode)
  {
    // Updates the parent's link (left/right pointer)
    if (node->parent == nullptr)
    {
      root = newChildNode;
    }
    else if (node == node->parent->left)
    {
      node->parent->left = newChildNode;
    }
    else
    {
      node->parent->right = newChildNode;
    }

    // Updates newChildNode's parent pointer
    if (newChildNode != nullptr)
    {
      newChildNode->parent = node->parent;
    }
  }

 public:
  /**
   * Creates an empty `BSTMap`. Runs in O(1).
   */
  BSTMap()
  {
    root = nullptr;
    sz = 0;
    curr = nullptr;
  }

  /**
   * Checks if the `BSTMap` is empty. Runs in O(1).
   */
  bool empty() const
  {
    return sz == 0;
  }

  /**
   * Returns the number of mappings in the `BSTMap`. Runs in O(1).
   */
  size_t size() const
  {
    return sz;
  }

  /**
   * Adds the mapping `{key -> value}` to the `BSTMap`. If the key already
   * exists, does not update the mapping (like the C++ STL map).
   *
   * Uses the key to determine the location in the underlying BST. Creates
   * exactly one new node.
   *
   * Runs in O(H), where H is the height of the tree.
   */
  void insert(KeyT key, ValT value)
  {
    // If the tree is empty, assigns root as a new BSTNode with the values of key and value
    if (root == nullptr)
    {
      root = new BSTNode(key, value, nullptr);
      sz = 1;
      return;
    }
    // Else, iterates through the tree until it hits the end of the correct branch the node should be inserted
    else
    {
      BSTNode* currentNode = root;
      while (true)
      {
        // If the key already exists in the tree, do nothing (duplicate key)
        if (key == currentNode->key)
        {
          return;
        }
        // If the key is less than the currentNode's key, the function moves to currentNode's left node
        else if (key < currentNode->key)
        {
          // If the currentNode's left pointer isn't nullptr, the function continues to the left
          if (currentNode->left != nullptr)
          {
            currentNode = currentNode->left;
          }
          // Else, the function creates a new BSTNode and assigns it to currentNode's left pointer
          else
          {
            currentNode->left = new BSTNode(key, value, currentNode);

            // Increments the size by 1 to account for the new BSTNode created
            sz++;
            return;
          }
        }
        // Else, the function moves to currentNode's right node
        else
        {
          // If the currentNode's right pointer isn't nullptr, the function continues to the right
          if (currentNode->right != nullptr)
          {
            currentNode = currentNode->right;
          }
          // Else, the function creates a new BSTNode and assigns it to currentNode's right pointer
          else
          {
            currentNode->right = new BSTNode(key, value, currentNode);

            // Increments the size by 1 to account for the new BSTNode created
            sz++;
            return;
          }
        }
      }
    }
  }

  /**
   * Return a reference to the value stored for `key` in the map.
   *
   * If key is not present in the map, throw `out_of_range` exception.
   *
   * Runs in O(H), where H is the height of the tree.
   */
  ValT& at(const KeyT& key) const
  {
    BSTNode* node = findBSTNode(root, key);

    if (node == nullptr)
    {
      throw out_of_range("Key not found in BSTMap");
    }
    return node->value;
  }

  /**
   * Returns `true` if the key is present in the map, and false otherwise.
   *
   * Runs in O(H), where H is the height of the tree.
   */
  bool contains(const KeyT& key) const
  {
    return (findBSTNode(root, key) != nullptr);
  }

  /**
   * Empties the `BSTMap`, freeing all memory it controls.
   *
   * Runs in O(N), where N is the number of mappings.
   */
  void clear()
  {
    deleteBST(root);
    root = nullptr;
    sz = 0;
  }

  /**
   * Destructor, cleans up the `BSTMap`.
   *
   * Runs in O(N), where N is the number of mappings.
   */
  ~BSTMap()
  {
    clear();
  }

  /**
   * Converts the `BSTMap` to a string representation, with the values
   * in-order by key.
   *
   * Example:
   *
   * ```c++
   * BSTMap<string, int> names;
   * names.insert("Gwen", 10);
   * names.insert("Jen", 5);
   * names.insert("Ben", 10);
   * names.insert("Sven", 7);
   * ```
   *
   * Calling `names.to_string()` would return the following **multi-line**
   * string:
   *
   * ```text
   * Ben: 10
   * Gwen: 10
   * Jen: 5
   * Sven: 7
   * ```
   *
   * Runs in O(N), where N is the number of mappings.
   */
  string to_string() const
  {
    ostringstream oss;
    printBST(root, oss);
    return oss.str();
  }

  /**
   * Copy constructor.
   *
   * Copies the mappings from the provided `BSTMap`.
   * The internal tree structure must be copied exactly.
   *
   * Runs in O(N), where N is the number of mappings in `other`.
   */
  BSTMap(const BSTMap& other)
  {
    root = nullptr;
    sz = 0;
    curr = nullptr;
    
    if (other.root != nullptr)
    {
      root = copyBST(other.root, nullptr);
      sz = other.sz;
    }
  }

  /**
   * Assignment operator; `operator=`.
   *
   * Clears this tree, and copies the mappings from the provided `BSTMap`.
   * The internal tree structure must be copied exactly.
   *
   * Runs in O(N + O), where N is the number of mappings in `this`, and O is
   * the number of mappings in `other`.
   */
  BSTMap& operator=(const BSTMap& other)
  {
    // Prevents self-assignment
    if (this == &other)
    {
      return *this;
    }

    // Clears the BSTMap before copying from the other BSTMap
    clear();

    // If the BSTMap being copied contains anything, each node is copied over
    // Otherwise, the BSTMap is assigned default values
    if (other.root != nullptr)
    {
      root = copyBST(other.root, nullptr);
      sz = other.sz;
    }
    else
    {
      root = nullptr;
      sz = 0;
    }

    return *this;
  }

  // =========================================================================

  /**
   * Locates the smallest key in the BSTMap, and removes and returns its
   * mapping. If the `BSTMap` is empty, throws a `runtime_error`.
   *
   * Creates no new nodes, and does not update the key or value of any
   * existing nodes.
   *
   * Runs in O(H), where H is the height of the tree.
   */
  pair<KeyT, ValT> remove_min()
  {
    // If the BST is empty, throw a runtime error
    if (root == nullptr)
    {
      throw runtime_error("Error: Cannot use remove_min() on an empty BSTMap");
    }

    BSTNode* parentNode = nullptr;
    BSTNode* node = root;

    // Looks for the smallest node in the BST
    while (node->left != nullptr)
    {
      parentNode = node;
      node = node->left;
    }

    pair<KeyT, ValT> removedNode(node->key, node->value);

    // If the node being deleted has a right node, this block of code is executed
    if (node->right != nullptr)
    {
      // If the tree has only one node / root is the smallest, replaces root with node->right
      if (parentNode == nullptr)
      {
        root = node->right;
        node->right->parent = nullptr;
      }
      else
      {
        // Reattaches the parentNode's left pointer to skip the node being deleted and point to node->right
        parentNode->left = node->right;
        node->right->parent = parentNode;
      }
    }
    else
    {
      if (parentNode == nullptr)
      {
        root = nullptr;
      }
      else
      {
        parentNode->left = nullptr;
      }
    }

    delete node;
    sz--;
    return removedNode;
  }

  /**
   * Checks if the contents of `this` and `other` are equivalent.
   *
   * Two `BSTMap` objects are equivalent if they contain the same
   * mappings from key to value. To be equivalent, the two `BSTMap`
   * objects need not have the same internal tree structure.
   *
   *
   * Two different approaches could be used here.
   *
   * Approach #1:
   * Runs in worst-case O(N) time, using worst-case O(N) heap space,
   * where N is the maximum number of nodes in either of the `BSTMap`
   * objects. Since you cannot include other C++ classes, the extra
   * space in this approach would have to be used with arrays or
   * with `string` objects.
   *
   * Approach #2:
   * Runs in worst-case O(N*H) time, using worst-case O(1) space on
   * the heap, where N is the maximum number of nodes in either of
   * the `BSTMap` objects and H is the largest height between the two
   * `BSTMap` objects.
   *
   */
  bool operator==(const BSTMap& other) const
  {
    // If the sizes of the two trees being compared are not equal, return false immediately
    if (sz != other.sz)
    {
      return false;
    }

    // Creates two copies of the BSTMaps so that begin() and next() can be used to iterate through each BSTMap
    BSTMap<KeyT, ValT> mapA = *this;
    BSTMap<KeyT, ValT> mapB = other;

    mapA.begin();
    mapB.begin();

    KeyT keyA;
    KeyT keyB;
    ValT valA;
    ValT valB;

    // Traverses both BSTMaps at the same time
    while (true)
    {
      bool hasNextNodeA = mapA.next(keyA, valA);
      bool hasNextNodeB = mapB.next(keyB, valB);

      // If one BSTMap finishs traversing before the other, returns false
      if (hasNextNodeA != hasNextNodeB)
      {
        return false;
      }

      // If both are done traversing, returns true
      if (!hasNextNodeA && !hasNextNodeB)
      {
        return true;
      }

      // If a mismatch is found in either the keys or values, returns false
      if ((keyA != keyB) || (valA != valB))
      {
        return false;
      }
    }
  }

  /**
   * Resets internal state for an iterative inorder traversal.
   *
   * See `next` for usage details. Modifies nothing except for `curr`.
   *
   * O(H), where H is the maximum height of the tree.
   */
  void begin()
  {
    curr = findMin(root);
  }

  /**
   * Uses the internal state to return the next in-order key and value
   * by reference, and advances the internal state. Returns `true` if the
   * reference parameters were set, and `false` otherwise.
   *
   * Example usage:
   *
   * ```c++
   * BSTMap<string, int> bst;
   * bst.begin();
   * string key;
   * int value;
   * while (bst.next(key, val)) {
   *   cout << key << ": " << val << endl;
   * }
   * ```
   *
   * Modifies nothing except for `curr`.
   *
   * Runs in worst-case O(H) where H is the height of the tree.
   */
  bool next(KeyT& key, ValT& val)
  {
    // If the current node is null, the function is done traversing
    if (curr == nullptr)
    {
      return false;
    }

    key = curr->key;
    val = curr->value;

    // Moves curr to the next node
    if (curr->right != nullptr)
    {
      curr = findMin(curr->right);
    }
    else
    {
      BSTNode* parentNode = curr->parent;
      while ((parentNode != nullptr) && (curr == parentNode->right))
      {
        curr = parentNode;
        parentNode = parentNode->parent;
      }

      curr = parentNode;
    }

    return true;
  }

  // =========================================================================

  /**
   * Removes the mapping for the given key from the `BSTMap`, and returns the
   * value.
   *
   * Throws `out_of_range` if the key is not present in the map. Creates no new
   * nodes, and does not update the key or value of any existing nodes.
   *
   * Runs in worst-case O(H) where H is the height of the tree.
   */
  ValT erase(const KeyT& key)
  {
    // If the BSTMap is empty, throws an out_of_range error 
    if (root == nullptr)
    {
      throw out_of_range("Error: Cannot use erase() on an empty BSTMap");
    }

    // Finds the node to erase using findBSTNode()
    // If the node is not found, throws an out_of_range error  
    BSTNode* node = findBSTNode(root, key);
    if (node == nullptr)
    {
      throw out_of_range("Error: Key not found in BSTMap");
    }

    // Gets the value of the node being removed
    ValT removedNodeValue = node->value;

    // Case A: node is a leaf node
    if ((node->left == nullptr) && (node->right == nullptr))
    {
      reconnectChildToParent(node, nullptr);
      delete node;
    }
    // Case B: node only has a right child node
    else if (node->left == nullptr)
    {
      reconnectChildToParent(node, node->right);
      delete node;
    }
    // Case C: node only has a left child node
    else if (node->right == nullptr)
    {
      reconnectChildToParent(node, node->left);
      delete node;
    }
    // Case D: node has two child nodes
    else
    {
      // Finds the successor node (minimum node in the right-most subtree)
      BSTNode* successorNode = findMin(node->right);

      // If successorNode isn't the immediate right child, detachs successor from its current parent and connects successorNode's right child to that spot
      if (successorNode->parent != node)
      {
        reconnectChildToParent(successorNode, successorNode->right);

        successorNode->right = node->right;
        if (successorNode->right != nullptr)
        {
          successorNode->right->parent = successorNode;
        }
      }

      successorNode->left = node->left;
      if (successorNode->left != nullptr)
      {
        successorNode->left->parent = successorNode;
      }

      reconnectChildToParent(node, successorNode);

      delete node;
    }

    // Decreases the size of the BSTMap by one to account for the erased node
    sz--;
    return removedNodeValue;
  }

  // =========================================================================

  /**
   * Returns a pointer to the root node of the `BSTMap`. For autograder testing
   * purposes only.
   */
  void* getRoot() const
  {
    return this->root;
  }
};
