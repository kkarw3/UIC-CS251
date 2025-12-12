// Krrish Karwal
// Professor Ayala
// Project 2 - Lyrics DB
// CS 251 - Data Structures
// October 1, 2025

#include "include/lyrics.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>

using namespace std;

string cleanToken(const string& token)
{
  string lowerCaseToken = token;

  for (char &c : lowerCaseToken)
  {
    c = tolower(c);
  }

  int tokenStart = 0;
  while ((tokenStart < lowerCaseToken.size()) && (ispunct(lowerCaseToken[tokenStart])))
  {
    tokenStart++;
  }

  int tokenEnd = lowerCaseToken.size();
  while ((tokenEnd > tokenStart) && (ispunct(lowerCaseToken[tokenEnd - 1])))
  {
    tokenEnd--;
  }

  string lowerCaseTokenTrimmed = lowerCaseToken.substr(tokenStart, tokenEnd - tokenStart);

  bool containsAlpha = false;

  for (char c : lowerCaseTokenTrimmed)
  {
    if (isalpha(c))
    {
      containsAlpha = true;
      break;
    }
  }

  if (!containsAlpha)
  {
    return "";
  }

  return lowerCaseTokenTrimmed;
}

set<string> gatherTokens(const string& text)
{
  // TODO student
  stringstream ss(text);
  string word;
  set<string> gatheredTokens;

  while (ss >> word)
  {
    string cleanedToken = cleanToken(word);
    
    if (!cleanedToken.empty())
    {
      gatheredTokens.insert(cleanedToken);
    }
  }

  return gatheredTokens;
}

int buildIndices(const string& filename, map<string, set<string>>& inverted_index, map<string, string>& artist_index)
{
  ifstream inputFile(filename);

  if (!inputFile.is_open())
  {
    return 0;
  }

  string songTitle;
  string songArtist;
  string songLyrics;

  int songCount = 0;

  while ((getline(inputFile, songTitle)) && (getline(inputFile, songArtist)) && (getline(inputFile, songLyrics)))
  {
    artist_index[songTitle] = songArtist;

    set<string> tokens = gatherTokens(songLyrics);

    for (const string& t : tokens)
    {
      inverted_index[t].insert(songTitle);
    }

    songCount++;
  }

  return songCount;
}

set<string> findQueryMatches(const map<string, set<string>>& index, const string& sentence)
{
  // TODO student
  stringstream ss(sentence);
  string token;

  set<string> result;
  set<string> allSongs;

  for (const auto& [word, songs] : index)
  {
    allSongs.insert(songs.begin(), songs.end());
  }


  bool firstTerm = true;

  while (ss >> token)
  {
    char charModifier = ' ';

    if ((token[0] == '+') || (token[0] == '-'))
    {
      charModifier = token[0];
      token = token.substr(1);
    }

    string cleanedToken = cleanToken(token);
    if (cleanedToken.empty())
    {
      continue;
    }

    auto it = index.find(cleanedToken);
    set<string> currentSearchTerms;

    if (it != index.end())
    {
      currentSearchTerms = it->second;
    }

    if (firstTerm)
    {
      if (charModifier == '-')
      {
        result = allSongs;
        set<string> newResult;

        set_difference(result.begin(), result.end(), currentSearchTerms.begin(), currentSearchTerms.end(), inserter(newResult, newResult.begin()));

        result = newResult;
      }
      else
      {
        result = currentSearchTerms;
      }
      firstTerm = false;
    }
    else
    {
      set<string> newResult;

      if (charModifier == '+')
      {
        // Intersection
        set_intersection(result.begin(), result.end(), currentSearchTerms.begin(), currentSearchTerms.end(), inserter(newResult, newResult.begin()));
      }
      else if (charModifier == '-')
      {
        // Difference
        set_difference(result.begin(), result.end(), currentSearchTerms.begin(), currentSearchTerms.end(), inserter(newResult, newResult.begin()));
      }
      else
      {
        // Union
        set_union(result.begin(), result.end(), currentSearchTerms.begin(), currentSearchTerms.end(), inserter(newResult, newResult.begin()));
      }

      result = newResult;
    }
  }

  return result;
}

void searchEngine(const string& filename)
{
  map<string, set<string>> inverted_index;
  map<string, string> artist_index;

  int songCount = buildIndices(filename, inverted_index, artist_index);

  if (songCount == 0)
  {
    cout << "Invalid filename." << endl;
  }

  cout << "Stand by while building indices..." << endl;

  set<string> uniqueArtists;

  for (const auto& [song, artist] : artist_index)
  {
    uniqueArtists.insert(artist);
  }

  cout << "Indexed " << songCount << " songs containing " << inverted_index.size() << " unique terms and " << uniqueArtists.size() << " artists." << endl;

  while (true)
  {
    string userQuery;
    cout << "Enter query sentence (press enter to quit): ";

    if (!getline(cin, userQuery) || userQuery.empty())
    {
      cout << "Thank you for searching our Lyrics DB!" << endl;
      break;
    }

    set<string> songMatches = findQueryMatches(inverted_index, userQuery);

    if (songMatches.empty())
    {
      cout << "Found 0 matching songs" << endl;
    }
    else
    {
      cout << "Found " << songMatches.size() << " matching songs" << endl;

      vector<string> sortedSongMatches(songMatches.begin(), songMatches.end());
      sort(sortedSongMatches.begin(), sortedSongMatches.end());

      for (const string& songName : sortedSongMatches)
      {
        cout << songName << " by " << artist_index[songName] << endl;
      }
    }
  }
}