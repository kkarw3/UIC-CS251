// Krrish Karwal

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <random>

#include "hashmap.h"

using namespace std;
using namespace testing;

namespace
{

class Random
{
 private:
  static mt19937 rng;

 public:
  static void seed(int s)
  {
    Random::rng.seed(s);
  }

  /**
   * Generate a random integer in the range 0 (inclusive) to `max` (exclusive)
   */
  static int randInt(int max)
  {
    // Even though mt19937 is standardized, STL distributions aren't!
    // So, unfortunately, even though this is biased, we're forced to
    // do something like this. Technically uint32_fast_t isn't...
    // totally consistent across platforms? But within reason it works.
    return rng() % (max + 1);
  }
};

std::mt19937 Random::rng;

TEST(HashMapCore, DefaultConstructor)
{
  HashMap<int, int> map;
  
  EXPECT_THAT(map.size(), Eq(0));
  EXPECT_THAT(map.empty(), Eq(true));
}

TEST(HashMapCore, SizeUpdatesAfterInsert)
{
  HashMap<int, int> map;

  map.insert(1, 10);
  map.insert(2, 20);
  map.insert(3, 30);

  EXPECT_THAT(map.size(), Eq(3));
  EXPECT_THAT(map.empty(), Eq(false));
}

TEST(HashMapCore, ReinsertDoesntChangeMap)
{
  HashMap<int, int> map;

  map.insert(5, 10);
  map.insert(5, 15);

  EXPECT_THAT(map.size(), Eq(1));
  EXPECT_THAT(map.empty(), Eq(false));
  EXPECT_THAT(map.at(5), Eq(10));
}

TEST(HashMapCore, InsertAndAt)
{
  HashMap<int, int> map;

  map.insert(1, 100);
  map.insert(11, 200);

  EXPECT_THAT(map.size(), Eq(2));
  EXPECT_THAT(map.contains(1), Eq(true));
  EXPECT_THAT(map.contains(11), Eq(true));
  EXPECT_THAT(map.at(1), Eq(100));
  EXPECT_THAT(map.at(11), Eq(200));
}

TEST(HashMapCore, Erase)
{
  HashMap<int, int> map;

  map.insert(1, 100);
  map.insert(11, 200);

  size_t oldSize = map.size();
  int erasedVal = map.erase(11);

  EXPECT_THAT(erasedVal, Eq(200));
  EXPECT_THAT(map.size(), Eq(oldSize - 1));
  EXPECT_THAT(map.contains(1), Eq(true));
  EXPECT_THAT(map.contains(11), Eq(false));
  EXPECT_THAT(map.at(1), Eq(100));
}

TEST(HashMapCore, AtAndEraseThrowsForMissingKey)
{
  HashMap<int, int> map;

  EXPECT_THROW(map.at(1), out_of_range);

  map.insert(1, 10);

  EXPECT_THROW(map.at(5), out_of_range);
  EXPECT_THROW(map.erase(5), out_of_range);
}

TEST(HashMapCore, ClearAndSizeReset)
{
  HashMap<int, int> map;

  map.insert(1, 10);
  map.insert(2, 20);
  map.insert(3, 30);

  ASSERT_THAT(map.size(), Eq(3));
  ASSERT_THAT(map.empty(), Eq(false));

  map.clear();

  EXPECT_THAT(map.size(), Eq(0));
  EXPECT_THAT(map.empty(), Eq(true));

  EXPECT_THAT(map.contains(1), Eq(false));
  EXPECT_THAT(map.contains(2), Eq(false));
  EXPECT_THAT(map.contains(3), Eq(false));

  map.insert(20, 200);

  EXPECT_THAT(map.size(), Eq(1));
  EXPECT_THAT(map.contains(20), Eq(true));
  EXPECT_THAT(map.at(20), Eq(200));
}

TEST(HashMapCore, CopyConstructor)
{
  HashMap<int, int> map;

  map.insert(1, 10);
  map.insert(11, 20);

  HashMap<int, int> copyMap(map);

  EXPECT_THAT(copyMap.size(), Eq(map.size()));
  EXPECT_FALSE(copyMap.empty());

  EXPECT_THAT(copyMap.contains(1), Eq(true));
  EXPECT_THAT(copyMap.contains(11), Eq(true));
  EXPECT_THAT(copyMap.at(1), Eq(10));
  EXPECT_THAT(copyMap.at(11), Eq(20));
}

TEST(HashMapCore, CopyConstructorEmptyMap)
{
  HashMap<int, int> emptyMap;
  HashMap<int, int> copyMap(emptyMap);

  EXPECT_THAT(copyMap.size(), Eq(0));
  EXPECT_THAT(copyMap.empty(), Eq(true));
}

TEST(HashMapCore, AssignmentOperator)
{
  HashMap<int, int> mapOne;
  mapOne.insert(1, 10);
  mapOne.insert(11, 20);

  HashMap<int, int> mapTwo;
  mapTwo.insert(100, 1000);

  mapTwo = mapOne;

  EXPECT_THAT(mapTwo.size(), Eq(mapOne.size()));
  EXPECT_FALSE(mapTwo.empty());

  EXPECT_TRUE(mapTwo.contains(1));
  EXPECT_TRUE(mapTwo.contains(11));
  EXPECT_THAT(mapTwo.at(1), Eq(10));
  EXPECT_THAT(mapTwo.at(11), Eq(20));
}

TEST(HashMapCore, AssignmentOperatorEmptyMap)
{
  HashMap<int, int> mapOne;
  HashMap<int, int> mapTwo;

  mapOne.insert(1, 10);
  mapOne = mapTwo;

  EXPECT_THAT(mapOne.size(), Eq(0));
  EXPECT_THAT(mapOne.empty(), Eq(true));
}

TEST(HashMapCore, PreventSelfAssignment)
{
  HashMap<int, int> map;

  map.insert(1, 10);

  EXPECT_NO_THROW(map = map);

  EXPECT_THAT(map.size(), Eq(1));
  EXPECT_THAT(map.empty(), Eq(false));
  EXPECT_THAT(map.contains(1), Eq(true));
  EXPECT_THAT(map.at(1), Eq(10));
}

TEST(HashMapAugmented, EquivalentOperator)
{
  HashMap<int, int> mapOne;
  HashMap<int, int> mapTwo;

  mapOne.insert(1, 10);
  mapOne.insert(2, 20);
  mapTwo.insert(1, 10);
  mapTwo.insert(2, 20);

  EXPECT_THAT(mapOne == mapTwo, Eq(true));
}

TEST(HashMapAugmented, EquivalentOperatorDiffSizes)
{
  HashMap<int, int> mapOne;
  HashMap<int, int> mapTwo;

  mapOne.insert(1, 10);
  mapOne.insert(2, 20);
  mapTwo.insert(2, 20);

  EXPECT_THAT(mapOne == mapTwo, Eq(false));
}

TEST(HashMapAugmented, EquivalentOperatorDiffValues)
{
  HashMap<int, int> mapOne;
  HashMap<int, int> mapTwo;

  mapOne.insert(1, 10);
  mapTwo.insert(1, 50);

  EXPECT_THAT(mapOne == mapTwo, Eq(false));
}

TEST(HashMapAugmented, EquivalentOperatorMissingKey)
{
  HashMap<int, int> mapOne;
  HashMap<int, int> mapTwo;

  mapOne.insert(5, 50);
  mapOne.insert(7, 70);

  mapTwo.insert(5, 50);

  EXPECT_THAT(mapOne == mapTwo, Eq(false));
}

TEST(HashMapAugmented, EquivalentOperatorSizeCheck)
{
  HashMap<int, int> mapOne;
  HashMap<int, int> mapTwo;

  mapOne.insert(3, 30);
  mapTwo.insert(3, 30);

  mapTwo.insert(5, 50);

  EXPECT_THAT(mapOne == mapTwo, Eq(false));
}

TEST(HashMapAugmented, BeginNextWithOneNode)
{
  HashMap<int, int> map;

  map.insert(5, 50);
  
  map.begin();

  int key;
  int value;

  EXPECT_THAT(map.next(key, value), Eq(true));
  EXPECT_THAT(key, Eq(5));
  EXPECT_THAT(value, Eq(50));
  EXPECT_THAT(map.next(key, value), Eq(false));
}

TEST(HashMapAugmented, BeginNextWithMultipleNodes)
{
  HashMap<int, int> map;

  for (int i = 0; i < 20; i++)
  {
    map.insert(i, i * 10);
  }

  map.begin();

  set<pair<int, int>> seen;

  int key;
  int value;

  while (map.next(key, value))
  {
    seen.insert({key, value});
  }

  EXPECT_THAT(seen.size(), Eq(20));

  for (int i = 0; i < 20; i++)
  {
    EXPECT_THAT(seen.count({i, i * 10}), Eq(true));
  }
}

TEST(HashMapAugmented, BeginNextCollision)
{
  HashMap<int, int> map;

  map.insert(0, 100);
  map.insert(10, 200);
  map.insert(20, 300);
  map.insert(30, 400);

  map.begin();

  set<pair<int, int>> seen;

  int key;
  int value;

  while (map.next(key, value))
  {
    seen.insert({key, value});
  }

  EXPECT_THAT(seen.size(), Eq(4));
  EXPECT_THAT(seen.count({0,100}), Eq(true));
  EXPECT_THAT(seen.count({10,200}), Eq(true));
  EXPECT_THAT(seen.count({20,300}), Eq(true));
  EXPECT_THAT(seen.count({30,400}), Eq(true));
}

TEST(HashMapAugmented, BeginNextResizing)
{
  HashMap<int, int> map;

  for (int i = 0; i < 20; i++)
  {
    map.insert(i, i + 1);
  }


  map.begin();

  set<pair<int, int>> seen;

  int key;
  int value;

  while (map.next(key, value))
  {
    seen.insert({key, value});
  }

  EXPECT_THAT(seen.size(), Eq(20));

  for (int i = 0; i < 20; i++)
  {
    EXPECT_TRUE(seen.count({i, i + 1}));
  }
}

TEST(HashMapAugmented, BeginNextStops)
{
  HashMap<int,int> map;

  for (int i = 0; i < 5; i++)
  {
    map.insert(i, i * 2);
  }

  map.begin();

  int key;
  int value;

  int count = 0;

  while (map.next(key, value))
  {
    count++;
  }

  EXPECT_THAT(count, Eq(5));

  EXPECT_THAT(map.next(key, value), Eq(false));
}

TEST(HashMapAugmented, BeginNextRepeatedCalls)
{
  HashMap<int,int> map;

  map.begin();

  int key;
  int value;

  EXPECT_THAT(map.next(key, value), Eq(false));
  EXPECT_THAT(map.next(key, value), Eq(false));
  EXPECT_THAT(map.next(key, value), Eq(false));
}

}  // namespace