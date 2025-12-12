// Krrish Karwal

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "linkedlist.h"

using namespace std;
using namespace testing;

TEST(LinkedListCore, InitialEmptyList)
{
  LinkedList<int> list;
  EXPECT_THAT(list.empty(), Eq(true));
}

TEST(LinkedListCore, PushFront)
{
  LinkedList<int> list;
  list.push_front(5);
  list.push_front(10);
  EXPECT_THAT(list.at(0), Eq(10));
}

TEST(LinkedListCore, PushBack)
{
  LinkedList<int> list;
  list.push_back(5);
  list.push_back(10);
  EXPECT_THAT(list.at(1), Eq(10));
}

TEST(LinkedListCore, PopFront)
{
  LinkedList<int> list;
  list.push_back(15);
  list.push_back(20);

  int value = list.pop_front();
  EXPECT_THAT(value, Eq(15));
}

TEST(LinkedListCore, PopBack)
{
  LinkedList<int> list;
  list.push_back(15);
  list.push_back(20);

  int value = list.pop_back();
  EXPECT_THAT(value, Eq(20));
}

TEST(LinkedListCore, PopBackOneElement)
{
  LinkedList<int> list;
  list.push_back(55);

  int value = list.pop_back();
  EXPECT_THAT(value, Eq(55));
  EXPECT_THAT(list.empty(), Eq(true));

  EXPECT_THROW(list.pop_back(), runtime_error);
}

TEST(LinkedListCore, Clear)
{
  LinkedList<int> list;
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);
  list.push_back(40);
  list.push_back(50);

  list.clear();
  EXPECT_THAT(list.empty(), Eq(true));
}

TEST(LinkedListCore, ThrowsOutOfRange)
{
  LinkedList<int> list;
  EXPECT_THROW(list.at(0), out_of_range);
}

TEST(LinkedListCore, ThrowsRuntimePopFrontAndBack)
{
  LinkedList<int> list;
  EXPECT_THROW(list.pop_front(), runtime_error);
  EXPECT_THROW(list.pop_back(), runtime_error);
}

TEST(LinkedListAugmented, CopyConstructor)
{
  LinkedList<int> list;
  list.push_back(20);
  list.push_back(30);
  list.push_back(50);

  LinkedList<int> copy(list);

  EXPECT_THAT(copy.size(), Eq(3));
  EXPECT_THAT(copy.at(0), Eq(20));
  EXPECT_THAT(copy.at(1), Eq(30));
  EXPECT_THAT(copy.at(2), Eq(50));

  list.at(0) = 100;
  EXPECT_THAT(copy.at(0), Eq(20));
}

TEST(LinkedListAugmented, Operator)
{
  LinkedList<int> list1;
  list1.push_back(1);
  list1.push_back(2);
  list1.push_back(3);

  LinkedList<int> list2;
  list2.push_back(12);
  list2.push_back(24);

  list1 = list2;

  EXPECT_THAT(list1.size(), Eq(2));
  EXPECT_THAT(list1.at(0), Eq(12));
  EXPECT_THAT(list1.at(1), Eq(24));

  list2.at(0) = 65;
  EXPECT_THAT(list1.at(0), Eq(12));
}

TEST(LinkedListAugmented, AssignSelf)
{
  LinkedList<int> list;
  list.push_back(3);
  list.push_back(6);
  list.push_back(9);

  LinkedList<int>& listRef = list;
  list = listRef;

  EXPECT_THAT(list.size(), Eq(3));
  EXPECT_THAT(list.at(0), Eq(3));
  EXPECT_THAT(list.at(1), Eq(6));
  EXPECT_THAT(list.at(2), Eq(9));
}

TEST(LinkedListAugmented, AssignEmpty)
{
  LinkedList<int> list1;
  LinkedList<int> list2;

  list2.push_back(25);
  list2.push_back(50);

  list2 = list1;

  EXPECT_THAT(list2.empty(), Eq(true));
  EXPECT_THAT(list2.size(), Eq(0));
}

TEST(LinkedListAugmented, CopyEmpty)
{
  LinkedList<int> list;
  LinkedList<int> copy(list);

  EXPECT_THAT(copy.empty(), Eq(true));
  EXPECT_THAT(copy.size(), Eq(0));
}

TEST(LinkedListAugmented, ToString)
{
  LinkedList<int> list1;
  LinkedList<int> list2;

  list1.push_back(11);
  list1.push_back(22);
  list1.push_back(33);

  EXPECT_THAT(list1.to_string(), Eq("[11, 22, 33]"));
  EXPECT_THAT(list2.to_string(), Eq("[]"));
}

TEST(LinkedListAugmented, Find)
{
  LinkedList<int> list;

  list.push_back(9);
  list.push_back(18);
  list.push_back(27);
  list.push_back(36);
  list.push_back(45);

  EXPECT_THAT(list.find(18), Eq(1));
  EXPECT_THAT(list.find(45), Eq(4));
  EXPECT_THAT(list.find(9), Eq(0));
  EXPECT_THAT(list.find(27), Eq(2));
  EXPECT_THAT(list.find(36), Eq(3));
  EXPECT_THAT(list.find(999), Eq(-1));
}

TEST(LinkedListAugmented, RemoveAt)
{
  LinkedList<int> list;

  list.push_back(9);
  list.push_back(18);
  list.push_back(27);
  list.push_back(36);
  list.push_back(45);

  list.remove_at(2);
  EXPECT_THAT(list.at(0), Eq(9));
  EXPECT_THAT(list.at(1), Eq(18));
  EXPECT_THAT(list.at(2), Eq(36));
  EXPECT_THAT(list.at(3), Eq(45));

  EXPECT_THROW(list.remove_at(5), out_of_range);
}

TEST(LinkedListExtras, InsertAfter)
{
  LinkedList<int> list;

  list.push_back(5);
  list.push_back(10);
  list.push_back(20);
  list.push_back(25);

  list.insert_after(1, 15);

  EXPECT_THAT(list.size(), Eq(5));
  EXPECT_THAT(list.at(0), Eq(5));
  EXPECT_THAT(list.at(1), Eq(10));
  EXPECT_THAT(list.at(2), Eq(15));
  EXPECT_THAT(list.at(3), Eq(20));
  EXPECT_THAT(list.at(4), Eq(25));

  EXPECT_THROW(list.insert_after(9, 45), out_of_range);
}

TEST(LinkedListExtras, InsertAfterZero)
{
  LinkedList<int> list;

  list.push_back(10);
  list.push_back(20);

  list.insert_after(0, 15);

  EXPECT_THAT(list.size(), Eq(3));
  EXPECT_THAT(list.at(0), Eq(10));
  EXPECT_THAT(list.at(1), Eq(15));
  EXPECT_THAT(list.at(2), Eq(20));
}

TEST(LinkedListExtras, RemoveEvens)
{
  LinkedList<int> list;

  list.push_back(5);
  list.push_back(10);
  list.push_back(15);
  list.push_back(20);
  list.push_back(25);
  list.push_back(30);
  list.push_back(35);
  list.push_back(40);
  list.push_back(45);
  list.push_back(50);

  list.remove_evens();

  EXPECT_THAT(list.at(0), Eq(10));
  EXPECT_THAT(list.at(1), Eq(20));
  EXPECT_THAT(list.at(2), Eq(30));
  EXPECT_THAT(list.at(3), Eq(40));
  EXPECT_THAT(list.at(4), Eq(50));
}

TEST(LinkedListExtras, RemoveEvensEmpty)
{
  LinkedList<int> list;

  list.remove_evens();

  EXPECT_THAT(list.empty(), Eq(true));
  EXPECT_THAT(list.size(), Eq(0));
}