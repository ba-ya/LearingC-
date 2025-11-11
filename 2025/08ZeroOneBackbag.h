#ifndef _8ZEROONEBACKBAG_H
#define _8ZEROONEBACKBAG_H

#include "00solution.h"

namespace ZeroOneBackbag {
// 494目标和
int findTargetSumWays(vector<int>& nums, int target) {
    // 背包容量 (sum - |target|) / 2
    int s = reduce(nums.begin(), nums.end()) - abs(target);
    if (s < 0 || s % 2) {
        return 0;
    }
    int n = nums.size();
    int m = s / 2;
    vector<vector<int>> memo(n, vector<int>(m + 1, -1));
    auto dfs = [&](this auto &&dfs, int i, int c) ->int {
        if (i < 0) {
            return c == 0;
        }
        int &res = memo[i][c];
        if (res != -1) {
            return res;
        }
        // 剩下容量不够了, 只能不选
        if (c < nums[i]) {
            return res = dfs(i - 1, c);

        }
        // 不选 + 选, 总方案数是所有情况之和
        return res = dfs(i - 1, c) + dfs(i - 1, c - nums[i]);

    };
    return dfs(n - 1, m);
}
}

#endif // _8ZEROONEBACKBAG_H
