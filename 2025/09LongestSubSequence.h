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
}

/// 最长增长子序列
namespace LIS {

}
#endif // _9LONGESTSUBSEQUENCE_H
