#include <iostream>
#include <unordered_map>
#include <sstream>
#include <vector>

using namespace std;

bool wordPattern(string pattern, string s) {
    unordered_map<char, string> charToWord;
    unordered_map<string, char> wordToChar;
    istringstream iss(s);
    vector<string> words;
    string word;
    
    while (iss >> word) {
        words.push_back(word);
    }

    if (words.size() != pattern.size()) {
        return false;
    }

    for (int i = 0; i < pattern.size(); ++i) {
        char p = pattern[i];
        string w = words[i];
        
        if (charToWord.find(p) != charToWord.end()) {
            if (charToWord[p] != w) {
                return false;
            }
        } else {
            if (wordToChar.find(w) != wordToChar.end()) {
                return false;
            }
            charToWord[p] = w;
            wordToChar[w] = p;
        }
    }

    return true;
}

int main() {
    // Test cases
    cout << boolalpha;  // Print true/false instead of 1/0
    cout << wordPattern("abba", "dog cat cat dog") << endl;  
    cout << wordPattern("abba", "dog cat cat fish") << endl; 
    cout << wordPattern("aaaa", "dog dog dog dog") << endl;  
    cout << wordPattern("abba", "dog dog dog dog") << endl; 
    
    return 0;
}
