#ifndef _6BACKTRACKING_H
#define _6BACKTRACKING_H

#include "00solution.h"

namespace BackTrack_Subset {;
// 17电话号码的数字组合
vector<string> letterCombinations(string digits) {
    vector<string> ans;
    int n = digits.size();
    if (n == 0) {
        return {};
    }
    string path(n, 0);
    string MAPPING[10]{"", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};
    function<void(int)> dfs = [&](int i) {
        if (i == n) {
            ans.push_back(path);
            return;
        }
        int number = digits[i] - '0';
        for (char c : MAPPING[number]) {
            path[i] = c;
            dfs(i + 1);
        }
    };
    dfs(0);
    return ans;
}

// 78子集
vector<vector<int>> subsets(vector<int>& nums) {
    int n = nums.size();
    if (n == 0) {
        return {};
    }
    vector<vector<int>> ans;
    vector<int> path;
    function<void(int)> dfs = [&](int i) {
        if (n == i) {
            ans.emplace_back(path);
            return;
        }
        // 不选nums[i]
        dfs(i + 1);

        // 选nums[i]
        path.push_back(nums[i]);
        dfs(i + 1);
        // 恢复现场
        path.pop_back();
    };
    dfs(0);
    return ans;
}

// 131分割回文串
vector<vector<string>> partition(string s) {
    // 是否回文
    auto check = [&](int left, int right) {
        while (left < right) {
            if (s[left] != s[right]) {
                return false;
            }
            left++;
            right--;
        }
        return true;
    };
    int n = s.size();
    vector<vector<string>> ans;
    vector<string> path;
    // i表示当前是第几个逗号, 右端点的位置
    // start回文串开始位置
    function<void(int, int)> dfs = [&](int i, int start) {
        if (i == n) {
            ans.emplace_back(path);
            return;
        }
        // FIX ME: don't understand
        // 不加逗号
        if (i <= n - 2) {
            dfs(i + 1, start);
        }

        // 加逗号
        if (check(start, i)) {
            path.emplace_back(s.substr(start, i - start + 1));
            dfs(i + 1, i + 1);
            path.pop_back();
        }

    };
    dfs(0, 0);
    return ans;

}

int pre_sum[1001];
int init = []() {
    for (int i = 1; i <= 1000; i++) {
        string s = to_string(i * i);
        int n = s.size();
        function<bool(int, int)> dfs = [&](int p, int sum) {
            if (p == n) {
                return sum == i;
            }
            int x = 0;
            for (int j = p; j < n; j++) {
                x = x * 10 + s[j] - '0';
                if (dfs(j + 1, sum + x)) {
                    return true;
                }
            }
            return false;
        };
        pre_sum[i] = pre_sum[i - 1] + (dfs(0, 0) ? i * i : 0);
    }
    return 0;
}();
// 2698求一个整数的惩罚数
int punishmentNumber(int n) {
    return pre_sum[n];
}

}
namespace BackTrack_CombinationNPrun {
}
namespace BackTrack_Permutation {
}

#endif // _6BACKTRACKING_H
