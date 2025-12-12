// Krrish Karwal

#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

using namespace std;

template <typename KeyT, typename ValT>
class HashMap
{
 private:
  struct ChainNode
  {
    const KeyT key;
    ValT value;
    ChainNode* next;

    ChainNode(KeyT key, ValT value) : key(key), value(value), next(nullptr) {}
    ChainNode(KeyT key, ValT value, ChainNode* next) : key(key), value(value), next(next) {}
  };

  ChainNode** data;
  size_t sz;
  size_t capacity;

  // Utility members for begin/next
  ChainNode* curr;
  size_t curr_idx;

  // Rehashes a hashmap with double the capacity
  void rehash(size_t new_capacity)
  {
    // Creates a new hashmap with double the capacity and initalizes it similar to a default hashmap
    ChainNode** new_data = new ChainNode*[new_capacity];
    
    for (size_t i = 0; i < new_capacity; i++)
    {
      new_data[i] = nullptr;
    }

    // Moves existing nodes into the newly-created bucket arrays
    for (size_t i = 0; i < capacity; i++)
    {
      ChainNode* currentNode = data[i];
      while (currentNode != nullptr)
      {
        ChainNode* nextNode = currentNode->next;
        
        // Calculates the new index for each existing node in the old hashmap
        size_t new_index = hash<KeyT>{}(currentNode->key) % new_capacity;

        // Inserts the old node into the new hashmap
        currentNode->next = new_data[new_index];
        new_data[new_index] = currentNode;
        currentNode = nextNode;
      }
    }

    // Deletes the old hashmap and updates the data & capacity values to account for the new hashmap
    delete[] data;
    data = new_data;
    capacity = new_capacity;
  }

 public:
  /**
   * Creates an empty `HashMap` with 10 buckets.
   */
  HashMap()
  {
    sz = 0;
    capacity = 10;
    data = new ChainNode*[capacity];

    for (size_t i = 0; i < capacity; i++)
    {
      data[i] = nullptr;
    }
  }

  /**
   * Creates an empty `HashMap` with `capacity` buckets.
   */
  HashMap(size_t capacity)
  {
    sz = 0;
    this->capacity = capacity;
    data = new ChainNode*[capacity];

    // Initalizes each data value in data to nullptr
    for (size_t i = 0; i < capacity; i++)
    {
      data[i] = nullptr;
    }
  }

  /**
   * Checks if the `HashMap` is empty. Runs in O(1).
   */
  bool empty() const
  {
    return sz == 0;
  }

  /**
   * Returns the number of mappings in the `HashMap`. Runs in O(1).
   */
  size_t size() const
  {
    return sz;
  }

  /**
   * Adds the mapping `{key -> value}` to the `HashMap`. If the key already
   * exists, does not update the mapping (like the C++ STL map).
   *
   * Uses the hash value of the key to determine the location in the
   * underlying hash table. Creates exactly one new node; resizes by doubling
   * when the load factor exceeds 1.5.
   *
   * On resize, doesn't create new nodes, but rearranges existing ones.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  void insert(KeyT key, ValT value)
  {
    // Finds the bucket index for the given key
    size_t index = hash<KeyT>{}(key) % capacity;

    // Iterates through the bucket given by 'index'
    ChainNode* currentNode = data[index];
    while (currentNode != nullptr)
    {
      // If the key is already in the hash table, don't update value
      if (currentNode->key == key)
      {
        return;
      }
      currentNode = currentNode->next;
    }

    // Resizes the hash table when the load factor post-insertion is greater than 1.5
    double newLoadFactor = static_cast<double>(sz + 1) / static_cast<double>(capacity);
    if (newLoadFactor > 1.5)
    {
      rehash(capacity * 2);
      index = hash<KeyT>{}(key) % capacity;
    }

    // Creates a new node with the given key and value
    ChainNode* newChainNode = new ChainNode(key, value, data[index]);
    data[index] = newChainNode;

    // Increments sz by one to account for the newly-added node
    sz++;
  }

  /**
   * Return a reference to the value stored for `key` in the map.
   *
   * If key is not present in the map, throw `out_of_range` exception.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  ValT& at(const KeyT& key) const
  {
    // Finds the bucket index for the given key
    size_t index = hash<KeyT>{}(key) % capacity;

    // Iterates through the bucket given by 'index'
    ChainNode* currentNode = data[index];
    while (currentNode != nullptr)
    {
      // If the key is in the hashmap, returns the key's value
      if (currentNode->key == key)
      {
        return currentNode->value;
      }
      currentNode = currentNode->next;
    }

    // If the given key was not found, throws an error
    throw out_of_range("Error: Given key not found in hashmap");
  }

  /**
   * Returns `true` if the key is present in the map, and false otherwise.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  bool contains(const KeyT& key) const
  {
    // Finds the bucket index for the given key
    size_t index = hash<KeyT>{}(key) % capacity;

    // Iterates through the bucket given by 'index'
    ChainNode* currentNode = data[index];
    while (currentNode != nullptr)
    {
      // If the key is in the hashmap, returns true
      if (currentNode->key == key)
      {
        return true;
      }
      currentNode = currentNode->next;
    }

    // If the key is not in the hashmap, returns false
    return false;
  }

  /**
   * Empties the `HashMap`, freeing all nodes. The bucket array may be left
   * alone.
   *
   * Runs in O(N+B), where N is the number of mappings and B is the number of
   * buckets.
   */
  void clear()
  {
    // For each bucket in the hashmap, delete each value
    for (size_t i = 0; i < capacity; i++)
    {
      // Used to track the current node in the current bucket
      ChainNode* currentNode = data[i];

      while (currentNode != nullptr)
      {
        // Gets the next node in the bucket
        ChainNode* nextNode = currentNode->next;

        // 
        delete currentNode;
        currentNode = nextNode;
      }

      // Sets the current bucket to nullptr after deleting every node in the bucket
      data[i] = nullptr;
    }

    // Resets the hashmap's member values
    sz = 0;
    curr = nullptr;
    curr_idx = 0;
  }

  /**
   * Destructor, cleans up the `HashMap`.
   *
   * Runs in O(N+B), where N is the number of mappings and B is the number of
   * buckets.
   */
  ~HashMap()
  {
    clear();
    delete[] data;
  }

  /**
   * Removes the mapping for the given key from the `HashMap`, and returns the
   * value.
   *
   * Throws `out_of_range` if the key is not present in the map. Creates no new
   * nodes, and does not update the key or value of any existing nodes.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  ValT erase(const KeyT& key)
  {
    // Finds the bucket index for the given key
    size_t index = hash<KeyT>{}(key) % capacity;

    // Iterates through the bucket given by 'index' to look for the given key
    ChainNode* currentNode = data[index];
    ChainNode* previousNode = nullptr;

    while ((currentNode != nullptr) && (!(currentNode->key == key)))
    {
      previousNode = currentNode;
      currentNode = currentNode->next;
    }

    // If the current node is nullptr after iterating, throws an out_of_range error
    if (currentNode == nullptr)
    {
      throw out_of_range("Error: Key to erase was not found in the hashmap");
    }

    // Unlinks the node being erased from the bucket chain
    if (previousNode == nullptr)
    {
      data[index] = currentNode->next;
    }
    else
    {
      previousNode->next = currentNode->next;
    }

    // Gets the value of the node being erased
    ValT value = currentNode->value;
    delete currentNode;

    // Decreases the size by one to account for the erased node
    sz--;

    // Returns the value of the node erased
    return value;
  }

  /**
   * Copy constructor.
   *
   * Copies the mappings from the provided `HashMap`.
   *
   * Runs in O(N+B), where N is the number of mappings in `other`, and B is the
   * number of buckets.
   */
  HashMap(const HashMap& other)
  {
    // Initial values for the hashmap copy
    data = nullptr;
    sz = 0;
    this->capacity = other.capacity;
    curr = nullptr;
    curr_idx = 0;

    data = new ChainNode*[capacity];

    for (size_t i = 0; i < capacity; i++)
    {
      data[i] = nullptr;
    }

    // Copies all data from the hashmap into this hashmap
    for (size_t i = 0; i < other.capacity; i++)
    {
      ChainNode* currentOtherNode = other.data[i];

      while (currentOtherNode != nullptr)
      {
        insert(currentOtherNode->key, currentOtherNode->value);
        currentOtherNode = currentOtherNode->next;
      }
    }
  }

  /**
   * Assignment operator; `operator=`.
   *
   * Clears this table, and copies the mappings from the provided `HashMap`.
   *
   * Runs in O((N1+B1) + (N2+B2)), where N1 and B1 are the number of mappings
   * and buckets in `this`, and N2 and B2 are the number of mappings and buckets
   * in `other`.
   */
  HashMap& operator=(const HashMap& other)
  {
    // If the two hashmaps are the exact same, returns 
    if (this == &other)
    {
      return *this;
    }

    // Clears the hashmap before copying from the other hashmap
    clear();
    delete[] data;

    // Resets the hashmap to default values
    capacity = other.capacity;
    data = new ChainNode*[capacity];

    for (size_t i = 0; i < capacity; i++)
    {
      data[i] = nullptr;
    }

    sz = 0;
    curr = nullptr;
    curr_idx = 0;

    // Copies all data from the hashmap into this hashmap
    for (size_t i = 0; i < other.capacity; i++)
    {
      ChainNode* currentOtherNode = other.data[i];

      while (currentOtherNode != nullptr)
      {
        insert(currentOtherNode->key, currentOtherNode->value);
        currentOtherNode = currentOtherNode->next;
      }
    }

    return *this;
  }

  // =====================

  /**
   *
   * Checks if the contents of `this` and `other` are equivalent.
   *
   * Two `HashMap` objects are equivalent if they contain the same
   * mappings from key to value. To be equivalent, the two `HashMap`
   * objects need not have the elements saved in the same order
   * inside of the buckets.
   *
   * Runs in worst-case O(B*L) time, where B is the maximum number
   * of buckets in either of the `HashMap` objects and L is the length
   * of the  largest chain on any of the buckets.
   *
   */
  bool operator==(const HashMap& other) const
  {
    // If the sizes of the two hashmaps differ, returns false
    if (this->sz != other.sz)
    {
      return false;
    }

    // For each node in this hashmap, check if the other hashmap contains the same node
    for (size_t i = 0; i < capacity; i++)
    {
      ChainNode* currentNode = data[i];

      while (currentNode != nullptr)
      {
        // If the other hashmap doesn't contain the exact same node, returns false
        if (!other.contains(currentNode->key))
        {
          return false;
        }

        // If the other hashmap contains the same key but not the same value, returns false
        if (other.at(currentNode->key) != currentNode->value)
        {
          return false;
        }

        currentNode = currentNode->next;
      }
    }

    // If all nodes match exactly, returns true
    return true;
  }

  /**
   * Resets internal state for an iterative traversal.
   *
   * See `next` for usage details. Modifies nothing except for `curr` and
   * `curr_idx`.
   *
   * Runs in worst-case O(B), where B is the number of buckets.
   */
  void begin()
  {
    // TODO_STUDENT

    // Resets curr and curr_idx
    curr = nullptr;
    curr_idx = 0;

    // Searches for the first non-empty bucket in the hashmap
    while ((curr_idx < capacity) && (data[curr_idx] == nullptr))
    {
      curr_idx++;
    }

    // If curr_idx is within the bounds of the hashmap, sets curr to the first node in the hashmap
    if (curr_idx < capacity)
    {
      curr = data[curr_idx];
    }
  }

  /**
   * Uses the internal state to return the "next" key and value
   * by reference, and advances the internal state. Returns `true` if the
   * reference parameters were set, and `false` otherwise.
   *
   * Example usage:
   *
   * ```c++
   * HashMap<string, int> hm;
   * hm.begin();
   * string key;
   * int value;
   * while (hm.next(key, val)) {
   *   cout << key << ": " << val << endl;
   * }
   * ```
   *
   * Does not visit the mappings in any defined order.
   *
   * Modifies nothing except for `curr` and `curr_idx`.
   *
   * Runs in worst-case O(B) where B is the number of buckets.
   */
  bool next(KeyT& key, ValT& value)
  {
    // TODO_STUDENT

    // If there's no more data in the hashmap, returns false
    if (curr == nullptr)
    {
      return false;
    }

    // Outputs the current node's key/value pair
    key = curr->key;
    value = curr->value;

    // Goes to the next node in the current bucket
    curr = curr->next;

    // If the current bucket has no more nodes, moves to the next bucket
    while ((curr == nullptr) && (++curr_idx < capacity))
    {
      curr = data[curr_idx];
    }

    return true;
  }

  // ===============================================

  /**
   * Returns a pointer to the underlying memory managed by the `HashMap`.
   * For autograder testing purposes only.
   */
  void* get_data() const
  {
    return this->data;
  }

  /**
   * Returns the capacity of the underlying memory managed by the `HashMap`. For
   * autograder testing purposes only.
   */
  size_t get_capacity()
  {
    return this->capacity;
  }
};
