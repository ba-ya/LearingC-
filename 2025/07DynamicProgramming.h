#ifndef _7DYNAMICPROGRAMMING_H
#define _7DYNAMICPROGRAMMING_H

#include "00solution.h"

namespace DynamicProgramming {
// 198打家劫舍
int rob(vector<int>& nums) {
    int n = nums.size();
    vector<int> memo(n, -1);
    function<int(int)> dfs = [&](int i) ->int {
        if (i < 0) {
            return 0;
        }
        if (memo[i] != -1) {
            return memo[i];
        }
        // 不选i, 0到i-1个房子的最大值
        // 选i, 0到i-2个房子的最大值和当前房子
        memo[i] = max(dfs(i - 1), dfs(i - 2) + nums[i]);
        return memo[i];
    };
    return dfs(n - 1);
}

// 70爬楼梯
int climbStairs(int n) {
    vector<int> memo(n + 1, -1);
    // 分类讨论,
    // 最后爬一个台阶(考虑前面爬n - 1个台阶的方案) 与
    // 最后爬两个台阶(考虑前面爬n - 2个台阶的方案)
    // dfs(i) = dfs(i - 1) + dfs(i - 2)
    // dfs(1) = 1, dfs(2) = 2, 倒推出dfs(0) = 1
    function<int(int)> dfs = [&](int i) {
        if (i <= 1) {
            return 1;
        }
        int &res = memo[i];
        if (res != -1) {
            return res;
        }
        return res = dfs(i - 1) + dfs(i - 2);
    };
    // 爬n个台阶对应的方案
    return dfs(n);
}

// 746使用最小花费爬楼梯
int minCostClimbingStairs(vector<int>& cost) {
    int n = cost.size();
    vector<int> memo(n + 1, -1);
    // dfs(0) = 0, dfs(1) = 0, 可以选择从0或1开始
    // dfs(i),爬到第i台阶的所有花费
    function<int(int)> dfs = [&](int i) -> int{
        if (i <= 1) {
            return 0;
        }
        if (memo[i] != -1) {
            return memo[i];
        }
        // cost[i - 2]和cost[i - 1]是爬剩下的2或1阶到i
        return memo[i] = min(dfs(i - 2) + cost[i - 2], dfs(i - 1) + cost[i - 1]);
    };
    // 求爬到第n台阶的所有花费
    return dfs(n);
}

// 3693爬楼梯2
int climbStairs(int n, vector<int>& costs) {
    vector<int> memo(n + 1, -1);
    // c++20, 有开销,一直超时, 1000ms左右
    // function<int(int)> dfs = [&](int i) -> int{
    // c++23, 换成这个就只有7ms
    auto dfs = [&](this auto &&dfs, int i) ->int {
        if (i < 0) {
            return 0;
        }
        int &res = memo[i];
        if (res != -1) {
            return res;
        }
        return res = min(dfs(i - 1) + costs[i] + 1,
                         min(dfs(i - 2) + costs[i] + 4, dfs(i - 3) + costs[i] + 9));
    };
    return dfs(n - 1);
}

// 377组合总和4
int combinationSum4(vector<int>& nums, int target) {
    // 变相爬楼梯
    // 一次可以爬nums中任意一个数, 有多少种方案能爬到target台阶上
    vector<int> memo(target + 1, -1);
    auto dfs = [&](this auto &&dfs, int i) ->int {
        if (i <= 0) {
            // 方案数,dfs(0) = 1, 原地不动也是一种方案
            return 1;
        }
        int &res = memo[i];
        if (res != -1) {
            return res;
        }
        res = 0;
        // 最后一次可以分类讨论n次(n = nums.size())
        // dfs(i - x)
        for (auto x : nums) {
            if (i >= x) {
                res += dfs(i - x);
            }
        }
        return res;
    };
    return dfs(target);
}

// 2466统计构造好字符串的方案数
int countGoodStrings(int low, int high, int zero, int one) {
    const int MOD = 1e9+7;
    int ans = 0;
    // memo一份就够了,不需要重置
    vector<int> memo(high + 1, -1);
    auto dfs = [&](this auto &&dfs, int i) ->int {
        if (i <= 0) {
            return 1;
        }
        int &res = memo[i];
        if (res != -1) {
            return res;
        }
        return res = (dfs(i - zero) + dfs(i - one)) % MOD;
    };
    for (int target = low; target <= high; target++) {
        ans = (ans + dfs(target)) % MOD;
    }
    return ans;
}

// 2266统计打字方案数
int countTexts(string pressedKeys) {
    int n = pressedKeys.size();
    if (n == 0) {
        return 0;
    }
    const int MOD = 1e9+7;
    vector<int> memo1(n + 1, -1);
    vector<int> memo2(n + 1, -1);
    auto dfs = [&](this auto &&dfs, int i, char c) {
        if (i == 0) {
            return 1;
        }
        if (i < 0) {
            return 0;
        }
        int cnt = (c == '7' || c == '9') ? 4 : 3;
        int &res = cnt == 4 ? memo1[i] : memo2[i];
        if (res != -1) {
            return res;
        }
        res = 0;
        for (int j = 1; j <= cnt; j++) {
            res = (res + dfs(i - j, c)) % MOD;
        }
        return res;
    };
    long long ans = 0;
    int start_id = 0;
    char start = pressedKeys[0];
    for (int i = 0; i < n; i++) {
        char &c = pressedKeys[i];
        if (start != c || i == n - 1) {
            int target = i - start_id;
            if (start == c) {
                target += 1;
            }
            ans = (ans + (long long)dfs(target, start)) % MOD;
            start = c;
            start_id = i;
        }
    }
    return ans;

}
}

#endif // _7DYNAMICPROGRAMMING_H
