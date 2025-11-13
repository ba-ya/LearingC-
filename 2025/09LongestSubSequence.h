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

/// 最长增长子序列
namespace LIS {

}
#endif // _9LONGESTSUBSEQUENCE_H
