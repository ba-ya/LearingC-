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
    const int n = stones.size();
    // 总共n堆, 减少n-1堆能变成1堆
    // 每次合并能减少k-1堆
    if ((n - 1) % (k - 1)) {
        return  -1;
    }
    // 1堆的时候可以这样算
    // 倒推回最后一次合成,是k堆加上纯石头成本
    // dfs(i, j, 1) = dfs(i, j, k) + sum[i, j] //前缀和s[j + 1] - s[i]
    // s[i]表示[0, i - 1]堆的所有和
    int s[n + 1];
    s[0] = 0;
    for (int i = 0; i < n; i++) {
        s[i + 1] = s[i] + stones[i];
    }
    int memo[n][n][k+1];
    memset(memo, -1, sizeof(memo));
    auto dfs = [&](this auto &&dfs, int i, int j, int p) -> int {
        int &res = memo[i][j][p];
        if (res != -1) {
            return res;
        }
        if (p == 1) {
            return res = i == j ? 0 : dfs(i, j, k) + s[j + 1] - s[i];
        }
        // 一般情况, 分两部分, 一个一'堆',一个k-1'堆', 分(k - 1)堆到第一部分
        // dfs(i, j, k) = dfs(i, m, 1) + dfs(m + 1, j, k - 1)
        res = INT_MAX;
        for (int m = i; m < j; m += (k - 1)) {
            res = min(dfs(i, m, 1) + dfs(m + 1, j, p - 1), res);
        }
        return res;
    };
    return dfs(0, n - 1, 1);
}

};
namespace DP_Tree {
// 543二叉树的直径
int diameterOfBinaryTree(TreeNode* root) {
    // 链长:当前节点到叶子节点的路径, 空节点链长为-1, 叶子节点链长为0
    // dfs返回的是左右链中最长的链长
    int ans = 0;
    auto dfs = [&](this auto &&dfs, TreeNode *root) ->int {
        if (root == nullptr) {
            return -1;
        }
        int l_len = dfs(root->left) + 1;
        int r_len = dfs(root->right) + 1;
        ans = max(ans, l_len + r_len);
        return max(l_len, r_len);
    };
    dfs(root);
    return ans;
}

// 124二叉树的最大路径和
int maxPathSum(TreeNode* root) {
    // 链长: 下面某个节点(这里可以不用是叶子节点)到当前节点的路径, 节点是负值是时可以不用加入路径
    // dfsf返回的是最大的链和
    int ans = INT_MIN;
    auto dfs = [&](this auto &&dfs, TreeNode *root) -> int {
        if (root == nullptr) {
            return 0;// 没有节点,总和是0
        }
        int l_len = dfs(root->left);// 左子树最大链和
        int r_len = dfs(root->right);// 右子树最大链和
        ans = max(ans, l_len + r_len + root->val);
        return max(0, max(l_len, r_len) + root->val);// 当前子树最大链和(加上本身)
    };
    dfs(root);
    return ans;
}

// 2246相邻字符的不同的最长路径
int longestPath(vector<int>& parent, string s) {
    // 相邻字符只存在于有父子关系的字符之间
    int n = parent.size();
    vector<vector<int>> g(n);
    // i = 0, 对应的parent[0] = -1, 表示是根节点
    // 某个父节点下面所有的子节点下标
    for (int i = 1; i < n; i++) {
        g[parent[i]].emplace_back(i);
    }

    // 最长的链长
    int ans = 0;
    // x表示传入父节点的下标, 得到的是以x为父节点的最大链长
    auto dfs = [&](this auto &&dfs, int x) -> int {
        // 相当于当前节点的链长是0
        int max_len = 0;
        // 枚举所有子节点
        for (int y : g[x]) {
            // 子节点y下面的链长 + x到y的路径
            int len = dfs(y) + 1;
            if (s[x] != s[y]) {
                // ans需要的左边链+右边链
                ans = max(ans, max_len + len);
                // 维护最长的链
                max_len = max(max_len, len);
            }
        }
        return max_len;
    };
    // 根节点开始
    dfs(0);
    return ans + 1;//节点个数
}

// 687最长同值路径
int longestUnivaluePath(TreeNode* root) {
    // 自己写对了,但加了两个变量sum 和 max_len
    // 换成参考的,修改l_len, r_len
    int ans = 0;
    auto dfs = [&](this auto &&dfs, TreeNode *node) -> int {
        if (node == nullptr) {
            return -1;
        }
        int l_len = dfs(node->left) + 1;
        int r_len = dfs(node->right) + 1;
        if (node->left && node->val != node->left->val) l_len = 0;
        if (node->right && node->val != node->right->val) r_len = 0;
        ans = max(ans, l_len + r_len);
        return max(l_len, r_len);
    };
    dfs(root);
    return ans;
}

int diameter(vector<vector<int>>& edges) {
    // 不懂为啥这里要+1
    vector<vector<int>> g(edges.size() + 1);
    for (auto e : edges) {
        int x = e[0];
        int y = e[1];
        // 不知道父子关系,需要都放进来
        g[x].emplace_back(y);
        g[y].emplace_back(x);
    }

    int res = 0;
    // 返回最大链长
    auto dfs = [&](this auto &&dfs, int x, int parent) -> int {
        int max_len = 0;
        for (auto y : g[x]) {
            // 不是'父'节点, 求下一级的len
            if (y != parent) {
                // 以x作为父母节点,枚举y
                int sub_len = dfs(y, x) + 1;
                res = max(res, max_len + sub_len);
                max_len = max(max_len, sub_len);
            }
        }
        return max_len;
    };
    dfs(0, -1);
    return res;
}
// 3203合并两棵树的最小直径
int minimumDiameterAfterMerge(vector<vector<int>>& edges1, vector<vector<int>>& edges2) {
    int d1 = diameter(edges1);
    int d2 = diameter(edges2);
    // d1, d2, d1 / 2 向上取整 + d2 / 2 向上取整 + 1
    // d1 / 2 = (d1 - 1) / 2 + 1 = (d1 + 1) / 2
    return max(max(d1, d2), (d1 + 1) / 2 + (d2 + 1) / 2 + 1 );
}

// 1617统计子树中城市之间的最大距离
vector<int> countSubgraphsForEachDiameter(int n, vector<vector<int>>& edges) {
    vector<vector<int>> g(n);
    // 编号从1开始, 调整为从0开始
    for (auto e : edges) {
        int x = e[0] - 1;
        int y = e[1] - 1;
        g[x].emplace_back(y);
        g[y].emplace_back(x);
    }

    // vis, 只有对应的is_set存在,才可以参与计算直径
    // is_set, 选与不选得到的子集
    vector<int> ans(n - 1), vis(n), is_set(n);
    int diameter = 0;
    // 返回最大链长, 更新直径
    auto dfs = [&](this auto &&dfs, int x) -> int {
        vis[x] = true;
        int max_len = 0;
        for (auto y : g[x]) {
            // 求其下面子节点的链长
            if (!vis[y] && is_set[y]) {
                int len = dfs(y) + 1;
                diameter = max(diameter, max_len + len);
                max_len = max(max_len, len);
            }
        }
        return max_len;
    };

    // 选与不选,枚举子集
    auto f = [&](this auto &&f, int i) ->void {
        if (i == n) {
            // 1. 找到第一个被选中的节点 v（作为 DFS 起点）
            for (int v = 0; v < n; v++) {
                if (is_set[v]) {
                    // 2. 重置访问标记和直径
                    fill(vis.begin(), vis.end(), 0);
                    diameter = 0;
                    // 3. 从 v 开始 DFS，计算该子图的直径
                    // 为什么只从一个点开始？因为如果子图是连通的，从任意一点 DFS 都能遍历整个子图；
                    // 如果不连通，则 vis != is_set，会被后续条件过滤掉。
                    dfs(v);
                    break;
                }
            }
            // 4. 检查：这个子集是否非空、连通、且直径有效
            if (diameter && vis == is_set) {
                // ans[0]对应的是直径为1的情况
                ans[diameter - 1] += 1;
            }
            return;
        }

        // 不选
        f(i + 1);

        // 选
        is_set[i] = true;
        f(i + 1);
        is_set[i] = false;
    };
    f(0);
    return ans;
}

// 2538最大价值与最小价值和的差值
long long maxOutput(int n, vector<vector<int>>& edges, vector<int>& price) {
    vector<vector<int>> g(n);
    for (auto &e : edges) {
        int x = e[0];
        int y = e[1];
        g[x].emplace_back(y);
        g[y].emplace_back(x);
    }

    long long ans;
    auto dfs = [&](this auto &&dfs, int x, int parent) -> pair<long long, long long> {
        long long p = price[x];
        // 两个max, 一个有叶子节点,一个不含叶子节点
        long long max_s1 = p;
        long long max_s2 = 0;
        for (auto y : g[x]) {
            if (y != parent) {
                auto [s1, s2] = dfs(y, x);
                ans = max(ans, max(max_s1 + s2, s1 + max_s2));
                // 此时叶子节点在更底层,与x节点无关
                max_s1 = max(max_s1, s1 + p);
                max_s2 = max(max_s2, s2 + p);
            }
        }
        return {max_s1, max_s2};
    };
    dfs(0, -1);
    return ans;
}

};
namespace DP_MaxSet {

};
namespace DP_MinSet {

};

#endif // __DP_H
