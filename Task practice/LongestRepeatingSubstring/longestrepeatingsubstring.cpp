#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <cassert>

using namespace std;

int longestRepeatingSubstring(string& s) {
    int n = s.size();
    int res = 0;

    for (int i = 0; i < n; i++) {

        vector<bool> visited(256, false);

        for (int j = i; j < n; j++) {

            if (visited[s[j]] == true)
                break;

          
            else {
                res = max(res, j - i + 1);
                visited[s[j]] = true;
            }
        }
    }
    return res;
}
void runTests() {
    string s1 = "abcabcbb";
    assert(longestRepeatingSubstring(s1) == 3); 
    string s3 = "aaaa";
    assert(longestRepeatingSubstring(s3) == 1); 
    cout << "All test cases passed!" << endl;
}
int main(){
    runTests();
    return 0;
}


