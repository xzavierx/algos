#include <iterator>
#include <stdio.h>
#include <vector>
#include <iostream>
using namespace std;
class Solution {
public:
  vector<vector<int>> permute(vector<int>& nums) {
    vector<vector<int>> a;
    if (nums.size() == 0) return a;

    vector<int> state;
    dfs(state, a, nums);
    return a;
  }
  
  
  void dfs(vector<int>& state, vector<vector<int>>& res, vector<int>& nums) {
    if (state.size() == nums.size()) {
      res.push_back(state);
      return;
    }
    ++level;
    auto placeholder = string(state.size() * 2, ' ');
    cout << placeholder << "- dfs" << level << endl;

    for (int i = 0; i < nums.size(); ++i) {
      if (std::find(state.begin(), state.end(), nums[i]) != std::end(state)) {
        cout << placeholder << "i = " << i << " skip" << endl;
        continue;
      }
      state.push_back(nums[i]);
      cout << placeholder <<  "i = " << i << " " << "state = [";
      std::copy(state.begin(), state.end(), std::ostream_iterator<int>(cout, " "));
      cout << "]" << endl;
      dfs(state, res, nums);
      state.pop_back();
    }
  }
  int level = 0;
};

int main() {
  vector<int> nums = {1, 2, 3};
  Solution s;
  auto result =s.permute(nums);
}
/*
- dfs1
i = 0 state = [1]
  - dfs2 
    i = 0 skip
    i = 1 state = [1 2]
      - dfs3
        i = 0 skip
        i = 1 skip
        i = 2 state = [1 2 3]

    i = 2 state = [1 3]
      - dfs4
        i = 0 skip
        i = 1 state = [1 3 2]
        i = 2 skip
i = 1 state = [2]
  - dfs5      
    i = 0 state = [2 1]
    - dfs6
      i = 0 skip
      i = 1 skip
      i = 2 state = [2 1 3]
    i = 1 skip
    i = 2 state = [2 3]
    - dfs7
      i = 0 state = [2 3 1]
      i = 1 skip
      i = 2 skip

i = 2 state = [3]
  - dfs8 
    i = 0 state = [3 1]
    - dfs9
      i = 0 skip
      i = 1 state = [3 1 2]
      i = 2 skip
    i = 1 state = [3 2]
    - dfs10 
      i = 0 state = [3 2 1]
      i = 1 skip
      i = 2 skip
    i = 2 skip
*/
