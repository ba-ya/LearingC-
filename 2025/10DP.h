#ifndef __DP_H
#define __DP_H

#include "00solution.h"

/// 动态规划的时间复杂度 = 状态个数 × 单个状态的转移个数
namespace DP_BuyNSellStock {
// 122买卖股票的最佳时机2
int maxProfit(vector<int>& prices) {
    int n = prices.size();
    // 每个价格有hold与not hold两种状态,需要分别存储
    vector<vector<int>> memo(n, vector<int>(2, -1));
    // dfs(i, hold)表示在第i天结束时候的状态
    auto dfs = [&](this auto &&dfs, int i, bool hold) {
        if (i < 0) {
            // 第0天开始持有股票, 不合法设置成INT_MIN
            // 第0天开始不持有股票, 利润是0
            return hold ? INT_MIN : 0;
        }
        int &res = memo[i][hold];
        if (res != -1) {
            return res;
        }
        if (hold) {
            // 前一天持有, 前一天未持有但今天买入一支
            return res = max(dfs(i - 1, true), dfs(i - 1, false) - prices[i]);
        }
        // 前一天未持有, 前一天持有但今天卖出
        return res = max(dfs(i - 1, false), dfs(i - 1, true) + prices[i]);
    };
    return dfs(n - 1, false);
}

// 309买卖股票的最佳时机含冷冻期
int maxProfit_2(vector<int>& prices) {
    int n = prices.size();
    vector<vector<int>> f(n + 2, vector<int>(2));
    // 边界条件f[-1][0] = INT_MIN
    f[1][1] = INT_MIN;
    for (int i = 0; i < n; i++) {
        f[i + 2][0] = max(f[i + 1][0], f[i + 1][1] + prices[i]);
         // - prices[i]是第i天买股票, 必须要是i-2转移过来
        f[i + 2][1] = max(f[i + 1][1], f[i][0] - prices[i]);
    }
    return f[n + 1][0];
}

// 188买卖股票的最佳时机4
int maxProfit(int k, vector<int>& prices) {
    int n = prices.size();
    vector<vector<vector<int>>> memo(n, vector<vector<int>>(k + 1, vector<int>(2, -1)));
    // dfs(i, j, hold)表示在第i天结束的时候,卖出次数还有j次, 目前持有股票的状态
    auto dfs = [&](this auto &&dfs, int i, int j, bool hold) {
        // 交易次数不可能小于0
        if (j < 0) {
            return INT_MIN / 2;// 防止溢出
        }
        // 第0天持有股票是不合法的
        if (i < 0) {
            return hold ? INT_MIN / 2 : 0;
        }
        int &res = memo[i][j][hold];
        if (res != -1) {
            return res;
        }
        if (hold) {
            return res = max(dfs(i - 1, j, true), dfs(i - 1, j, false) - prices[i]);
        }
        return res = max(dfs(i - 1, j, false), dfs(i - 1, j - 1, true) + prices[i]);
    };
    return dfs(n - 1, k, false);
}

// 714买卖股票的最佳时机含手续费
int maxProfit(vector<int>& prices, int fee) {
    int n = prices.size();
    vector<vector<int>> memo(n, vector<int>(2, -1));
    auto dfs = [&](this auto &&dfs, int i, bool hold) {
        if (i < 0) {
            return hold ? INT_MIN / 2 : 0;
        }
        int &res = memo[i][hold];
        if (res != -1) {
            return res;
        }
        if (hold) {
            return res = max(dfs(i - 1, true), dfs(i - 1, false) - prices[i]);
        }
        return res = max(dfs(i - 1, false), dfs(i - 1, true) + prices[i] - fee);
    };
    return dfs(n - 1, false);
}

// 2826将三个组排序
int minimumOperations(vector<int>& nums) {
    // 看不懂DP做法,用LIS
    // LIS也忘光光了,完全照抄的
    int n = nums.size();
    vector<int> memo(n, -1);
    auto dfs = [&](this auto &&dfs, int i) -> int {
        int &res = memo[i];
        if (res != -1) {
            return res;
        }
        int ans = 0;
        for (int j = 0; j < i; j++) {
            // 非递减
            if (nums[j] <= nums[i]) {
                ans = max(ans, dfs(j));
            }
        }
        return res = ans + 1;
    };
    int ans = 0;
    for (int i = 0; i < n; i++) {
        ans = max(ans, dfs(i));
    }
    return n - ans;
}

long long maxScore(vector<int>& nums, int x) {
    int n = nums.size();
    // 在i位置是奇数, 在i位置是偶数
    vector<array<long long, 2>> memo(n, {-1, -1});
    auto dfs = [&](this auto &&dfs, int i, int j) -> long long {
        if (i == n) {
            return 0;
        }
        long long &res = memo[i][j];
        if (res != -1) {
            return res;
        }
        // 奇偶性相同,必须选,
        // [i + 1, n - 1]范围内奇偶性
        if (nums[i] % 2 == j) {
            //奇偶性相同不同管, 奇偶性不同需要-x
            return res = max(dfs(i + 1, j), dfs(i + 1, j^1) - x) + nums[i];
        }
        // 奇偶性不同, 不能选
        return res = dfs(i + 1, j);
    };
    return dfs(0, nums[0] % 2);
}

};
namespace DP_Interval {
// 516最长回文子序列
// 时间复杂度:O(n*n), 空间复杂度:O(n*n)
int longestPalindromeSubseq(string s) {
    int n = s.size();
    vector<vector<int>> memo(n, vector<int>(n, -1));
    auto dfs = [&](this auto &&dfs, int i, int j) -> int {
        if (i > j) {
            return 0;// ""
        }
        if (i == j) {
            return 1;// 单一字母
        }
        int &res = memo[i][j];
        if (res != -1) {
            return res;
        }
        // 字母相同都可以前进i+1, j-1
        if (s[i] == s[j]) {
            return res = dfs(i + 1, j - 1) + 2;
        }
        // 不相同两种选择(i+1, j) || (i, j - 1)
        return res = max(dfs(i + 1, j), dfs(i, j - 1));
    };
    return dfs(0, n - 1);
}

// 1039多边形三角剖分的最低得分
int minScoreTriangulation(vector<int>& values) {

}

};
namespace DP_Tree {

};
namespace DP_MaxSet {

};
namespace DP_MinSet {

};

#endif // __DP_H
