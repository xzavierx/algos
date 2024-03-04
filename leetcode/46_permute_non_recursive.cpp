#include <iterator>
#include <vector>
#include <queue>
#include <iostream>
using namespace std;
class Solution {
public:
  vector<vector<int>> permute(vector<int>& nums) {
    vector<vector<int>> res;
    queue<vector<int>>qu;
    int begin=0,twobegin=0;
    for(int i=begin;i<nums.size();i++){
      swap(nums[i],nums[begin]);
      qu.emplace(nums);
      twobegin=0;
      while(!qu.empty()){
        cout << qu.size() << endl;
        int sz=qu.size();
        twobegin++;
        while(sz--){
          vector<int> temp = qu.front();
          if(twobegin==nums.size()) res.emplace_back(temp);
          qu.pop();
          for(int i = twobegin;i<nums.size();i++){
              swap(temp[i],temp[twobegin]);
              std::copy(temp.begin(), temp.end(), std::ostream_iterator<int>(cout, " "));
              cout << endl;
              qu.emplace(temp);
              swap(temp[i],temp[twobegin]);
          }
        }
      }
      swap(nums[i],nums[begin]);
    }
    return res;
  }
};

int main() {
  vector<int> nums = {1, 2, 3, 4};
  Solution s;
  auto result =s.permute(nums);
  // for (auto s : result) {
  //   std::copy(s.begin(), s.end(), std::ostream_iterator<int>(cout, " "));
  //   std::cout << std::endl;
  // }
}