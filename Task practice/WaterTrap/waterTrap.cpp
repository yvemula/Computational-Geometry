#include <iostream>
#include <vector>
using namespace std;

int trap(vector<int>& height) {
    if (height.empty()) return 0;

    int left = 0, right = height.size() - 1;
    int left_max = 0, right_max = 0;
    int water_trapped = 0;

    while (left < right) {
        if (height[left] < height[right]) {
            if (height[left] >= left_max) {
                left_max = height[left];  // Update left max
            } else {
                water_trapped += left_max - height[left];  // Trap water
            }
            ++left;
        } else {
            if (height[right] >= right_max) {
                right_max = height[right];  // Update right max
            } else {
                water_trapped += right_max - height[right];  // Trap water
            }
            --right;
        }
    }

    return water_trapped;
}

int main() {
    vector<int> height1 = {2, 0, 2};
    cout << "Water trapped: " << trap(height1) << endl;  // Output: 2

    vector<int> height2 = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
    cout << "Water trapped: " << trap(height2) << endl;  // Output: 6

    return 0;
}
