// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
string cleanToken(string s)
{
    int start = 0, end = 0;
    while (end < s.length() && ispunct(s[end])) end++;
    s = s.erase(start, end);
    start = end = s.length() - 1;
    while (end > 0 && ispunct(s[end])) end--;
    s = s.erase(end + 1, start - end);
    bool flag = false;
    for (int i = 0 ; i < s.length() && !flag; ++i) {
        if (isalpha(s[i])) {
            flag = true;
        }
    }
    return flag ? toLowerCase(s) : "";
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> gatherTokens(string text)
{
    Set<string> tokens;
    Vector<string> words = stringSplit(text, " ");
    for (auto& word : words) {
        string token = cleanToken(word);
        if (token != "")
            tokens.add(token);
    }
    return tokens;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
int buildIndex(string dbfile, Map<string, Set<string>>& index)
{
    ifstream in;

    if (!openFile(in, dbfile))
        error("Cannot open file named " + dbfile);

    Vector<string> lines;
    int count = 0;
    readEntireFile(in, lines);
    for (int i = 0 ; i < lines.size() ; i = i + 2) {
        string website = lines[i];
        string words = lines[i + 1];
        count++;
        Set<string> tokens = gatherTokens(words);
        for (auto& token : tokens) {
            if (!index.containsKey(token)) {
                Set<string> set;
                set.add(website);
                index.put(token, set);
            }else {
                auto set = index.get(token);
                set.add(website);
                index.put(token, set);
            }
        }
    }
    return count;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    Set<string> result;
    // TODO: your code here
    Vector<string> words = stringSplit(query, " ");
    for (auto &word : words) {
        if (word[0] == '+') {
            word = cleanToken(word);
            result.intersect(index.get(word));
        }else if(word[0] == '-') {
            word = cleanToken(word);
            result.difference(index.get(word));
        }else {
            word = cleanToken(word);
            result.unionWith(index.get(word));
        }
    }
    cout << index << endl;
    return result;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
void searchEngine(string dbfile)
{
    // TODO: your code here
    Map<string, Set<string>> index;
    buildIndex(dbfile, index);
    string query;
    while(true) {
        cin >> query;
        if(query == "") break;
        Set<string> result = findQueryMatches(index, query);
        cout << result << endl;
    }
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("///hello///"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
}

PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("///hello///"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}

// TODO: add your test cases here
STUDENT_TEST("test for cleanToken") {
    EXPECT_EQUAL(cleanToken("/hello/,.;?.."), "hello");
    EXPECT_EQUAL(cleanToken("wo!RLD!"), "wo!rld");
    EXPECT_EQUAL(cleanToken("/hel..lo/"), "hel..lo");
    EXPECT_EQUAL(cleanToken("/./."), "");

    Map<string, Set<string>> index;
    int nPages = buildIndex("res/website.txt", index);
    EXPECT_EQUAL(nPages, 32);
    EXPECT_EQUAL(index.size(), 3875);
    EXPECT(index.containsKey("that"));
    EXPECT(index.containsKey("cs106b"));

    //    TIME_OPERATION()
    searchEngine("res/website.txt");
}

// TODO: add your test cases here
