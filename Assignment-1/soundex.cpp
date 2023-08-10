/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "../testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include "vector.h"
using namespace std;

/* This function is intended to return a string which
 * contains only the letter characters from the original
 * (all non-letter characters are removed)
 *
 * WARNING: The provided code is buggy!
 *
 * Use unit tests to identify which inputs to this function
 * are incorrectly handled. Then, remove this comment and
 * replace it with a description of the bug you fixed.
 */
string removeNonLetters(string s) {
  string result = charToString(s[0]);
  for (int i = 1; i < s.length(); i++) {
    if (isalpha(s[i])) {
      result += s[i];
    }
  }
  return result;
}

unordered_map<char, int> digit{{'A', '0'}, {'E', '0'}, {'I', '0'}, {'O', '0'},
                               {'U', '0'}, {'H', '0'}, {'W', '0'}, {'Y', '0'},
                               {'B', '1'}, {'F', '1'}, {'P', '1'}, {'V', '1'},
                               {'C', '2'}, {'G', '2'}, {'J', '2'}, {'K', '2'},
                               {'Q', '2'}, {'S', '2'}, {'X', '2'}, {'Z', '2'},
                               {'D', '3'}, {'T', '3'}, {'L', '4'}, {'M', '5'},
                               {'N', '5'}, {'R', '6'}};

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
string soundex(string s) {
  /* TODO: Fill in this function. */
  s = removeNonLetters(s);
  s = toUpperCase(s);
  string tmp = "";
  string ans = "";
  for (int i = 0; i < s.length(); i++) {
    if (i > 0 && digit[s[i - 1]] == digit[s[i]]) continue;
    tmp += digit[s[i]];
  }
  tmp[0] = s[0];
  for (int i = 0; i < tmp.length(); ++i) {
    if (tmp[i] != '0') ans += tmp[i];
  }
  for (int i = ans.length() ; i < 4 ; ++i) {
    ans += '0';
  }
  return ans.substr(0, 4);
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void soundexSearch(string filepath) {
  // The proivded code opens the file with the given name
  // and then reads the lines of that file into a vector.
  ifstream in;
  Vector<string> databaseNames;

  if (openFile(in, filepath)) {
    readEntireFile(in, databaseNames);
  }
  cout << "Read file " << filepath << ", "
       << databaseNames.size() << " names found." << endl;

  // The names in the database are now stored in the provided
  // vector named databaseNames

  /* TODO: Fill in the remainder of this function. */
  unordered_map<string, Vector<string>> map;
  for (auto& name : databaseNames) {
    map[soundex(name)].add(name);
  }
  for (auto it = map.begin() ; it != map.end() ; it++) {
    it->second.sort();
  }
  string inputName;
  while ((inputName = getLine("\nEnter a surname (RETURN to quit): ")) != "") {
    string code = soundex(inputName);
    cout << "Soundex code is " << code << endl;
    cout << "Matches from database: " << map[code] << endl;
  }
  cout << "All done!" << endl;
}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
  string s = "O'Hara";
  string result = removeNonLetters(s);
  EXPECT_EQUAL(result, "OHara");
  s = "Planet9";
  result = removeNonLetters(s);
  EXPECT_EQUAL(result, "Planet");
  s = "tl dr";
  result = removeNonLetters(s);
  EXPECT_EQUAL(result, "tldr");
}

PROVIDED_TEST("Sample inputs from handout") {
  EXPECT_EQUAL(soundex("Curie"), "C600");
  EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
  EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
  EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
  EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
  EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
  EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
  EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
  EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
  EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
  EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
  EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
  // Some versions of Soundex make special case for consecutive codes split by hw
  // We do not make this special case, just treat same as codes split by vowel
  EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// TODO: add your test cases here

STUDENT_TEST("soundexSearch test") {
//  soundexSearch("res/small.txt");
  soundexSearch("res/surnames.txt");
}
