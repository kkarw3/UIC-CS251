// Krrish Karwal
// Professor Ayala
// Project 1 - Ciphers
// CS 251 - Data Structures
// September 17, 2025

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "include/caesar_dec.h"
#include "include/caesar_enc.h"
#include "include/subst_dec.h"
#include "include/subst_enc.h"
#include "utils.h"

using namespace std;

// Initialize random number generator in .cpp file for ODR reasons
std::mt19937 Random::rng;

const string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// Function declarations go at the top of the file so we can call them
// anywhere in our program, such as in main or in other functions.
// Most other function declarations are in the included header
// files.

// When you add a new helper function, make sure to declare it up here!

/**
 * Print instructions for using the program.
 */
void printMenu();
vector<string> loadFile(const string& fileName);
QuadgramScorer initQuadgramScorer(const string& fileName);
void decryptSubstCipherFile(const QuadgramScorer& scorer);

int main()
{
  // Loads the words from dictionary.txt into a vector of strings 
  vector<string> dict = loadFile("dictionary.txt");

  // Loads the quadGrams text file and initializes a quadGrams object named 'scorer'
  QuadgramScorer scorer = initQuadgramScorer("english_quadgrams.txt");

  Random::seed(time(NULL));
  string command;

  cout << "Welcome to Ciphers!" << endl;
  cout << "-------------------" << endl;
  cout << endl;

  do
  {
    printMenu();
    cout << endl << "Enter a command (case does not matter): ";

    // Use getline for all user input to avoid needing to handle
    // input buffer issues relating to using both >> and getline
    getline(cin, command);
    cout << endl;

    if (command == "R" || command == "r")
    {
      string seed_str;
      cout << "Enter a non-negative integer to seed the random number "
              "generator: ";
      getline(cin, seed_str);
      Random::seed(stoi(seed_str));
    }
    else if (command == "C" || command == "c")
    {
      caesarEncryptCommand();
    }
    else if (command == "D" || command == "d")
    {
      caesarDecryptCommand(dict);
    }
    else if (command == "A" || command == "a")
    {
      applyRandSubstCipherCommand();
    }
    else if (command == "E" || command == "e")
    {
      computeEnglishnessCommand(scorer);
    }
    else if (command == "S" || command == "s")
    {
      decryptSubstCipherCommand(scorer);
    }
    else if (command == "F" || command == "f")
    {
      decryptSubstCipherFile(scorer);
    }

    cout << endl;
  }
  while (!(command == "x" || command == "X") && !cin.eof());

  return 0;
}

void printMenu()
{
  cout << "Ciphers Menu" << endl;
  cout << "------------" << endl;
  cout << "C - Encrypt with Caesar Cipher" << endl;
  cout << "D - Decrypt Caesar Cipher" << endl;
  cout << "E - Compute English-ness Score" << endl;
  cout << "A - Apply Random Substitution Cipher" << endl;
  cout << "S - Decrypt Substitution Cipher from Console" << endl;
  cout << "F - Decrypt Substitution Cipher from File" << endl;
  cout << "R - Set Random Seed for Testing" << endl;
  cout << "X - Exit Program" << endl;
}

// Loads a given dictionary file into a vector of strings
vector<string> loadFile(const string& fileName)
{
  vector<string> dict;
  ifstream infile(fileName);

  string word;
  while (infile >> word)
  {
    dict.push_back(word);
  }

  infile.close();
  return dict;
}

// Helper function to help initialize a quadGram object
QuadgramScorer initQuadgramScorer(const string& fileName)
{
  vector<string> quadGrams;
  vector<int> quadGramsCounts;

  ifstream inputFile(fileName);
  string line;

  while (getline(inputFile, line))
  {
    string quad;
    stringstream ss(line);
    string quadGramsCountsStr;

    if (getline(ss, quad, ',') && getline(ss, quadGramsCountsStr))
    {
      quadGrams.push_back(quad);
      quadGramsCounts.push_back(stoi(quadGramsCountsStr));
    }
  }

  inputFile.close();

  return QuadgramScorer(quadGrams, quadGramsCounts);
}
// "#pragma region" and "#pragma endregion" group related functions in this file
// to tell VSCode that these are "foldable". You might have noticed the little
// down arrow next to functions or loops, and that you can click it to collapse
// those bodies. This lets us do the same thing for arbitrary chunks!
#pragma region CaesarEnc

char rot(char c, int amount)
{

  char newLetter;

  int numChar = ALPHABET.find(c);
  numChar = (numChar + amount) % ALPHABET.size();

  newLetter = ALPHABET[numChar];
  return newLetter;
}

string rot(const string& line, int amount)
{
  // TODO: student
  string result = "";

  for (char c : line)
  {
    if (isalpha(c))
    {
      char upperCase = toupper(c);
      char rotChar = rot(upperCase, amount);
      result += rotChar;
    }
    else if (c == ' ')
    {
      result += " ";
    }
  }

  return result;
}

// Encrypts a given string using rot()
void caesarEncryptCommand()
{
  // TODO: student
  string inputString;
  int rotateAmount;
  string rotateAmountString;
  string encryptedString;

  cout << "Enter the text to encrypt:";
  getline(cin, inputString);

  cout << "Enter the number of characters to rotate by: ";
  getline(cin, rotateAmountString);
  rotateAmount = stoi(rotateAmountString);

  encryptedString = rot(inputString, rotateAmount);
  cout << "Encrypted text: " << encryptedString << endl;
}

#pragma endregion CaesarEnc

#pragma region CaesarDec

void rot(vector<string>& strings, int amount)
{
  for (string& s: strings)
  {
    s = rot(s, amount);
  }
}

string clean(const string& s)
{
  string result;

  for (char c : s)
  {
    if (isalpha(c))
    {
      char upperC = toupper(c);
      result += upperC;
    }
  }

  return result;
}

vector<string> splitBySpaces(const string& s)
{
  vector<string> result;

  stringstream ss(s);
  string word;

  while (ss >> word)
  {
    result.push_back(word);
  }

  return result;
}

string joinWithSpaces(const vector<string>& words)
{
  // TODO: student
  string result;

  for (int i = 0; i < words.size(); i++)
  {
    if (i > 0)
    {
      result += " ";
    }
    result += words.at(i);
  }

  return result;
}

int numWordsIn(const vector<string>& words, const vector<string>& dict)
{
  int numWords = 0;

  // Loops through and compares each word in vector 'words' to the dictionary to see if it's in the dictionary
  for (int i = 0; i < words.size(); i++)
  {
    for (int j = 0; j < dict.size(); j++)
    {
      if (words[i] == dict[j])
      {
        numWords++;
      }
    }
  }

  return numWords;
}

void caesarDecryptCommand(const vector<string>& dict)
{
  // TODO: student
  string userInput;
  vector<string> inputWords;
  bool decryptionsFound = false;

  cout << "Enter the text to decrypt:";
  getline(cin, userInput);

  inputWords = splitBySpaces(userInput);

  for (int i = 0; i < inputWords.size(); i++)
  {
    inputWords.at(i) = clean(inputWords.at(i));
  }

  // Loops through and brute forces all 26 possible Caesar shifts
  for (int i = 0; i < 26; i++)
  {
    vector<string> inputWordsShifted;

    for (int j = 0; j < inputWords.size(); j++)
    {
      inputWordsShifted.push_back(rot(inputWords[j], i));
    }

    // Counts up how many words in the inputWordsShifted vector match with the dictionary
    int numWordsMatched = numWordsIn(inputWordsShifted, dict);

    if (numWordsMatched * 2 > inputWordsShifted.size())
    {
      cout << joinWithSpaces(inputWordsShifted) << endl;
      decryptionsFound = true;
    }
  }

  if (!decryptionsFound)
  {
    cout << "No good decryptions found" << endl;
  }
}

#pragma endregion CaesarDec

#pragma region SubstEnc

string applySubstCipher(const vector<char>& cipher, const string& s)
{
  string result;

  for (char c : s)
  {
    if (isalpha(c))
    {
      char upperC = toupper(c);
      int letterIndex = upperC - 'A';
      result += cipher[letterIndex];
    }
    else
    {
      result += c;
    }
  }

  return result;
}

void applyRandSubstCipherCommand()
{
  string stringInput;
  getline(cin, stringInput);

  vector<char> cipher = genRandomSubstCipher();
  string encryptedStringInput = applySubstCipher(cipher, stringInput);

  cout << encryptedStringInput << endl;
}

#pragma endregion SubstEnc

#pragma region SubstDec

double scoreString(const QuadgramScorer& scorer, const string& s)
{
  double score = 0.0;

  for (int i = 0; i + 4 <= s.size(); i++)
  {
    string quadStringCluster = s.substr(i, 4);
    score += scorer.getScore(quadStringCluster);
  }

  return score;
}

void computeEnglishnessCommand(const QuadgramScorer& scorer)
{
  cout << "Enter a string for scoring:";

  string stringInput;
  getline(cin, stringInput);

  stringInput = clean(stringInput);

  if (stringInput.size() < 4)
  {
    cout << "Englishness score: " << 0.0 << endl;
    return;
  }

  double score = scoreString(scorer, stringInput);
  cout << "Englishness score: " << score << endl;
}

vector<char> hillClimb(const QuadgramScorer& scorer, const string& ciphertext)
{
  // TODO: student

  vector<char> bestCaseCipher = genRandomSubstCipher();
  string bestCaseDecryption = applySubstCipher(bestCaseCipher, ciphertext);
  double bestCaseScore = scoreString(scorer, clean(bestCaseDecryption));

  int decryptionFails = 0;

  while (decryptionFails < 1000)
  {
    vector<char> secondCipher = bestCaseCipher;
    int a = Random::randInt(25);
    int b = Random::randInt(25);

    while (a == b)
    {
      b = Random::randInt(25);
    }

    char temp = secondCipher[a];
    secondCipher[a] = secondCipher[b];
    secondCipher[b] = temp;

    string randomDecryption = applySubstCipher(secondCipher, ciphertext);
    double newScore = scoreString(scorer, clean(randomDecryption));

    if (newScore > bestCaseScore)
    {
      bestCaseScore = newScore;
      bestCaseCipher = secondCipher;
      decryptionFails = 0;
    }
    else
    {
      decryptionFails++;
    }

  }

  return bestCaseCipher;
}

vector<char> decryptSubstCipher(const QuadgramScorer& scorer, const string& ciphertext)
{
  vector<char> bestCipher = hillClimb(scorer, ciphertext);
  string bestDecryption = applySubstCipher(bestCipher, ciphertext);
  double bestScore = scoreString(scorer, clean(bestDecryption));

  for (int i = 1; i < 25; i++)
  {
    vector<char> cipher = hillClimb(scorer, ciphertext);
    string decryption = applySubstCipher(cipher, ciphertext);
    double decryptionScore = scoreString(scorer, clean(decryption));

    if (decryptionScore > bestScore)
    {
      bestScore = decryptionScore;
      bestCipher = cipher;
      bestDecryption = decryption;
    }
  }

  return bestCipher;
}

void decryptSubstCipherCommand(const QuadgramScorer& scorer)
{
  // TODO: student
  string userCiphertext;
  getline(cin, userCiphertext);

  vector<char> cipher = decryptSubstCipher(scorer, userCiphertext);
  string plaintext = applySubstCipher(cipher, userCiphertext);

  cout << plaintext << endl;
}

#pragma endregion SubstDec

void decryptSubstCipherFile(const QuadgramScorer& scorer)
{
  string inputFileName;
  string outputFileName;

  cout << "Enter input filename: ";
  getline(cin, inputFileName);

  cout << "Enter output filename: ";
  getline(cin, outputFileName);

  ifstream infile(inputFileName);

  if (!infile.is_open())
  {
    cout << "Couldn't open file " << inputFileName << endl;
    return;
  }

  string line;
  string cipherFileText;

  while (getline(infile, line))
  {
    cipherFileText += line + "\n";
  }
  infile.close();

  vector<char> cipher = decryptSubstCipher(scorer, cipherFileText);
  string plaintext = applySubstCipher(cipher, cipherFileText);

  ofstream outfile(outputFileName);
  if (!outfile.is_open())
  {
    cout << "Couldn't open file " << outputFileName << endl;
    return;
  }

  outfile << plaintext;
  outfile.close();

  cout << "Decryption complete. Output written to " << outputFileName << endl;
}