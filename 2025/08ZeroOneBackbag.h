#ifndef _8ZEROONEBACKBAG_H
#define _8ZEROONEBACKBAG_H

#include "00solution.h"

namespace ZeroOneBackbag {
// 494目标和
// 时间复杂度:O(mn), 空间复杂度:O(mn)
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

// 322零钱兑换
// 时间复杂度:O(n*amount), 空间复杂度:O(n*amount)
int coinChange(vector<int>& coins, int amount) {
    // 求零钱个数
    int n = coins.size();
    vector<vector<int>> memo(n, vector<int>(amount + 1, -1));
    auto dfs = [&](this auto &&dfs, int i, int c) ->int{
        if (i < 0) {
            return c == 0 ? 0 : INT_MAX / 2;
        }
        int &res = memo[i][c];
        if (res != -1) {
            return res;
        }
        // 容量不够只能不选
        if (c < coins[i]) {
            return res = dfs(i - 1, c);
        }
        // 不选与选(可以重复,i不用减1, +1表示加上现在这次选的零钱)
        return res = min(dfs(i - 1, c), dfs(i, c - coins[i]) + 1);
    };
    int ans = dfs(n - 1, amount);
    // 如果不存在,返回的结果是min(INT_MAX / 2, INT_MAX / 2 + 1), 即INT_MAX / 2
    return ans == INT_MAX / 2 ? -1 : ans;
}

// 2915和为目标值的最长子序列的长度
// 时间复杂度:O(n*target), 空间复杂度:O(n*target)
int lengthOfLongestSubsequence(vector<int>& nums, int target) {
    int n = nums.size();
    vector<vector<int>> memo(n, vector<int>(target + 1, -1));
    auto dfs = [&](this auto &&dfs, int i, int c) -> int {
        if (i < 0) {
            return c == 0 ? 0 : INT_MIN;
        }
        int &res = memo[i][c];
        if (res != -1) {
            return res;
        }
        res = dfs(i - 1, c);
        if (c >= nums[i]) {
            res = max(res, dfs(i - 1, c - nums[i]) + 1);
        }
        return res;
    };
    int ans = dfs(n - 1, target);
    // cout << INT_MIN << endl;//-2147483648
    // cout << INT_MIN / 2 << endl;//-1073741824
    // cout << ans << endl;//-2147483646
    // 如果不存在,返回的结果是 max(INT_MIN / 2, INT_MIN / 2 + 1), 即INT_MIN / 2 + 1,
    // 不是确定值了,下面判断就不能用=, 要用<,>之类的
    return ans >= 0 ? ans : -1;
}

// 416分隔等和子集
bool canPartition(vector<int>& nums) {
    // p = q = sum / 2
    // 背包容量是sum / 2
    int sum = reduce(nums.begin(), nums.end());
    if (sum % 2) {
        return false;
    }
    int MOD = 1e9+7;
    int n = nums.size();
    int target = sum / 2;
    vector<vector<int>> memo(n, vector<int>(target + 1, -1));
    // 求方案数, (判断有没有,)
    auto dfs = [&](this auto &&dfs, int i, int c) -> int {
        if (i < 0) {
            return c == 0;
        }
        int &res = memo[i][c];
        if (res != -1) {
            return res;
        }
        // 不选
        res = dfs(i - 1, c) % MOD;
        if (c >= nums[i]) {
            // 不选 + 选
            // res = (res + dfs(i - 1, c - nums[i])) % MOD;// 方案数
            res = res || dfs(i - 1, c - nums[i]); // 判断有没有
        }
        return res;
    };
    int ans = dfs(n - 1, target);
    return ans > 0;
}

// 518零钱兑换2
// 时间复杂度:O(n*amount), 空间复杂度:O(n*amount)
int change(int amount, vector<int>& coins) {
    // 求方案数, 可以重复选
    int n = coins.size();
    vector<vector<int>> memo(n, vector<int>(amount + 1, -1));
    auto dfs = [&](this auto &&dfs, int i, int c) -> int {
        if (i < 0) {
            // 求的是恰好,所以这里要判断容量是不是刚好为空
            return c == 0 ? 1 : 0;
        }
        int &res = memo[i][c];
        if (res != -1) {
            return res;
        }
        // 容量不够,不选
        if (c < coins[i]) {
            return res = dfs(i - 1, c);
        }
        // 不选 + 选
        return res = dfs(i - 1, c) + dfs(i, c - coins[i]);
    };
    int ans = dfs(n - 1, amount);
    return ans;
}

// 279完全平方数
int numSquares(int n) {
    // 方案里面所选的数字最少的
    vector<int> nums;
    for (int i = 1; i * i <= n; i++) {
        nums.emplace_back(i * i);
    };
    int size = nums.size();
    vector<vector<int>> memo(size, vector<int>(n + 1, -1));
    auto dfs = [&](this auto &&dfs, int i, int c) -> int {
        if (i < 0) {
            return c == 0 ? 0 : INT_MAX - 1;
        };
        int &res = memo[i][c];
        if (res != -1) {
            return res;
        }
        if (c < nums[i]) {
            return res = dfs(i - 1, c);
        }
        return res = min(dfs(i - 1, c), dfs(i, c - nums[i]) + 1);
    };
    int ans = dfs(size - 1, n);
    return ans == INT_MAX - 1 ? 0 : ans;
}
}

#endif // _8ZEROONEBACKBAG_H
