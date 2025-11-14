#ifndef _9LONGESTSUBSEQUENCE_H
#define _9LONGESTSUBSEQUENCE_H

#include "00solution.h"

namespace LCS {
//1143最长公共子序列
int longestCommonSubsequence(string text1, string text2) {
    int n = text1.size();
    int m = text2.size();
    vector<vector<int>> memo(n, vector<int>(m, -1));
    auto dfs = [&](this auto &&dfs, int i, int j) -> int {
        if (i < 0 || j < 0) {
            return 0;
        }
        int &res = memo[i][j];
        if (res != -1) {
            return res;
        }
        if (text1[i] == text2[j]) {
            return res = dfs(i - 1, j - 1) + 1;
        }
        return res = max(dfs(i, j - 1), dfs(i - 1, j));
    };
    return dfs(n - 1, m - 1);
}

// 72编辑距离
int minDistance(string s, string t) {
    int n = s.size();
    int m = t.size();
    vector<vector<int>> memo(n, vector<int>(m, -1));
    auto dfs = [&](this auto &&dfs, int i, int j) -> int {
        if (i < 0) {
            return j + 1;
        }
        if (j < 0) {
            return i + 1;
        }
        int &res = memo[i][j];
        if (res != -1) {
            return res;
        }
        // 相等,不需要修改
        if (s[i] == t[j]) {
            return res = dfs(i - 1, j - 1);
        }
        // 关注s, 不相等加一次操作
        // 删除(去掉s[i]), 插入(和t[j]一样的,相等于把t[j]去掉), 修改(s[i]=t[j],都去掉)
        return res = min(dfs(i - 1, j), min(dfs(i, j - 1), dfs(i - 1, j - 1))) + 1;
    };
    return dfs(n - 1, m - 1);
}

// 97交错字符串
bool isInterleave(string s1, string s2, string s3) {
    int n = s1.size();
    int m = s2.size();
    if (n + m != s3.size()) {
        return false;
    }
    vector<vector<int>> memo(n + 1, vector<int>(m + 1, -1));
    // 下标i, 当前个数i+1
    // 下标j, 当前个数j+1
    // 对应s3,个数是i+j+2, 下标是i+j+1
    auto dfs = [&](this auto &&dfs, int i, int j) -> bool {
        // 所有字符都遍历完,都是true
        // 最后结果也是true
        if (i < 0 && j < 0) {
            return true;
        }
        // 存在(-1, x), (x, -1)的情况,所以memo要+1,避免下标为负
        // 记忆数组与实际错一个
        int &res = memo[i + 1][j + 1];
        if (res != -1) {
            return res;
        }
        // s1[i]和s3最后一个字符一样
        // s2[j]和s3最后一个字符一样
        return res = (i >= 0 && s1[i] == s3[i + j + 1] && dfs(i - 1, j)) ||
                     (j >= 0 && s2[j] == s3[i + j + 1] && dfs(i, j - 1));
    };
    return dfs(n - 1, m - 1);
}

// 1092最短公共超序列
string shortestCommonSupersequence(string str1, string str2) {
    int n = str1.length();
    int m = str2.length();
    vector<vector<int>> memo(n, vector<int>(m, -1));
    auto dfs = [&](this auto &&dfs, int i, int j) -> int{
        if (i < 0) {
            return j + 1;
        }
        if (j < 0) {
            return i + 1;
        }
        int &res = memo[i][j];
        if (res != -1) {
            return res;
        }
        if (str1[i] == str2[j]) {
            return res = dfs(i - 1, j - 1) + 1;
        }
        int tmp1 = dfs(i - 1, j);
        int tmp2 = dfs(i, j - 1);
        return res = min(dfs(i - 1, j), dfs(i, j - 1)) + 1;
    };
    auto make_ans = [&](this auto &&make_ans, int i, int j) -> string {
        if (i < 0) return str1.substr(0, j + 1);
        if (j < 0) return str1.substr(0, i + 1);
        if (str1[i] == str2[j]) {
            return make_ans(i - 1, j - 1) + str1[i];
        }
        if (dfs(i, j) == dfs(i - 1, j)) {
            return make_ans(i - 1, j) + str1[i];
        }
        return make_ans(i, j - 1) + str2[j];
    };
    return make_ans(n - 1, m - 1);
}
}

namespace LIS {
// 300最长递增子序列
// 时间复杂度:O(n^2), 空间复杂度:O(n)
int lengthOfLIS(vector<int>& nums) {
    int n = nums.size();
    vector<int> memo(n, -1);
    // dfs(i)表示已nums[i]为结尾的子序列最长长度
    auto dfs = [&](this auto &&dfs, int i) {
        int &res = memo[i];
        if (res != -1) {
            return res;
        }
        for (int j = 0; j < i; j++) {
            // nums[j]是nums[i]结尾的子序列的一部分
            // dfs(i) = dfs(j) + 1
            // 严格递增,<
            if (nums[j] < nums[i]) {
                res = max(res, dfs(j) + 1);
            }
        }
        // 没有更小的,说明子序列只有nums[i]自己,答案是1
        return res = res == -1 ? 1 : res;
    };
    // 没有排序,n-1不一定是最大的,在前面可能存在较大的数,使得子序列更长
    int ans = 0;
    for (int i = 0; i < n; i++) {
        ans = max(ans, dfs(i));
    }
    return ans;
}

// 300最长递增子序列
// 贪心+二分
int lengthOfLIS_2(vector<int>& nums) {
    vector<int> g;
    for (auto x : nums) {
        // 求得第一个>=x的位置
        auto it = ranges::lower_bound(g, x);
        // 如果g这个序列没有>=x的数,就假如
        if (it == g.end()) {
            g.emplace_back(x);
        } else {
            *it = x;
        }
    }
    return g.size();
}

// 2826将三个组排序
// 时间复杂度:O(n^2), 空间复杂度:O(n)
int minimumOperations(vector<int>& nums) {
    int n = nums.size();
    vector<int> memo(n, -1);
    auto dfs = [&](this auto &&dfs, int i) ->int {
        int &res = memo[i];
        if (res != -1) {
            return res;
        }
        for (int j = 0; j < i; j++) {
            if (nums[j] <= nums[i]) {
                res = max(res, dfs(j) + 1);
            }
        }
        res = res == -1 ? 1 : res;
        return res;
    };
    int ans = 0;
    for (int i = 0; i < n; i++) {
        ans = max(ans, dfs(i));
    }
    return n - ans;
}

// 1671得到山形数组的最小删除次数
int minimumMountainRemovals(vector<int>& nums) {
    int n = nums.size();
    vector<int> memo_pre(n, -1), memo_suf(n, -1);
    auto dfs_pre = [&](this auto &&dfs_pre, int i) -> int {
        int &res = memo_pre[i];
        if (res != -1) {
            return res;
        }
        for (int j = 0; j < i; j++) {
            if (nums[j] < nums[i]) {
                res = max(res, dfs_pre(j) + 1);
            }
        }
        return res = res == -1 ? 1 : res;
    };
    auto dfs_suf = [&](this auto &&dfs_suf, int i) -> int {
        int &res = memo_suf[i];
        if (res != -1) {
            return res;
        }
        for (int j = i + 1; j < n; j++) {
            if (nums[j] < nums[i]) {
                res = max(res, dfs_suf(j) + 1);
            }
        }
        return res = res == -1 ? 1 : res;
    };
    int ans = 0;
    for (int i = 0; i < n; i++) {
        int pre = dfs_pre(i);
        int suf = dfs_suf(i);
        if (pre >= 2 && suf >= 2) {
            ans = max(ans, pre + suf - 1);
        }
    }
    return n - ans;
}

// 354俄罗斯套娃信封问题
int maxEnvelopes(vector<vector<int>>& envelopes) {
    // w升序排列, h降序排列
    ranges::sort(envelopes, {},
                 [](vector<int> e) {return pair{e[0], -e[1]};});
    vector<int> g;
    for (auto &e : envelopes) {
        int h = e[1];
        auto it = ranges::lower_bound(g, h);
        if (it == g.end()) {
            g.emplace_back(h);
        } else {
            *it = h;
        }
    }
    return g.size();
}
}
#endif // _9LONGESTSUBSEQUENCE_H
