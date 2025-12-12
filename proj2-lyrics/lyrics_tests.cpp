#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/lyrics.h"

using namespace std;
using namespace testing;


// Tests for the function cleanToken()
TEST(CleanToken, oneLetterLong)
{
  ASSERT_THAT(cleanToken("t"), StrEq("t"));
  ASSERT_THAT(cleanToken("e"), StrEq("e"));
  ASSERT_THAT(cleanToken("x"), StrEq("x"));
  ASSERT_THAT(cleanToken("t"), StrEq("t"));
}

TEST(CleanToken, punctAtBothEnds)
{
  ASSERT_THAT(cleanToken("..............INtEnTions..............."), StrEq("intentions"));
  ASSERT_THAT(cleanToken(".?!EntrOpy?!."), StrEq("entropy"));
  ASSERT_THAT(cleanToken("#$Afterburner^&*:"), StrEq("afterburner"));
}

TEST(CleanToken, punctInMiddle)
{
  ASSERT_THAT(cleanToken("P.I?L!O&T#S"), StrEq("p.i?l!o&t#s"));
  ASSERT_THAT(cleanToken("FR,.EE!.DO.$M"), StrEq("fr,.ee!.do.$m"));
  ASSERT_THAT(cleanToken("P!@er#$io%^&*d"), StrEq("p!@er#$io%^&*d"));
}

TEST(CleanToken, punctAtStartAndMiddle)
{
  ASSERT_THAT(cleanToken(".><?A%^C%@#@TOR"), StrEq("a%^c%@#@tor"));
  ASSERT_THAT(cleanToken(",!@#R$O)B#$%!@#OT"), StrEq("r$o)b#$%!@#ot"));
  ASSERT_THAT(cleanToken("@!#V$%#@OI(&$CES"), StrEq("v$%#@oi(&$ces"));
}

TEST(CleanToken, punctAtMiddleAndEnd)
{
  ASSERT_THAT(cleanToken("TR$%^&Ees^~#$%^"), StrEq("tr$%^&ees"));
  ASSERT_THAT(cleanToken("ri(@#$&De&$@!>"), StrEq("ri(@#$&de"));
  ASSERT_THAT(cleanToken("TA@#$Ll$%^@#$y(@%^&..*)"), StrEq("ta@#$ll$%^@#$y"));
}

TEST(GatherTokens, leadingSpaces)
{
  string inputText = "        run it back";
  set<string> expectedResult = {"run", "it", "back"};

  EXPECT_THAT(gatherTokens(inputText), ContainerEq(expectedResult))
      << "text=\"" << inputText << "\"";
}

TEST(GatherTokens, trailingSpaces)
{
  string inputText = "Not dead yet        ";
  set<string> expectedResult = {"not", "dead", "yet"};

  EXPECT_THAT(gatherTokens(inputText), ContainerEq(expectedResult))
      << "text=\"" << inputText << "\"";
}

TEST(GatherToken, multipleSpaces)
{
  string inputText = "Tour   de   Force";
  set<string> expectedResult = {"tour", "de", "force"};

  EXPECT_THAT(gatherTokens(inputText), ContainerEq(expectedResult))
      << "text=\"" << inputText << "\"";
}

TEST(BuildIndices, tinyTxt)
{
  map<string, set<string>> inverted_index;
  map<string, string> artist_index;

  int songCount = buildIndices("data/tiny.txt", inverted_index, artist_index);

  EXPECT_THAT(songCount, Eq(4));

  map<string, string> expectedArtistIndex =
  {
    {"Jingle bells", "James Pierpont"},
    {"Brother John", "Unknown"},
    {"Baa baa Black Sheep", "Unknown"},
    {"Carol of the bells", "Mykola Leontovych"}
  };

  EXPECT_THAT(artist_index, ContainerEq(expectedArtistIndex));

  EXPECT_THAT(inverted_index["jingle"], ContainerEq(set<string>{"Jingle bells"}));
  EXPECT_THAT(inverted_index["bells"], ContainerEq(set<string>{"Brother John", "Jingle bells", "Carol of the bells"}));
  EXPECT_THAT(inverted_index["you"], ContainerEq(set<string>{"Brother John", "Baa baa Black Sheep"}));
  EXPECT_THAT(inverted_index["sir"], ContainerEq(set<string>{"Baa baa Black Sheep"}));
  EXPECT_THAT(inverted_index["sweet"], ContainerEq(set<string>{"Carol of the bells"}));
}

TEST(BuildIndices, fileNotFound)
{
  map<string, set<string>> inverted_index;
  map<string, string> artist_index;

  int songCount = buildIndices("data/nonexistent.txt", inverted_index, artist_index);

  EXPECT_THAT(songCount, Eq(0));
  EXPECT_THAT(inverted_index.empty(), Eq(true));
  EXPECT_THAT(artist_index.empty(), Eq(true));
}

TEST(FindQueryMatches, singleWordFound)
{
  map<string, set<string>> index =
  {
    {"love", {"Hello", "Imagine"}},
    {"time", {"Yesterday"}}
  };

  auto result = findQueryMatches(index, "love");
  EXPECT_THAT(result, ContainerEq(set<string>({"Hello", "Imagine"})));
}

TEST(FindQueryMatches, intersectionOfSearchTerms)
{
  map<string, set<string>> index =
  {
    {"night", {"Shallow", "Skyfall"}},
    {"day", {"Skyfall", "Viva La Vida"}}
  };

  auto result = findQueryMatches(index, "night +day");
  EXPECT_THAT(result, ContainerEq(set<string>({"Skyfall"})));
}

TEST(FindQueryMatches, differenceOfSearchTerms)
{
  map<string, set<string>> index =
  {
    {"eyes", {"Rolling in the Deep", "Chandelier"}},
    {"heart", {"Chandelier"}}
  };

  auto result = findQueryMatches(index, "eyes -heart");
  EXPECT_THAT(result, ContainerEq(set<string>({"Rolling in the Deep"})));
}


TEST(FindQueryMatches, unionOfSearchTerms)
{
  map<string, set<string>> index =
  {
    {"love", {"Hallelujah"}},
    {"time", {"Bohemian Rhapsody"}}
  };

  auto result = findQueryMatches(index, "love time");
  EXPECT_THAT(result, ContainerEq(set<string>({"Hallelujah", "Bohemian Rhapsody"})));
}

TEST(FindQueryMatches, firstTermNotInIndex)
{
  map<string, set<string>> index =
  {
    {"love", {"Poker Face"}},
    {"bad", {"Poker Face", "Shape of You"}}
  };

  auto result = findQueryMatches(index, "ghost +bad");
  EXPECT_THAT(result, ContainerEq(set<string>()));
}

TEST(FindQueryMatches, laterTermPlusNotInIndex)
{
  map<string, set<string>> index =
  {
    {"sun", {"Here Comes the Sun", "Walking on Sunshine"}},
    {"moon", {"Moonlight Sonata"}}
  };

  auto result = findQueryMatches(index, "sun +star");
  EXPECT_THAT(result, ContainerEq(set<string>()));
}

TEST(FindQueryMatches, laterTermUnmodifiedNotInIndex)
{
  map<string, set<string>> index = 
  {
    {"rain", {"November Rain", "Set Fire to the Rain"}},
    {"storm", {"Riders on the Storm"}}
  };
    
  auto result = findQueryMatches(index, "rain hail");
  EXPECT_THAT(result, ContainerEq(set<string>({"November Rain", "Set Fire to the Rain"})));
}

TEST(FindQueryMatches, laterTermMinusNotInIndex)
{
  map<string, set<string>> index = 
  {
    {"love", {"Billie Jean", "Smells Like Teen Spirit"}},
    {"night", {"Smells Like Teen Spirit"}}
  };

  auto result = findQueryMatches(index, "love -phantom night");
  EXPECT_THAT(result, ContainerEq(set<string>({"Billie Jean", "Smells Like Teen Spirit"})));
}