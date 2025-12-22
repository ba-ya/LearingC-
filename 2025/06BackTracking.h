#ifndef _6BACKTRACKING_H
#define _6BACKTRACKING_H

#include "00solution.h"

namespace BackTrack_Subset {;
// 17电话号码的数字组合
vector<string> letterCombinations(string digits) {
    vector<string> ans;
    int n = digits.size();
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

// 78, 子集
vector<vector<int>> subsets(vector<int>& nums) {
    int n = nums.size();
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
    // 选或不选
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
    // start回文串开始位置
    // end表示当前是第几个逗号, 右端点的位置
    // 子串[start, end]
    function<void(int, int)> dfs = [&](int start, int end) {
        if (end == n) {
            ans.emplace_back(path);
            return;
        }
        // 不加逗号
        // i <= n - 2 , i < n - 1 , i + 1 < n
        if (end <= n - 2) {
            // 保证end+1在范围内
            dfs(start, end + 1);
        }

        // 加逗号
        if (check(start, end)) {
            path.emplace_back(s.substr(start, end - start + 1));
            dfs(end + 1, end + 1);
            path.pop_back();
        }

    };
    dfs(0, 0);
    return ans;
}
vector<vector<string>> partition_2(string s) {
    // 枚举选哪个
    auto is_palindrome = [&](int left, int right) ->bool {
        while (left < right) {
            if (s[left++] != s[right--]) {
                return false;
            }
        }
        return true;
    };
    int n = s.length();
    vector<vector<string>> ans;
    vector<string> path;
    // dfs(i),表示把i当start, 枚举end
    auto dfs = [&](this auto &&dfs, int i) {
        if (i == n) {
            ans.push_back(path);
            return;
        }
        for (int j = i; j < n; j++) {
            if (is_palindrome(i, j)) {
                path.emplace_back(s.substr(i, j - i + 1));
                dfs(j + 1);
                path.pop_back();
            }
        }
    };
    dfs(0);
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
        // dfs(0, 0)判断当前数字是否满足条件,满足就加上
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
// 77组合
vector<vector<int>> combine(int n, int k) {
    vector<vector<int>> ans;
    vector<int> path;
    function<void(int)> dfs = [&](int i) {
        // path还需要d个数
        int d = k - path.size();
        if (d == 0) {
            ans.emplace_back(path);
            return;
        }
        // 不选i
        if (i > d) {
            dfs(i - 1);
        }

        //选i
        path.emplace_back(i);
        dfs(i - 1);
        path.pop_back();
    };
    dfs(n);
    return ans;
}

// 216组合总和3
vector<vector<int>> combinationSum3(int k, int n) {
    vector<vector<int>> ans;
    vector<int> path;
    function<void(int, int)> dfs = [&](int i, int left_sum) {
        int d = k - path.size();
        // 剪枝操作
        // 等差数列求和公式, 剩下的最大的d个数之和(2a + (n - 1) * d) * (d / 2)不能小于left_sum
        if (left_sum < 0 || left_sum > (2 * i - d + 1) * d / 2) {
            return;
        }
        if (d == 0) {
            ans.emplace_back(path);
            return;
        }
        for (int j = i; j >= d; j--) {
            path.emplace_back(j);
            dfs(j - 1, left_sum - j);
            path.pop_back();
        }
    };
    dfs(9, n);
    return ans;
}

// 22, 括号生成
vector<string> generateParenthesis(int n) {
    vector<string> ans;
    // 长度固定为2n,可以先给path一个长度
    string path(n * 2, 0);
    // left个左括号,right个右括号
    function<void(int, int)> dfs = [&](int left, int right) {
        if (right ==  n) {
            ans.emplace_back(path);
            return;
        }
        // '('当作选
        if (left < n) {
            path[left + right] = '(';
            dfs(left + 1, right);
        }
        // ')'当作不选
        if (right < left) {
            path[left + right] = ')';
            dfs(left, right + 1);
        }
    };
    dfs(0, 0);
    return ans;
}
vector<string> generateParenthesis_2(int n) {
    // 记录左括号位置
    vector<string> ans;
    vector<int> path;//左括号位置
    // 这里i和balance表示个数
    // dfs(i, balance)表示已经填了i个括号,这i个括号中左括号 - 右括号 = balance
    auto dfs = [&](this auto &&dfs, int i, int balance) {
        if (path.size() == n) {
            string s(n * 2, ')');
            for (int i : path) {
                s[i] = '(';
            }
            ans.push_back(s);
            return;
        }
        for (int right = 0; right <= balance; right++) {
            // 先填right个右括号, 再填1个括号
            path.push_back(i + right);
            dfs(i + right + 1, balance + 1 - right);
            path.pop_back();
        }
    };
    dfs(0, 0);
    return ans;
}

// 39, 组合总和
vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
    ranges::sort(candidates);
    vector<vector<int>> ans;
    vector<int> path;
    function<void(int, int)> dfs = [&](int i, int capacity) {
        if (capacity == 0) {
            ans.emplace_back(path);
            return;
        }
        for (int j = i; j < candidates.size() && candidates[j] <= capacity; j++) {
            path.push_back(candidates[j]);
            dfs(j, capacity - candidates[j]);
            path.pop_back();
        }
    };
    dfs(0, target);
    return ans;
}
vector<vector<int>> combinationSum_2(vector<int>& candidates, int target) {
    int n = candidates.size();
    vector<vector<int>> ans;
    vector<int> path;
    auto dfs = [&](this auto &&dfs, int i, int capacity) {
        if (capacity == 0) {
            ans.push_back(path);
            return;
        }
        if (i >= n || capacity < 0) {
            return;
        }
        // 不选
        dfs(i + 1, capacity);

        // 选
        path.push_back(candidates[i]);
        dfs(i, capacity - candidates[i]);
        path.pop_back();
    };
    dfs(0, target);
    return ans;
}

// 93复原IP地址
vector<string> restoreIpAddresses(string s) {
    int n = s.size();
    vector<string> ans;
    //.号所在位置
    int path[4]{0};
    // i是字符位置,j是段数,val是当前段的值[0, 255]
    function<void(int, int, int)> dfs = [&](int i, int j, int val){
        if (i == n) {// 到字符结尾
            if (j == 4) { // 4段
                auto [a, b, c, d] = path;
                ans.emplace_back(s.substr(0, a) + "." +
                                 s.substr(a, b - a) + "." +
                                 s.substr(b, c - b) + "." +
                                 s.substr(c, d - c));
                return;
            }
        }
        if (j == 4) {
            return;
        }

        val = val * 10 + s[i] - '0';
        if (val > 255) {
            return;
        }
        //不选, i+1, j不变
        //必须在ip没有前置0的情况才能不选
        // 也就是第一个数字必须非0
        if (val > 0) {
            dfs(i + 1, j, val);
        }

        //选
        path[j] = i + 1;
        dfs(i + 1, j + 1, 0);
    };
    dfs(0, 0, 0);
    return ans;
}
}
namespace BackTrack_Permutation {
// 46, 全排列
// 时间复杂度: O(n(n!)) n!个根节点 * n
vector<vector<int>> permute(vector<int>& nums) {
    int n = nums.size();
    vector<vector<int>> ans;
    vector<int> path(n), on_path(n, 0);
    // dfs(i), 表示要构造排列>=i的路径
    function<void(int)> dfs = [&](int i) {
        if (i == n) {
            ans.emplace_back(path);
            return;
        }
        for (int j = 0; j < n; j++) {
            // 数字没选过,选一下
            if (!on_path[j]) {
                // 如果j没选过,path可以填一下
                path[i] = nums[j];
                on_path[j] = true;
                // path下一个就是i+1
                dfs(i + 1);
                // 再回复现场
                on_path[j] = false;
            }
        }
    };
    dfs(0);
    return ans;
}

// 51, N皇后
// 棋盘(0, 0)在左上角, 和二维数组的坐标一致
// amn(m = 行, n = 列)
// a00, a11, a22所在的线是主对角线(斜下方, r - c),
// a(n-1)0, a(n-2)1, a(n-3)2所在的线是次对角线(斜下方, r + c)
vector<vector<string>> solveNQueens(int n) {
    vector<vector<string>> ans;
    vector<string> board(n, string(n, '.'));
    // col, r + c, r - c
    // true表示当前列,当前主对角线,次对角线已经存在棋子
    // 不允许在这3条线上放置棋子
    vector<uint8_t> col(n), diag1(2 * n), diag2(2 * n);
    function<void(int)> dfs = [&](int r) {
        if (r == n) {
            ans.push_back(board);
            return;
        }
        for (int c = 0; c < n; c++) {
            int rc = r - c + n;
            if (!col[c] && !diag1[r + c] && !diag2[rc]) {
                board[r][c] = 'Q';
                col[c] = diag1[r + c] = diag2[rc] = true;
                dfs(r + 1);
                col[c] = diag1[r + c] = diag2[rc] = false;
                board[r][c] = '.';
            }
        }
    };
    dfs(0);
    return ans;
}

// 52, N皇后2
int totalNQueens(int n) {
    int ans = 0;
    vector<uint8_t> col(n), diag1(2 * n), diag2(2 * n);
    function<void(int)> dfs = [&](int r) {
        if (r == n) {
            ans++;
            return;
        }
        for (int c = 0; c < n; c++) {
            int rc1 = r + c;
            int rc2 = r - c + n;
            if (!col[c] && !diag1[rc1] && !diag2[rc2]) {
                col[c] = diag1[rc1] = diag2[rc2] = true;
                dfs(r + 1);
                col[c] = diag1[rc1] = diag2[rc2] = false;
            }
        }
    };
    dfs(0);
    return ans;
}

// 2850将石头分散到网格图的最少移动次数
int minimumMoves(vector<vector<int>>& grid) {
    vector<pair<int, int>> from, to;
    int n = grid.size();
    int m = grid[0].size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == 0) {
                to.emplace_back(i, j);
            } else {
                for (int k = 1; k < grid[i][j]; k++) {
                    from.emplace_back(i, j);
                }
            }
        }
    }
    int ans = INT_MAX;
    do {
        int total = 0;
        for (int i = 0; i < from.size(); i++) {
            // first对应的就是row上的差, 上下走
            // second对应的是col上的差, 左右走
            total += abs(from[i].first - to[i].first) + abs(from[i].second - to[i].second);
        }
        ans = min(ans, total);
        // 下一个排列
    } while(next_permutation(from.begin(), from.end()));
    // 按字典序进行排列,相同的from不会重复排列
    // -0  -1  -2  -3  -4  -5
    // 01  01  01  22  22  22
    // 01  22  22  01  01  22
    // 22  01  22  01  22  01
    // 22  22  01  22  01  01
    return ans;
}
}

#endif // _6BACKTRACKING_H
