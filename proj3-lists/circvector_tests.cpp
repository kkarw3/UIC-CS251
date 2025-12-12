// Krrish Karwal

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "circvector.h"

using namespace std;
using namespace testing;

TEST(CircVectorCore, InitialEmptyList)
{
  CircVector<int> circVec;
  EXPECT_THAT(circVec.empty(), Eq(true));
}

TEST(CircVectorCore, PushFront)
{
  CircVector<int> circVec;
  circVec.push_front(5);
  circVec.push_front(15);
  circVec.push_front(10);

  EXPECT_THAT(circVec.at(0), Eq(10));
}

TEST(CircVectorCore, PushBack)
{
  CircVector<int> circVec;
  circVec.push_back(5);
  circVec.push_back(10);
  circVec.push_back(15);
  EXPECT_THAT(circVec.at(2), Eq(15));
}

TEST(CircVectorCore, PopFront)
{
  CircVector<int> circVec;
  circVec.push_back(15);
  circVec.push_back(20);

  int value = circVec.pop_front();
  EXPECT_THAT(value, Eq(15));
}

TEST(CircVectorCore, PopBack)
{
  CircVector<int> circVec;
  circVec.push_back(20);
  circVec.push_back(40);

  int value = circVec.pop_back();
  EXPECT_THAT(value, Eq(40));
}

TEST(CircVectorCore, PopBackOneElement)
{
  CircVector<int> circVec;
  circVec.push_back(20);

  int value = circVec.pop_back();

  EXPECT_THAT(value, Eq(20));
  EXPECT_THAT(circVec.empty(), Eq(true));
  EXPECT_THROW(circVec.pop_back(), runtime_error);
}

TEST(CircVectorCore, Clear)
{
  CircVector<int> circVec;
  circVec.push_back(20);
  circVec.push_back(40);
  circVec.push_back(60);
  circVec.push_back(80);
  circVec.push_back(100);

  circVec.clear();
  EXPECT_THAT(circVec.empty(), Eq(true));
}

TEST(CircVectorCore, Resize)
{
  CircVector<int> circVec(1);
  for (int i = 0; i < 100; i++)
  {
    circVec.push_back(i);
  }
  EXPECT_THAT(circVec.size(), Eq(100));

  for (int i = 0; i < 100; i++)
  {
    EXPECT_THAT(circVec.at(i), Eq(i));
  }
}

TEST(CircVectorCore, WrappingWithPopBack)
{
  CircVector<int> circVec(5);
  circVec.push_back(1);
  circVec.push_back(2);
  circVec.push_back(3);
  circVec.push_back(4);
  circVec.push_back(5);
  circVec.pop_front();
  circVec.pop_front();
  circVec.push_back(6);
  circVec.push_back(7);

  int value = circVec.pop_back();
  EXPECT_THAT(value, Eq(7));
  EXPECT_THAT(circVec.size(), Eq(4));
  EXPECT_THAT(circVec.at(0), Eq(3));
  EXPECT_THAT(circVec.at(3), Eq(6));
}

TEST(CircVectorCore, WrappingWithPushBack)
{
  CircVector<int> circVec(5);
  circVec.push_back(10);
  circVec.push_back(20);
  circVec.push_back(30);
  circVec.push_back(40);
  circVec.push_back(50);
  circVec.pop_front();
  circVec.pop_front();
  circVec.push_back(60);
  circVec.push_back(70);

  EXPECT_THAT(circVec.at(0), Eq(30));
  EXPECT_THAT(circVec.at(1), Eq(40));
  EXPECT_THAT(circVec.at(2), Eq(50));
  EXPECT_THAT(circVec.at(3), Eq(60));
}

TEST(CircVectorCore, WrappingWithPopBackAndPushBack)
{
  CircVector<int> circVec(5);
  circVec.push_back(100);
  circVec.push_back(200);
  circVec.push_back(300);
  circVec.push_back(400);
  circVec.pop_front();
  circVec.push_back(500);

  EXPECT_THAT(circVec.at(0), Eq(200));
  EXPECT_THAT(circVec.at(1), Eq(300));
  EXPECT_THAT(circVec.at(2), Eq(400));
  EXPECT_THAT(circVec.at(3), Eq(500));
}

TEST(CircVectorCore, PushFrontResize)
{
  CircVector<int> circVec(2);
  circVec.push_front(1);
  circVec.push_front(2);
  circVec.push_front(3);

  EXPECT_THAT(circVec.size(), Eq(3));
  EXPECT_THAT(circVec.at(0), Eq(3));
  EXPECT_THAT(circVec.at(1), Eq(2));
  EXPECT_THAT(circVec.at(2), Eq(1));
}

TEST(CircVectorCore, ThrowsOutOfRange)
{
  CircVector<int> circVec;
  EXPECT_THROW(circVec.at(0), out_of_range);
}

TEST(CircVectorCore, ThrowsRuntimePopFrontAndBack)
{
  CircVector<int> circVec;
  EXPECT_THROW(circVec.pop_front(), runtime_error);
  EXPECT_THROW(circVec.pop_back(), runtime_error);
}

TEST(CircVectorAugmented, CopyConstructor)
{
  CircVector<int> circVec;
  circVec.push_back(50);
  circVec.push_back(100);
  circVec.push_back(150);

  CircVector<int> copy(circVec);

  EXPECT_THAT(circVec.size(), Eq(3));
  EXPECT_THAT(circVec.at(0), Eq(50));
  EXPECT_THAT(circVec.at(1), Eq(100));
  EXPECT_THAT(circVec.at(2), Eq(150));

  EXPECT_THAT(copy.size(), Eq(3));
  EXPECT_THAT(copy.at(0), Eq(50));
  EXPECT_THAT(copy.at(1), Eq(100));
  EXPECT_THAT(copy.at(2), Eq(150));

  circVec.at(0) = 600;
  EXPECT_THAT(copy.at(0), Eq(50));
}

TEST(CircVectorAugmented, Operator)
{
  CircVector<int> circVec1;
  circVec1.push_back(50);
  circVec1.push_back(100);
  circVec1.push_back(150);

  CircVector<int> circVec2;
  circVec2.push_back(12);
  circVec2.push_back(24);

  circVec1 = circVec2;

  EXPECT_THAT(circVec1.size(), Eq(2));
  EXPECT_THAT(circVec1.at(0), Eq(12));
  EXPECT_THAT(circVec1.at(1), Eq(24));

  circVec2.at(0) = 1000;
  EXPECT_THAT(circVec1.at(0), Eq(12));
}

TEST(CircVectorAugmented, AssignSelf)
{
  CircVector<int> circVec;
  circVec.push_back(6);
  circVec.push_back(12);
  circVec.push_back(18);

  CircVector<int>& circVecRef = circVec;
  circVec = circVecRef;

  EXPECT_THAT(circVec.size(), Eq(3));
  EXPECT_THAT(circVec.at(0), Eq(6));
  EXPECT_THAT(circVec.at(1), Eq(12));
  EXPECT_THAT(circVec.at(2), Eq(18));
}

TEST(CircVectorAugmented, ToString)
{
  CircVector<int> circVec1;
  CircVector<int> circVec2;

  circVec1.push_back(13);
  circVec1.push_back(26);
  circVec1.push_back(39);
  circVec1.push_back(52);

  EXPECT_THAT(circVec1.to_string(), Eq("[13, 26, 39, 52]"));
  EXPECT_THAT(circVec2.to_string(), Eq("[]"));
}

TEST(CircVectorAugmented, Find)
{
  CircVector<int> circVec;

  circVec.push_back(250);
  circVec.push_back(500);
  circVec.push_back(750);
  circVec.push_back(1000);
  circVec.push_back(1250);

  EXPECT_THAT(circVec.find(1000), Eq(3));
  EXPECT_THAT(circVec.find(750), Eq(2));
  EXPECT_THAT(circVec.find(250), Eq(0));
  EXPECT_THAT(circVec.find(1250), Eq(4));
  EXPECT_THAT(circVec.find(500), Eq(1));
  EXPECT_THAT(circVec.find(5000), Eq(-1));
}

TEST(CircVectorAugmented, RemoveAt)
{
  CircVector<int> circVec;

  circVec.push_back(150);
  circVec.push_back(300);
  circVec.push_back(450);
  circVec.push_back(900);
  circVec.push_back(1050);

  circVec.remove_at(1);
  EXPECT_THAT(circVec.at(0), Eq(150));
  EXPECT_THAT(circVec.at(1), Eq(450));
  EXPECT_THAT(circVec.at(2), Eq(900));
  EXPECT_THAT(circVec.at(3), Eq(1050));

  EXPECT_THROW(circVec.remove_at(10), out_of_range);
}

TEST(CircVectorExtras, InsertAfter)
{
  CircVector<int> circVec;

  circVec.push_back(5);
  circVec.push_back(10);
  circVec.push_back(20);
  circVec.push_back(25);

  circVec.insert_after(1, 15);

  EXPECT_THAT(circVec.size(), Eq(5));
  EXPECT_THAT(circVec.at(0), Eq(5));
  EXPECT_THAT(circVec.at(1), Eq(10));
  EXPECT_THAT(circVec.at(2), Eq(15));
  EXPECT_THAT(circVec.at(3), Eq(20));
  EXPECT_THAT(circVec.at(4), Eq(25));

  EXPECT_THROW(circVec.insert_after(9, 45), out_of_range);
}

TEST(CircVectorExtras, InsertAfterZero)
{
  CircVector<int> circVec;

  circVec.push_back(10);
  circVec.push_back(20);

  circVec.insert_after(0, 15);

  EXPECT_THAT(circVec.size(), Eq(3));
  EXPECT_THAT(circVec.at(0), Eq(10));
  EXPECT_THAT(circVec.at(1), Eq(15));
  EXPECT_THAT(circVec.at(2), Eq(20));
}

TEST(CircVectorExtras, InsertAfterResize)
{
  CircVector<int> circVec(3);

  circVec.push_back(50);
  circVec.push_back(75);
  circVec.push_back(125);

  circVec.insert_after(1, 100);

  EXPECT_THAT(circVec.size(), Eq(4));
  EXPECT_THAT(circVec.at(0), Eq(50));
  EXPECT_THAT(circVec.at(1), Eq(75));
  EXPECT_THAT(circVec.at(2), Eq(100));
  EXPECT_THAT(circVec.at(3), Eq(125));
}

TEST(CircVectorExtras, RemoveEvens)
{
  CircVector<int> circVec1;
  CircVector<int> circVec2;

  circVec1.push_back(5);
  circVec1.push_back(10);
  circVec1.push_back(15);
  circVec1.push_back(20);
  circVec1.push_back(25);
  circVec1.push_back(30);
  circVec1.push_back(35);
  circVec1.push_back(40);
  circVec1.push_back(45);
  circVec1.push_back(50);

  circVec1.remove_evens();

  EXPECT_THAT(circVec1.at(0), Eq(10));
  EXPECT_THAT(circVec1.at(1), Eq(20));
  EXPECT_THAT(circVec1.at(2), Eq(30));
  EXPECT_THAT(circVec1.at(3), Eq(40));
  EXPECT_THAT(circVec1.at(4), Eq(50));

  EXPECT_THAT(circVec2.empty(), Eq(true));
  EXPECT_THAT(circVec2.size(), Eq(0));
}

TEST(CircVectorExtras, RemoveEvensSizeUpdate)
{
  CircVector<int> circVec;

  circVec.push_back(100);
  circVec.push_back(200);
  circVec.push_back(300);
  circVec.push_back(400);

  circVec.remove_evens();

  EXPECT_THAT(circVec.size(), Eq(2));
  EXPECT_THAT(circVec.at(0), Eq(200));
  EXPECT_THAT(circVec.at(1), Eq(400));
}