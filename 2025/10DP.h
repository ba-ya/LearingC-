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
    int n = values.size();
    vector<vector<int>> memo(n, vector(n, -1));
    //dfs(i, j)表示从i到j顶点所以三角形的乘积
    auto dfs = [&](this auto &&dfs, int i, int j) -> int {
        // 两个点不能构成三角形,返回0
        if (i + 1 == j) {
            return 0;
        }
        int &res = memo[i][j];
        if (res != -1) {
            return res;
        }
        res = INT_MAX;
        // i, k, j
        // 只能确定以i,k,j为顶点的三角形分数
        for (int k = i + 1; k <= j - 1; k++) {
            res = min(res, values[i] * values[k] * values[j] + dfs(i, k) + dfs(k, j));
        }
        return res;
    };
    return dfs(0, n - 1);
}

// 3040相同分数的最大操作次数2
int maxOperations(vector<int>& nums) {
    int n = nums.size();
    vector<vector<int>> memo(n, vector<int>(n, -1));
    auto check = [&](int i, int j, int pre) {
        int sum = nums[i] + nums[j];
        return sum == pre;
    };
    auto dfs = [&](this auto &&dfs, int i, int j, int pre) -> int {
        if (i >= j) {
            return 0;
        }
        int &res = memo[i][j];
        if (res != -1) {
            return res;
        }
        res = 0;
        if (check(i, i + 1, pre)) {
            res = max(dfs(i + 2, j, pre) + 1, res);
        }
        if (check(j - 1, j, pre)) {
            res = max(dfs(i, j - 2, pre) + 1, res);
        }
        if (check(i, j, pre)) {
            res = max(dfs(i + 1, j - 1, pre) + 1, res);
        }
        return res;
    };
    // 初始化不对,应该把第一次操作摘出来
    // 这样才能确定target
    int res1 = dfs(2, n - 1, nums[0] + nums[1]);
    int res2 = dfs(0, n - 3, nums[n - 2] + nums[n - 1]);
    int res3 = dfs(1, n - 2, nums[0] + nums[n - 1]);
    return max(res1, max(res2, res3)) + 1;
}

// 1547切棍子的最小成本
// 时间复杂度:O(m^3), 空间复杂度:O(m^2)
// 子问题(状态)数目: ij有多少对,C(m, 2) = m! / (m - 2) ! * 2! = m * (m - 1) / 2 约等于m^2
// 每个子问题(状态)计算时间: 最差 k从0到m-1,为m
// 合起来就是 m^2 * m = m^3
int minCost(int n, vector<int>& cuts) {
    cuts.emplace_back(0);
    cuts.emplace_back(n);
    ranges::sort(cuts);
    int m = cuts.size();
    // dfs(i, j)表示以cuts[i], cuts[j]为左右端点切棍子的最小成本
    vector<vector<int>> memo(m, vector<int>(m, -1));
    auto dfs = [&](this auto &&dfs, int i, int j) -> int {
        // 只有左右端点,不能切了
        if (i + 1 == j) {
            return 0;
        }
        int &res = memo[i][j];
        if (res != -1) {
            return res;
        }
        res = INT_MAX;
        for (int k = i + 1; k < j; k++) {
            res = min(res, dfs(i, k) + dfs(k, j));
        }
        res += cuts[j] - cuts[i];
        return res;
    };
    return dfs(0, m - 1);
}

// 1771由子序列构造的最长回文串的长度
// 时间复杂度:O((n + m)^2), 空间复杂度:O((n + m)^2)
int longestPalindrome(string word1, string word2) {
    string s = word1 + word2;
    int n = s.size();
    vector<vector<int>> memo(n, vector<int>(n, -1));
    // 必须两个子串都含
    int ans = 0;
    auto dfs = [&](this auto &&dfs, int i, int j) -> int {
        int mid = word1.size();
        if (i > j) {
            return 0;// null
        }
        if (i == j) {
            return 1;// single character
        }
        int &res = memo[i][j];
        if (res != -1) {
            return res;
        }
        if (s[i] == s[j]) {
            res = 2 + dfs(i + 1, j - 1);
            // 等于说求回文子串长度还是按516求
            // 答案需要筛出来
            if (i < mid && j >= mid) {
                ans = max(ans, res);
            }
            return res;
        }
        return res = max(dfs(i + 1, j), dfs(i, j - 1));
    };
    dfs(0, n - 1);
    return ans;
}

// 1000合并石头的最低成本
int mergeStones(vector<int>& stones, int k) {
    int n = stones.size();
    if (n % k) {
        return  -1;
    }
    int ans = INT_MAX;
    vector<vector<int>> memo(n, vector<int>(n, -1));
    auto dfs = [&](this auto &&dfs, int i, int j) -> int {
        if (ans == -1) {
            return -1;
        }
        if (j - i < k) {
            ans = -1;
            return ans;
        }
        int &res = memo[i][j];
        if (res != -1) {
            return res;
        }
        res = INT_MAX;
        for (int l = i; l < i + k; l++) {
            res = min()
        }
    };
}

};
namespace DP_Tree {

};
namespace DP_MaxSet {

};
namespace DP_MinSet {

};

#endif // __DP_H
