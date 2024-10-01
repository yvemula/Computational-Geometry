#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;

bool wordPattern(string pattern, string str) {
    unordered_map<char, string> charToWord;
    unordered_map<string, char> wordToChar;
    vector<string> words;
    istringstream iss(str);//input string steam object to read from string
    string word;
    while (iss >> word) {
        words.push_back(word);
    }

    if (words.size() != pattern.length()) {
        return false;
    }
    for (int i = 0; i < pattern.size(); ++i) {
        char c = pattern[i];
        string w = words[i];

        if (charToWord.count(c)) {
            if (charToWord[c] != w) {
                return false;
            }
        } else {
            charToWord[c] = w;
        }

        if (wordToChar.count(w)) {
            if (wordToChar[w] != c) {
                return false;
            }
        } else {
            wordToChar[w] = c;
        }
    }
    return true;
}
int main() {
    string str1 = "dog cat cat dog";
    string pattern1 = "abba";
    cout << wordPattern(pattern1, str1) << endl;  // Output: 1 (true)

    string str2 = "dog cat cat fish";
    string pattern2 = "abba";
    cout << wordPattern(pattern2, str2) << endl;  // Output: 0 (false)
    return 0;
}
