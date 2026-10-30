/*
COMPLETE STRING (Longest Word With All Prefixes) - Coding Ninjas / Code360
https://www.naukri.com/code360/problems/complete-string_2687860

Problem Statement:
You are given an array 'A' of 'N' strings.
A string is called a COMPLETE STRING if every prefix of it is also present in the array 'A'.
Find the longest complete string in 'A'.
If several complete strings have the same (longest) length, return the lexicographically smallest one.
If no complete string exists, return "None".

Example:
Input:  N = 4, A = ["ab", "abc", "a", "bp"]
Output: "abc"
Explanation:
    "abc": its prefixes "a", "ab", "abc" are all present in A, so it is complete.
    "bp":  its prefixes are "b" and "bp", and "b" is not present in A, so it is not complete.
    "a" and "ab" are also complete, but "abc" is the longest.

Note:
The strings contain only lowercase English letters.
For the exact input format, constraints and time limit, see the problem page above.
*/

#include "trie_impl.cpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

string completeString(int n, vector<string> &a)
{
    // Write your code here.
    Trie trie=Trie();
    for(string s:a)
        trie.insert(s);
    trie.print();

    return a[0];
}

int main()
{
    // each test case: the input array and the expected answer
    vector<pair<vector<string>, string>> tests = {
        {{"ab", "abc", "a", "bp"}, "abc"},                 // example from the problem
        {{"n", "ni", "nin", "ninj", "ninja", "ninga"}, "ninja"}, // "ninga" is missing "ning"
        {{"ab", "bc"}, "None"},                            // "a" and "b" are missing
        {{"b", "a", "c"}, "a"},                            // all length 1, smallest wins
        {{"a", "ab", "ac"}, "ab"},                         // tie on length, smallest wins
    };

    for (auto &[a, expected] : tests)
    {
        string got = completeString(a.size(), a);
        cout << (got == expected ? "PASS" : "FAIL") << "  got: " << got << "  expected: " << expected << endl
             << endl;
    }
}