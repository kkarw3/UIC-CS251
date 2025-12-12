// Krrish Karwal

#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

template <typename T>
class CircVector
{
 private:
  T *data;
  size_t vec_size;
  size_t capacity;
  size_t front_idx;

  /**
  * Resizes a vector to have double its capacity when it's full.
  */
  void resize()
  {
    size_t old_capacity = capacity;
    size_t new_capacity = capacity * 2;
    T* new_data = new T[new_capacity];

    for (size_t i = 0; i < vec_size; i++)
    {
      size_t old_idx = (front_idx + i) % old_capacity;
      new_data[i] = data[old_idx];
    }
    delete[] data;

    data = new_data;
    capacity = new_capacity;
    front_idx = 0;
  }

  /**
  * Wraps the vector in a similar method to the one used for rotating ciphers in Project 1.
  */
  size_t wrap(size_t index) const
  {
    return (index + capacity) % capacity;
  }
 public:
  /**
   * Default constructor. Creates an empty `CircVector` with capacity 10.
   */
  CircVector()
  {
    capacity = 10;
    vec_size = 0;
    front_idx = 0;
    data = new T[capacity];
  }

  /**
   * Creates an empty `CircVector` with given capacity. Capacity must exceed 0.
   */
  CircVector(size_t capacity)
  {
    this->capacity = capacity;
    vec_size = 0;
    front_idx = 0;
    data = new T[capacity];
  } 

  /**
   * Returns whether the `CircVector` is empty (i.e. whether its
   * size is 0).
   */
  bool empty() const
  {
    return vec_size == 0;
  }

  /**
   * Returns the number of elements in the `CircVector`.
   */
  size_t size() const
  {
    return vec_size;
  }

  /**
   * Adds the given `T` to the front of the `CircVector`.
   */
  void push_front(T elem)
  {
    // Checks to see if the vector needs resizing before proceeding
    if (vec_size == capacity)
    {
      resize();
    }

    // Shifts front_idx one position to the "left" of the vector & assigns the given element to data's front-most index
    front_idx = wrap(front_idx - 1);
    data[front_idx] = elem;
    
    // Increases the vector size to account for the newly added element
    vec_size++;
  }

  /**
   * Adds the given `T` to the back of the `CircVector`.
   */
  void push_back(T elem)
  {
    // Checks to see if the vector needs resizing before proceeding
    if (vec_size == capacity)
    {
      resize();
    }

    // Finds the last-most index in the vector by wrapping around to the position found by adding the front-most index and the size of the vector
    size_t back_idx = wrap(front_idx + vec_size);
    data[back_idx] = elem;

    // Increases the vector size to account for the newly added element
    vec_size++;
  }

  /**
   * Removes the element at the front of the `CircVector`.
   *
   * If the `CircVector` is empty, throws a `runtime_error`.
   */
  T pop_front()
  {
    // Checks to see if the list is empty before proceeding
    if (empty())
    {
      throw runtime_error("Error: Cannot use pop_front on an empty vector.");
    }

    // Gets the value of the data value at the front-most index
    T value = data[front_idx];

    // Shifts the front-most index once to the right of itself
    front_idx = wrap(front_idx + 1);

    // Decreases the size of the vector since we removed an element
    vec_size--;
    
    // Returns the value given by the old front-most index
    return value;
  }

  /**
   * Removes the element at the back of the `CircVector`.
   *
   * If the `CircVector` is empty, throws a `runtime_error`.
   */
  T pop_back()
  {
    // Checks to see if the list is empty before proceeding
    if (empty())
    {
      throw runtime_error("Error: Cannot use pop_back on an empty vector.");
    }

    // Finds the back-most index based on the front-most index and the size of the vector
    size_t back_idx = wrap(front_idx + vec_size - 1);

    // Gets the value of the data value at the front-most index
    T value = data[back_idx];

    // Decreases the size of the vector since we removed an element
    vec_size--;

    // Returns the value given by the old front-most index
    return value;
  }

  /**
   * Removes all elements from the `CircVector`.
   */
  void clear()
  {
    vec_size = 0;
    front_idx = 0;
  }

  /**
   * Destructor. Clears all allocated memory.
   */
  ~CircVector()
  {
    delete[] data;
  }

  /**
   * Returns the element at the given index in the `CircVector`.
   *
   * If the index is invalid, throws `out_of_range`.
   */
  T &at(size_t index) const
  {
    if (index >= vec_size)
    {
      throw out_of_range("Given index is out of range.");
    }

    // Finds the index using wrap()
    size_t wrappedIndex = wrap(front_idx + index);

    return data[wrappedIndex];
  }

  /**
   * Copy constructor. Creates a deep copy of the given `CircVector`.
   *
   * Must run in O(N) time.
   */
  CircVector(const CircVector &other)
  {
    this->capacity = other.capacity;
    this->vec_size = other.vec_size;
    this->front_idx = 0;
    this->data = new T[capacity];

    for (size_t i = 0; i < vec_size; i++)
    {
      this->data[i] = other.at(i);
    }
  }

  /**
   * Assignment operator. Sets the current `CircVector` to a deep copy of the
   * given `CircVector`.
   *
   * Must run in O(N) time.
   */
  CircVector &operator=(const CircVector &other)
  {
    // If the two CircVector objects equal each other, exit the function
    if (this == &other)
    {
      return *this;
    }

    // Deletes any existing Nodes in this list before copying
    delete[] data;

    this->capacity = other.capacity;
    this->vec_size = other.vec_size;
    this->front_idx = 0;
    this->data = new T[capacity];

    for (size_t i = 0; i < vec_size; i++)
    {
      this->data[i] = other.at(i);
    }

    return *this;
  }

  /**
   * Converts the `CircVector` to a string. Formatted like `[0, 1, 2, 3, 4]`
   * (without the backticks -- hover the function name to see). Runs in O(N)
   * time.
   */
  string to_string() const
  {
    // Initializes a stringstream to assist in converting a vector to a string
    stringstream ss;

    ss << "[";
    
    // Iterates through the vector
    for (size_t i = 0; i < vec_size; i++)
    {
      ss << at(i);

      // Adds a comma and a space if it's not the last element in the vector
      if (i < vec_size - 1)
      {
        ss << ", ";
      }
    }

    ss << "]";
    return ss.str();
  }

  /**
   * Searches the `CircVector` for the first matching element, and returns its
   * index in the `CircVector`. If no match is found, returns "-1".
   */
  size_t find(const T &target)
  {
    size_t index = 0;

    // Iterates through the vector to find data
    while (index < vec_size)
    {
      // If the current Node's data is found, it returns its index
      if (at(index) == target)
      {
        return index;
      }

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
    // Throws an error if the given index is greater than vec_size
    if (index >= vec_size)
    {
      throw out_of_range("Given index is out of range.");
    }

    // Iterates through the list to the index before the index of the data value wanted for deletion
    for (size_t i = index; i < vec_size - 1; i++)
    {
      at(i) = at(i + 1);
    }

    // Decreases the list size by one to account for the deleted Node
    vec_size--;
  }

  /**
   * Inserts the given `T` as a new element in the `CircVector` after
   * the given index. If the index is invalid, throws `out_of_range`.
   */
  void insert_after(size_t index, T elem)
  {
    // Throws an error if the given index is greater than vec_size
    if (index >= vec_size)
    {
      throw out_of_range("Given index is out of range.");
    }

    // Resizes the vector if it's full before proceeding
    if (vec_size == capacity)
    {
      resize();
    }

    // Calculates the actual index to insert after
    size_t insert_idx = wrap(front_idx + index + 1);

    // Move every element one to the right to account for the new data value being inserted
    for (size_t i = vec_size; i > index + 1; i--)
    {
      size_t from_idx = wrap(front_idx + i - 1);
      size_t to_idx = wrap(front_idx + i);
      data[to_idx] = data[from_idx];
    }

    // Inserts the new element at the given index and increases the vector size by one to account for the newly added data value
    data[insert_idx] = elem;
    vec_size++;
  }

  /**
   * Remove every element that is currently in an
   * even-numbered position on the `CircVector`
   * (not the data array).
   *
   * Must run in O(N). Mustn't reallocate the array.
   *
   * For example, if a list was `[3, 9, 7, 6, 8]`, remove_evens
   * would change the list to `[9, 6]`, since we remove the
   * values in positions 0, 2, and 4 (the even-numbered indices)
   * from the list.
   */
  void remove_evens()
  {
    // If the vector is empty, exit the function
    if (vec_size == 0)
    {
      return;
    }

    size_t new_size = 0;

    // Iterates through the vector to remove all even-indexed elements
    for (size_t i = 1; i < vec_size; i += 2)
    {
      size_t from_idx = wrap(front_idx + i);
      size_t to_idx = wrap(front_idx + new_size);

      data[to_idx] = data[from_idx];
      new_size++;
    }

    // Resize the vector to account for the elements removed
    vec_size = new_size;
  }

  /**
   * Returns a pointer to the underlying memory managed by the `CircVec`.
   * For autograder testing purposes only. Do not change.
   */
  T *get_data() const
  {
    return this->data;
  }

  /**
   * Returns the capacity of the underlying memory managed by the `CircVec`. For
   * autograder testing purposes only. Do not change.
   */
  size_t get_capacity() const
  {
    return this->capacity;
  }
};