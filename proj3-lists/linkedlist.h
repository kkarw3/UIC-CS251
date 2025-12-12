// Krrish Karwal

#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

template <typename T>
class LinkedList
{
 private:
  class Node
  {
   public:
    T data;
    Node *next;

    Node(T data)
    {
      this->data = data;
      this->next = nullptr;
    }

    Node(T data, Node *next)
    {
      this->data = data;
      this->next = next;
    }
  };

  size_t list_size;
  Node *list_front;

 public:
  /**
   * Default constructor. Creates an empty `LinkedList`.
   */
  LinkedList()
  {
    list_size = 0;
    list_front = nullptr;
  }

  /**
   * Returns whether the `LinkedList` is empty (i.e. whether its
   * size is 0).
   */
  bool empty() const
  {
    return list_size == 0;
  }

  /**
   * Returns the number of elements in the `LinkedList`.
   */
  size_t size() const
  {
    return list_size;
  }

  /**
   * Adds the given `T` to the front of the `LinkedList`.
   */
  void push_front(T data)
  {
    list_front = new Node(data, list_front);
    list_size++;
  }

  /**
   * Adds the given `T` to the back of the `LinkedList`.
   */
  void push_back(T data)
  {
    // Creates a new node with the given data
    Node* newNode = new Node(data);

    // Checks to see if the list is empty before proceeding
    if (empty())
    {
      list_front = newNode;
    }
    else
    {
      // Sets the current Node to the front of the linked list
      Node* currentNode = list_front;

      // Iterates through the linked list until it finds the last Node
      while (currentNode->next != nullptr)
      {
        currentNode = currentNode->next;
      }

      // Sets the last Node's pointer to the new Node
      currentNode->next = newNode;
    }

    // Increases the list size to account for the newly added Node
    list_size++;
  }

  /**
   * Removes the element at the front of the `LinkedList`.
   *
   * If the `LinkedList` is empty, throws a `runtime_error`.
   */
  T pop_front()
  {
    // Checks to see if the list is empty before proceeding
    if (empty())
    {
      throw runtime_error("Error: Cannot use pop_front on an empty list.");
    }

    // Gets the value of the old front Node
    Node* oldFrontNode = list_front;
    T value = oldFrontNode->data;

    // Reassigns the front of the list to the Node pointed to by oldFrontNode 
    list_front = oldFrontNode->next;

    // Pops the front Node
    delete oldFrontNode;

    // Decreases the size of the list since we removed a Node
    list_size--;

    // Returns the old front Node's data value
    return value;
  }

  /**
   * Removes the element at the back of the `LinkedList`.
   *
   * If the `LinkedList` is empty, throws a `runtime_error`.
   */
  T pop_back()
  {
    // Checks to see if the list is empty before proceeding
    if (empty())
    {
      throw runtime_error("Error: Cannot use pop_front on an empty list.");
    }

    // If the list only has one element, it gets the data value of list_front before deleting it.
    // Otherwise, this if statement is skipped over and the function continues.
    if (list_front->next == nullptr)
    {
      T value = list_front->data;
      delete list_front;
      list_front = nullptr;
      list_size--;
      return value;
    }

    // Sets the current Node to the front of the linked list
    Node* currentNode = list_front;

    // Iterates through the linked list until it finds the second-to-last Node
    while (currentNode->next->next != nullptr)
    {
      currentNode = currentNode->next;
    }

    // Gets the value of the last Node
    T value = currentNode->next->data;

    // Deletes the last Node in the list
    delete currentNode->next;

    // Reassigns the last Node in the list 
    currentNode->next = nullptr;

    // Decreases the size of the list since we removed a Node
    list_size--;

    // Returns the value of the deleted Node
    return value;
  }

  /**
   * Empties the `LinkedList`, releasing all allocated memory, and resetting
   * member variables appropriately.
   */
  void clear()
  {
    // Sets the current Node to the front of the linked list
    Node* currentNode = list_front;

    // Iterates through the linked list, deleting each Node
    while (currentNode != nullptr)
    {
      Node* nextNode = currentNode->next;
      delete currentNode;
      currentNode = nextNode;
    }

    // Resets the linked list's values
    list_size = 0;
    list_front = nullptr;
  }

  /**
   * Destructor. Clears all allocated memory.
   */
  ~LinkedList()
  {
    clear();
  }

  /**
   * Returns the element at the given index in the `LinkedList`.
   *
   * If the index is invalid, throws `out_of_range`.
   */
  T &at(size_t index) const
  {
    // Throws an error if the given index is greater than list_size
    if (index >= list_size)
    {
      throw out_of_range("Given index is out of range.");
    }

    // Iterates through the linked list until it reaches the given index
    Node* currentNode = list_front;
    for (size_t i = 0; i < index; i++)
    {
      currentNode = currentNode->next;
    }

    return currentNode->data;
  }

  /**
   * Copy constructor. Creates a deep copy of the given `LinkedList`.
   *
   * Must run in O(N) time.
   */
  LinkedList(const LinkedList &other)
  {
    // Sets LinkedList parameters to default values
    list_size = 0;
    list_front = nullptr;

    /* Creates two Node pointers:
     * currentNode to iterate through the other list
     * list_back to track the last Node in the copy
    */
    Node* currentNode = other.list_front;
    Node* list_back = nullptr;

    // Iterates through other until it reaches the end 
    while (currentNode != nullptr)
    {
      Node* newNode = new Node(currentNode->data);

      /* Checks if list_front points to nullptr before appending a Node:
       * If list_front == nullptr, then it makes newNode the first Node in the copy
       * Otherwise, it appends newNode to the end of the list by making list_back point to newNode
      */
      if (list_front == nullptr)
      {
        list_front = newNode;
      }
      else
      {
        list_back->next = newNode;
      }

      // Moves the back of the list to newNode since newNode is now the last Node in the copy
      list_back = newNode;

      // Increases the copy list's size by one to account for newNode
      list_size++;

      // Moves to the next Node in the other list
      currentNode = currentNode->next;
    }
  }

  /**
   * Assignment operator. Sets the current `LinkedList` to a deep copy of the
   * given `LinkedList`.
   *
   * Must run in O(N) time.
   */
  LinkedList &operator=(const LinkedList &other)
  {
    // If the two LinkedList objects equal each other, exit the function
    if (this == &other)
    { 
      return *this;
    }

    // Deletes any existing Nodes in this list before copying
    clear();

    /* Creates two Node pointers:
     * currentNode to iterate through the other list
     * list_back to track the last Node in the copy
    */
    Node* currentNode = other.list_front;
    Node* list_back = nullptr;

    // Iterates through other until it reaches the end 
    while (currentNode != nullptr)
    {
      Node* newNode = new Node(currentNode->data);

      /* Checks if list_front points to nullptr before appending a Node:
       * If list_front == nullptr, then it makes newNode the first Node in the copy
       * Otherwise, it appends newNode to the end of the list by making list_back point to newNode
      */
      if (list_front == nullptr)
      {
        list_front = newNode;
      }
      else
      {
        list_back->next = newNode;
      }

      // Moves the back of the list to newNode since newNode is now the last Node in the copy
      list_back = newNode;

      // Increases the copy list's size by one to account for newNode
      list_size++;

      // Moves to the next Node in the other list
      currentNode = currentNode->next;
    }

    return *this;
  }

  /**
   * Converts the `LinkedList` to a string. Formatted like `[0, 1, 2, 3, 4]`
   * (without the backticks -- hover the function name to see). Runs in O(N)
   * time.
   */
  string to_string() const
  {
    // Initializes a stringstream to assist in converting a linked list to a string
    stringstream ss;

    ss << "[";

    Node* currentNode = list_front;
    
    // Iterates through the list
    for (size_t i = 0; i < list_size; i++)
    {
      ss << currentNode->data;

      // Adds a comma and a space if it's not the last element in the linked list
      if (i < list_size - 1)
      {
        ss << ", ";
      }

      // Moves to the next Node in the linked list
      currentNode = currentNode->next;
    }

    ss << "]";
    return ss.str();
  }

  /**
   * Searches the `LinkedList` for the first matching element, and returns its
   * index. If no match is found, returns "-1".
   */
  size_t find(const T &data)
  {
    Node* currentNode = list_front;

    size_t index = 0;

    // Iterates through the linked list to find data
    while (currentNode != nullptr)
    {
      // If the current Node's data is found, it returns its index
      if (currentNode->data == data)
      {
        return index;
      }

      // Moves to the next Node and increases the index
      currentNode = currentNode->next;
      index++;
    }

    return -1;
  }

  /**
   * Remove the element at the specified index in this list.
   *
   * If the index is invalid, throws `out_of_range`.
   */
  void remove_at(size_t index)
  {
    // Throws an error if the given index is greater than list_size
    if (index >= list_size)
    {
      throw out_of_range("Given index is out of range.");
    }
    
    // If the given index is 0, calls pop_front to remove the first Node
    if (index == 0)
    {
      pop_front();
      return;
    }

    // Keeps track of the Node before the Node wanted for deletion
    Node* previousNode = list_front;

    // Iterates through the list to the index before the index of the Node wanted for deletion
    for (size_t i = 0; i < index - 1; i++)
    {
      previousNode = previousNode->next;
    }

    // Points to the Node wanted for deletion
    Node* targetNode = previousNode->next;

    // Reassigns previousNode to point to the Node after targetNode
    previousNode->next = targetNode->next;

    // Deletes targetNode
    delete targetNode;

    // Decreases the list size by one to account for the deleted Node
    list_size--;
  }

  /**
   * Inserts the given `T` as a new element in the `LinkedList` after
   * the given index. If the index is invalid, throws `out_of_range`.
   */
  void insert_after(size_t index, T data)
  {
    // Throws an error if the given index is greater than list_size
    if (index >= list_size)
    {
      throw out_of_range("Given index is out of range.");
    }

    Node* newNode = new Node(data);

    if (empty())
    {
      // Makes the new Node the front of the list
      list_front = newNode;
    }
    else
    {
      Node* currentNode = list_front;

      // Iterates through the list until index is reached
      for (size_t i = 0; i < index; i++)
      {
        currentNode = currentNode->next;
      }

      // Inserts newNode between the indexed Node and the Node after
      newNode->next = currentNode->next;
      currentNode->next = newNode;

    }

    // Increases the size of the list by one to account for the new Node
    list_size++;
  }

  /**
   * Remove every element that is currently in an
   * even-numbered position on the list.
   *
   * Must run in O(N).
   *
   * For example, if a list was `[3, 9, 7, 6, 8]`, remove_evens
   * would change the list to `[9, 6]`, since we remove the
   * values in positions 0, 2, and 4 (the even-numbered indices)
   * from the list.
   */
  void remove_evens()
  {
    // If the list is empty, exit the function
    if (list_front == nullptr)
    {
      return;
    }

    // Since the front-most node is at index 0, it must always be removed
    Node* currentNode = list_front->next;
    delete list_front;

    // Reassigns currentNode to the front of the list
    list_front = currentNode;
    list_size--;

    // If the list is now empty after removing the front-most Node, exit the function
    if (list_front == nullptr)
    {
      return;
    }

    Node* previousNode = list_front;
    currentNode = previousNode->next;

    // Iterates through the even indexes of the linked list
    while (currentNode != nullptr)
    {
      // Deletes the even-indexed Node & decreases the list size to account for the deleted Node
      previousNode->next = currentNode->next;
      delete currentNode;
      list_size--;

      // Moves previousNode forward by one (This allows the function to skip over the odd indexs of the original list)
      previousNode = previousNode->next;

      // If the end of the list is reached, break
      if (previousNode == nullptr)
      {
        break;
      }

      // Moves to the next even-indexed Node
      currentNode = previousNode->next;
    }
  }

  /**
   * Returns a pointer to the node at the front of the `LinkedList`. For
   * autograder testing purposes only.
   */
  void *front() const
  {
    return this->list_front;
  }
};