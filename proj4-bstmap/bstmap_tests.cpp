// Krrish Karwal

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <random>

#include "bstmap.h"

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

TEST(BSTMapCore, ConstructorEmpty)
{
  BSTMap<int, int> bst;
  EXPECT_THAT(bst.empty(), Eq(true));
  EXPECT_THAT(bst.size(), Eq(0));
}

TEST(BSTMapCore, CopyConstructor)
{
  BSTMap<int, string> bstOne;
  bstOne.insert(10, "ten");
  bstOne.insert(20, "twenty");
  bstOne.insert(30, "thirty");
  
  BSTMap<int, string> bstTwo = bstOne;
  EXPECT_THAT(bstTwo.contains(10), Eq(true));
  EXPECT_THAT(bstTwo.at(30), Eq("thirty"));

  bstOne.insert(40, "forty");
  EXPECT_THAT(bstTwo.contains(40), Eq(false));
}

TEST(BSTMapCore, CopyEmptyConstructor)
{
  BSTMap<int, int> bstOne;
  BSTMap<int, int> bstTwo = bstOne;
  EXPECT_THAT(bstTwo.empty(), Eq(true));
  EXPECT_THAT(bstTwo.size(), Eq(0));
}

TEST(BSTMapCore, EmptyReturnsFalseAfterInserting)
{
  BSTMap<int, int> bst;
  bst.insert(10, 100);

  EXPECT_THAT(bst.empty(), Eq(false));
  EXPECT_THAT(bst.size(), Eq(1));
}

TEST(BSTMapCore, SizeIgnoresDuplicates)
{
  BSTMap<int, int> bst;
  EXPECT_THAT(bst.size(), Eq(0));
  bst.insert(2, 20);
  EXPECT_THAT(bst.size(), Eq(1));
  bst.insert(4, 40);
  EXPECT_THAT(bst.size(), Eq(2));
  bst.insert(2, 60);
  EXPECT_THAT(bst.size(), Eq(2));
}

TEST(BSTMapCore, InsertContains)
{
  BSTMap<int, string> bst;
  bst.insert(1, "one");
  bst.insert(3, "three");
  bst.insert(5, "five");

  EXPECT_THAT(bst.contains(1), Eq(true));
  EXPECT_THAT(bst.contains(2), Eq(false));
}

TEST(BSTMapCore, NoOverwritingOnInsertingDuplicates)
{
  BSTMap<int, string> bst;
  bst.insert(1, "one");
  bst.insert(3, "three");
  bst.insert(5, "five");

  EXPECT_THAT(bst.at(1), Eq("one"));
  EXPECT_THAT(bst.at(5), Eq("five"));
}

TEST(BSTMapCore, AtThrowsForAnyMissingKeys)
{
  BSTMap<int, string> bst;
  bst.insert(1, "one");
  bst.insert(3, "three");
  bst.insert(5, "five");

  EXPECT_THROW(bst.at(34), out_of_range);
}

TEST(BSTMapCore, ClearEmptiesAndResizes)
{
  BSTMap<int, int> bst;
  bst.insert(1, 10);
  bst.insert(3, 30);
  bst.insert(2, 20);
  bst.clear();

  EXPECT_THAT(bst.empty(), Eq(true));
  EXPECT_THAT(bst.size(), Eq(0));
}

string map_to_string(const map<int, int>& m)
{
  ostringstream ss;
  for (const auto& [key, value] : m)
  {
      ss << key << ": " << value << endl;
  }
  return ss.str();
}


TEST(BSTMapCore, ToString)
{
    vector<pair<int,int>> entries
    {
      {3,30},
      {1,10},
      {4,40},
      {2,20}
    };

    BSTMap<int,int> bst;
    map<int,int> referenceMap;

    for (auto [key, value] : entries)
    {
      bst.insert(key, value);
      referenceMap[key] = value;
    }

    EXPECT_THAT(bst.to_string(), StrEq(map_to_string(referenceMap)));
}

TEST(BSTMapCore, AssignCopiesContentsAndSize)
{
  BSTMap<int, string> bstOne;
  BSTMap<int, string> bstTwo;

  bstOne.insert(12, "twelve");
  bstOne.insert(24, "twenty-four");

  bstTwo = bstOne;

  EXPECT_THAT(bstTwo.empty(), Eq(false));
  EXPECT_THAT(bstTwo.size(), Eq(2));
  EXPECT_THAT(bstTwo.contains(24), Eq(true));
  EXPECT_THAT(bstTwo.at(12), Eq("twelve"));
}

TEST(BSTMapCore, AssignEmptyClearsTarget)
{
  BSTMap<int, int> bst;
  BSTMap<int, int> emptyBSTMap;
  bst.insert(1, 100);
  bst.insert(2, 200);
  bst = emptyBSTMap;

  EXPECT_THAT(bst.empty(), Eq(true));
  EXPECT_THAT(bst.size(), Eq(0));
}

TEST(BSTMapCore, SelfAssignmentPrevention)
{
  BSTMap<int, int> bst;
  bst.insert(1, 10);
  bst.insert(2, 20);
  size_t oldMapSize = bst.size();

  BSTMap<int, int>* ptr = &bst;
  *ptr = bst;

  EXPECT_THAT(bst.size(), Eq(oldMapSize));
  EXPECT_THAT(bst.at(1), Eq(10));
  EXPECT_THAT(bst.at(2), Eq(20));
}

TEST(BSTMapCore, AssignmentKeepsSize)
{
  BSTMap<int, int> bstOne;
  BSTMap<int, int> bstTwo;
  bstOne.insert(5, 50);
  bstOne.insert(2, 20);
  bstOne.insert(8, 80);
  bstOne.insert(1, 10);
  bstOne.insert(3, 30);
  bstTwo = bstOne;

  EXPECT_THAT(bstTwo.size(), Eq(bstOne.size()));
  EXPECT_THAT(bstTwo.at(5), Eq(50));
  EXPECT_THAT(bstTwo.at(2), Eq(20));
  EXPECT_THAT(bstTwo.at(8), Eq(80));
  EXPECT_THAT(bstTwo.at(1), Eq(10));
  EXPECT_THAT(bstTwo.at(3), Eq(30));
}

TEST(BSTMapAugmented, RemoveMin)
{
  BSTMap<int, string> bst;

  bst.insert(0, "zero");
  bst.insert(1, "one");
  bst.insert(3, "three");
  bst.insert(5, "five");
  bst.insert(7, "seven");
  bst.insert(9, "nine");

  size_t oldSize = bst.size();
  auto minimumPair = bst.remove_min();

  EXPECT_THAT(minimumPair.first, Eq(0));
  EXPECT_THAT(minimumPair.second, Eq("zero"));
  EXPECT_THAT(bst.contains(0), Eq(false));
  EXPECT_THAT(bst.size(), Eq(oldSize - 1));
}

TEST(BSTMapAugmented, RemoveMinKeepsChildOfMin)
{
  BSTMap<int, int> bst;
  bst.insert(2, 20);
  bst.insert(4, 40);
  bst.insert(1, 10);
  bst.insert(0, 0);
  bst.insert(6, 60);
  bst.insert(0, 1);
  bst.insert(1, 11);
  bst.insert(5, 50);
  bst.insert(3, 30);

  auto removedMin = bst.remove_min();
  EXPECT_THAT(removedMin.first, Eq(0));
  EXPECT_THAT(bst.contains(1), Eq(true));
  EXPECT_THAT(bst.at(1), Eq(10));
}


TEST(BSTMapAugmented, RemoveMinThrowsOnEmptyBSTMap)
{
  BSTMap<int, int> bst;
  EXPECT_THROW(bst.remove_min(), runtime_error);
}

TEST(BSTMapAugmented, RemoveMinKeepsStructure)
{
  BSTMap<int, int> bst;

  bst.insert(1, 10);
  bst.insert(3, 30);
  bst.insert(5, 50);
  bst.insert(7, 70);
  bst.insert(9, 90);

  bst.remove_min();

  EXPECT_THAT(bst.contains(1), Eq(false));
  EXPECT_THAT(bst.contains(3), Eq(true));
  EXPECT_THAT(bst.at(3), Eq(30));
  EXPECT_THAT(bst.at(5), Eq(50));
  EXPECT_THAT(bst.at(7), Eq(70));
  EXPECT_THAT(bst.at(9), Eq(90));
}

TEST(BSTMapAugmented, EquivalencyOperatorReturnsTrue)
{
  BSTMap<int, string> bstOne;
  BSTMap<int, string> bstTwo;

  bstOne.insert(1, "one");
  bstOne.insert(2, "two");
  bstOne.insert(3, "three");
  bstOne.insert(4, "four");
  bstOne.insert(5, "five");

  bstTwo.insert(1, "one");
  bstTwo.insert(2, "two");
  bstTwo.insert(3, "three");
  bstTwo.insert(4, "four");
  bstTwo.insert(5, "five");

  EXPECT_TRUE(bstOne == bstTwo);
}

TEST(BSTMapAugmented, EquivalencyOperatorReturnsFalse)
{
  BSTMap<int, string> bstOne;
  BSTMap<int, string> bstTwo;

  bstOne.insert(1, "one");
  bstOne.insert(2, "two");
  bstOne.insert(3, "three");
  bstOne.insert(4, "four");
  bstOne.insert(5, "five");

  bstTwo.insert(5, "five");
  bstTwo.insert(10, "ten");
  bstTwo.insert(15, "fifteen");
  bstTwo.insert(20, "twenty");

  EXPECT_FALSE(bstOne == bstTwo);
}

TEST(BSTMapAugmented, EquivalencyOperatorReturnsFalseWhenSizesDiffer)
{
  BSTMap<int, string> bstOne;
  BSTMap<int, string> bstTwo;

  bstOne.insert(1, "one");
  bstOne.insert(2, "two");
  bstOne.insert(3, "three");

  bstTwo.insert(1, "one");
  bstTwo.insert(2, "two");

  EXPECT_FALSE(bstOne == bstTwo);
  EXPECT_FALSE(bstTwo == bstOne);
}

TEST(BSTMapAugmented, EquivalencyOperatorReturnsFalseWhenValuesDiffer)
{
  BSTMap<int, string> bstOne;
  BSTMap<int, string> bstTwo;

  bstOne.insert(1, "one");
  bstOne.insert(2, "two");
  bstOne.insert(3, "three");

  bstTwo.insert(1, "one");
  bstTwo.insert(2, "two");
  bstTwo.insert(3, "thREE");

  EXPECT_FALSE(bstOne == bstTwo);
}


TEST(BSTMapAugmented, BeginNextIteratesProperly)
{
  BSTMap<int, string> bst;

  bst.insert(3, "three");
  bst.insert(6, "six");
  bst.insert(7, "seven");
  bst.insert(1, "one");
  bst.insert(4, "four");
  bst.insert(10, "ten");
  bst.insert(2, "two");
  bst.insert(5, "five");
  bst.insert(9, "nine");
  bst.insert(8, "eight");

  vector<int> expectedResult = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  vector<int> actualResult;

  bst.begin();
  int key;
  string value;

  while (bst.next(key, value))
  {
    actualResult.push_back(key);
  }

  EXPECT_THAT(actualResult, Eq(expectedResult));
}

TEST(BSTMapAugmented, BeginNextEmpty)
{
  BSTMap<int, int> bst;
  bst.begin();
  int key;
  int value;

  EXPECT_FALSE(bst.next(key, value));
}

TEST(BSTMapAugmented, NextReturnsFalseAfterTraversal)
{
  BSTMap<int, int> bst;
  bst.insert(2, 20);
  bst.insert(4, 40);
  bst.insert(6, 60);

  bst.begin();
  int key;
  int value;
  vector<int> keys;
  while (bst.next(key, value))
  {
    keys.push_back(key);
  }

  vector<int> expectedKeys = {2, 4, 6};
  EXPECT_THAT(bst.next(key, value), Eq(false));
  EXPECT_THAT(keys, Eq(expectedKeys));
}

TEST(BSTMapAugmented, BeginNextRandomizerTest)
{
  Random::seed(64);

  for (int i = 0; i < 20; i++)
  {
    BSTMap<int, int> bst;
    vector<int> keys;

    for (int j = 0; j < 6; j++)
    {
      int key = Random::randInt(30);

      if (!bst.contains(key))
      {
        bst.insert(key, key * 10);
        keys.push_back(key);
      }
    }

    sort(keys.begin(), keys.end());

    vector<int> traversalVec;
    int key;
    int value;

    bst.begin();
    while (bst.next(key, value))
    {
      traversalVec.push_back(key);
    }
    
    EXPECT_THAT(traversalVec, Eq(keys));
  }
}

TEST(BSTMapErase, EraseThrowsOnMissingKey)
{
  BSTMap<int, int> bst;
  bst.insert(1, 10);
  bst.insert(2, 20);
  bst.insert(3, 30);

  EXPECT_THROW(bst.erase(50), out_of_range);
}

TEST(BSTMapErase, ErasesLeaf)
{
  BSTMap<int, int> bst;
  bst.insert(4, 40);
  bst.insert(8, 80);
  bst.insert(12, 120);

  bst.erase(4);

  EXPECT_THAT(bst.contains(3), Eq(false));
  EXPECT_THAT(bst.size(), Eq(2));
  EXPECT_THAT(bst.at(12), Eq(120));
  EXPECT_THAT(bst.at(8), Eq(80));
}

TEST(BSTMapErase, ErasesRoot)
{
  BSTMap<int, int> bst;
  bst.insert(12, 120);

  bst.erase(12);

  EXPECT_THAT(bst.empty(), Eq(true));
  EXPECT_THAT(bst.size(), Eq(0));
}

TEST(BSTMapErase, ErasesRootWithOneChildNode)
{
  BSTMap<int, int> bst;
  bst.insert(5, 50);
  bst.insert(10, 100);

  bst.erase(5);

  EXPECT_THAT(bst.contains(5), Eq(false));
  EXPECT_THAT(bst.contains(10), Eq(true));
  EXPECT_THAT(bst.size(), Eq(1));
}

TEST(BSTMapErase, ErasesNodeWithOneChildNode)
{
  BSTMap<int, int> bst;
  bst.insert(4, 40);
  bst.insert(2, 20);
  bst.insert(1, 10);


  bst.erase(2);

  EXPECT_THAT(bst.contains(2), Eq(false));
  EXPECT_THAT(bst.contains(1), Eq(true));
  EXPECT_THAT(bst.at(4), Eq(40));
  EXPECT_THAT(bst.at(1), Eq(10));
  EXPECT_THAT(bst.size(), Eq(2));
}

TEST(BSTMapErase, ErasesNodeWithRightChildSuccessor)
{
  BSTMap<int, string> bst;
  bst.insert(50, "fifty");
  bst.insert(30, "thirty");
  bst.insert(70, "seventy");
  bst.insert(60, "sixty");
  bst.insert(80, "eighty");

  bst.erase(70);

  EXPECT_THAT(bst.contains(70), Eq(false));
  EXPECT_THAT(bst.contains(60), Eq(true));
  EXPECT_THAT(bst.contains(80), Eq(true));
  EXPECT_THAT(bst.at(80), Eq("eighty"));
  EXPECT_THAT(bst.at(60), Eq("sixty"));
  EXPECT_THAT(bst.size(), Eq(4));
}

TEST(BSTMapErase, ErasesNodeWithTwoGenerations)
{
  BSTMap<int, string> bst;
  bst.insert(5, "five");
  bst.insert(2, "two");
  bst.insert(8, "eight");
  bst.insert(6, "six");
  bst.insert(9, "nine");
  bst.insert(7, "seven");
  
  bst.erase(5);

  EXPECT_THAT(bst.contains(5), Eq(false));
  EXPECT_THAT(bst.contains(6), Eq(true));
  EXPECT_THAT(bst.contains(7), Eq(true));
  EXPECT_THAT(bst.at(8), Eq("eight"));
  EXPECT_THAT(bst.size(), Eq(5));
}

TEST(BSTMapErase, ErasesNodeWithLeftChildSuccessor)
{
  BSTMap<int, string> bst;
  bst.insert(10, "ten");
  bst.insert(5, "five");
  bst.insert(15, "fifteen");
  bst.insert(12, "twelve");
  bst.insert(20, "twenty");
  bst.insert(11, "eleven");

  bst.erase(10);

  EXPECT_THAT(bst.contains(10), Eq(false));
  EXPECT_THAT(bst.contains(15), Eq(true));
  EXPECT_THAT(bst.contains(11), Eq(true));
  EXPECT_THAT(bst.contains(20), Eq(true));
  EXPECT_THAT(bst.at(11), Eq("eleven"));
  EXPECT_THAT(bst.size(), Eq(5));
}

template <typename T>
void shuffle_vec(vector<T>& vec)
{
  for (int i = vec.size() - 1; i >= 1; i--)
  {
    size_t j = Random::randInt(i - 1);
    swap(vec.at(i), vec.at(j));
  }
}

TEST(BSTMapErase, EraseRandomizerTest)
{
  Random::seed(42);

  for (int i = 0; i < 10; i++)
  {
    BSTMap<int, string> bst;
    map<int, string> referenceMap;

    for (int j = 0; j < 12; j++)
    {
      int key = Random::randInt(50);
      if (!bst.contains(key))
      {
        string value = to_string(key);
        bst.insert(key, value);
        referenceMap[key] = value;
      }
    }

    vector<int> keys;
    for (auto& [key, value] : referenceMap)
    {
      keys.push_back(key);
    }

    shuffle_vec(keys);

    int eraseCount = keys.size() / 2;
    for (int j = 0; j < eraseCount; j++)
    {
      int keyToErase = keys[j];
      if (bst.contains(keyToErase))
      {
        bst.erase(keyToErase);
        referenceMap.erase(keyToErase);
      }
    }

    for (int j = 0; j < eraseCount; j++)
    {
      int erasedKey = keys[j];
      EXPECT_FALSE(bst.contains(erasedKey));
    }

    for (auto& [key, value] : referenceMap)
    {
      ASSERT_TRUE(bst.contains(key));
      EXPECT_THAT(bst.at(key), Eq(value));
    }

    EXPECT_THAT(bst.size(), Eq(referenceMap.size()));
  }
}

}  // namespace