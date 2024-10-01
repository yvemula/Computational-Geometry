#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<string> fullJustify(vector<string>& words, int maxWidth) {
    vector<string> res;
    vector<string> currentLine;
    int currentLength = 0;

    for (string& word : words) {
        if (currentLength + word.length() + currentLine.size() > maxWidth) {
           
            int spacesToDistribute = maxWidth - currentLength;
            for (int i = 0; i < spacesToDistribute; ++i) {
                currentLine[i % (currentLine.size() - 1 == 0 ? 1 : currentLine.size() - 1)] += " ";
            }

            string line = "";
            for (string& s : currentLine) line += s;

            res.push_back(line);

            currentLine.clear();
            currentLength = 0;
        }

       
        currentLine.push_back(word);
        currentLength += word.length();
    }


    string lastLine = "";
    for (int i = 0; i < currentLine.size(); ++i) {
        if (i > 0) lastLine += " ";
        lastLine += currentLine[i];
    }
    

    lastLine += string(maxWidth - lastLine.length(), ' ');
    res.push_back(lastLine);

    return res;
}

int main() {
    
    vector<string> words1 = {"This", "is", "an", "example", "of", "text", "justification."};
    int maxWidth1 = 16;
    vector<string> result1 = fullJustify(words1, maxWidth1);
    for (string& line : result1) {
        cout << "\"" << line << "\"" << endl;
    }
    cout << endl;

    
    vector<string> words2 = {"Science", "is", "what", "we", "understand", "well", "enough", "to", "explain", "to", "a", "computer.", "Art", "is", "everything", "else", "we", "do"};
    int maxWidth2 = 20;
    vector<string> result2 = fullJustify(words2, maxWidth2);
    for (string& line : result2) {
        cout << "\"" << line << "\"" << endl;
    }

    return 0;
}
