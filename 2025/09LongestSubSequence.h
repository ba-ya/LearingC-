#ifndef _9LONGESTSUBSEQUENCE_H
#define _9LONGESTSUBSEQUENCE_H

#include "00solution.h"

namespace LCS {
// 1143, 最长公共子序列
// 时间复杂度:O(mn), 空间复杂度:O(mn)
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
        // 相同字符,都跳过
        if (text1[i] == text2[j]) {
            return res = dfs(i - 1, j - 1) + 1;
        }
        // 没有相同的,要没text1跳,要么text2跳
        return res = max(dfs(i, j - 1), dfs(i - 1, j));
    };
    return dfs(n - 1, m - 1);
}

// 72, 编辑距离
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

// 97, 交错字符串
bool isInterleave(string s1, string s2, string s3) {
    int n = s1.size();
    int m = s2.size();
    if (n + m != s3.size()) {
        return false;
    }
    vector<vector<int>> memo(n + 1, vector<int>(m + 1, -1));
    // 下标i, 当前剩下的个数i+1
    // 下标j, 当前剩下的个数j+1
    // 对应s3,还未确认的个数是i+j+2, 下一个要比较的是s3[i+j+1]
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

// 1092, 最短公共超序列
string shortestCommonSupersequence(string str1, string str2) {
    // 构造的序列要两个string都是它的子序列,每个字母都要包含
    int n = str1.length();
    int m = str2.length();
    vector<vector<int>> memo(n, vector<int>(m, -1));
    // 在求什么? 求最短公共超序列的长度
    auto dfs = [&](this auto &&dfs, int i, int j) -> int {
        if (i < 0) {
            return j + 1;// str1是空串,返回剩下的str2
        }
        if (j < 0) {
            return i + 1;// str2是空串,返回剩下的str1
        }
        int &res = memo[i][j];
        if (res != -1) {
            return res;
        }
        if (str1[i] == str2[j]) {
            return res = dfs(i - 1, j - 1) + 1;// str1[i]
        }
        // 这里为什么要+1?
        // 不相等, str1,str2选一个字母放入目标序列中str1[i] || str2[j]
        return res = min(dfs(i - 1, j), dfs(i, j - 1)) + 1;
    };
    auto make_ans = [&](this auto &&make_ans, int i, int j) -> string {
        if (i < 0) return str1.substr(0, j + 1);
        if (j < 0) return str1.substr(0, i + 1);
        if (str1[i] == str2[j]) {
            return make_ans(i - 1, j - 1) + str1[i];
        }
        if (dfs(i, j) == dfs(i - 1, j) + 1) {
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
        int ans = 0;
        for (int j = 0; j < i; j++) {
            // nums[j]是nums[i]结尾的子序列的一部分
            // dfs(i) = dfs(j) + 1
            // 严格递增,<
            if (nums[j] < nums[i]) {
                ans = max(ans, dfs(j));
            }
        }
        // 没有更小的,说明子序列只有nums[i]自己,答案是1
        return res = ans + 1;
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
        // 求得第一个>=x的位置, 严格递增
        // 非递减应该如何改?换成upper_bound, 返回第一个大于x的位置
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
        int ans = 0;
        for (int j = 0; j < i; j++) {
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

// 1671得到山形数组的最小删除次数
int minimumMountainRemovals(vector<int>& nums) {
    // 记忆化搜索写起来清楚一点
    // 二分的写不清楚
    int n = nums.size();
    vector<int> memo_pre(n, -1), memo_suf(n, -1);
    auto dfs_pre = [&](this auto &&dfs_pre, int i) -> int {
        int &res = memo_pre[i];
        if (res != -1) {
            return res;
        }
        int ans = 0;
        for (int j = 0; j < i; j++) {
            if (nums[j] < nums[i]) {
                ans = max(ans, dfs_pre(j));
            }
        }
        return res = ans + 1;
    };
    auto dfs_suf = [&](this auto &&dfs_suf, int i) -> int {
        int &res = memo_suf[i];
        if (res != -1) {
            return res;
        }
        int ans = 0;
        // nums[i]是最大的
        for (int j = i + 1; j < n; j++) {
            if (nums[j] < nums[i]) {
                ans = max(ans, dfs_suf(j));
            }
        }
        return res = ans + 1;
    };
    int ans = 0;
    for (int i = 0; i < n; i++) {
        int pre = dfs_pre(i);
        int suf = dfs_suf(i);
        // 两个都包含封顶,计算的时候需要减掉一个
        if (pre >= 2 && suf >= 2) {
            ans = max(ans, pre + suf - 1);
        }
    }
    return n - ans;
}

// 354俄罗斯套娃信封问题
int maxEnvelopes(vector<vector<int>>& envelopes) {
    // 保证宽度和高度都是单独的,不允许多个相同宽度
    // w升序排列, h降序排列
    ranges::sort(envelopes, {},
                 [](vector<int> e) {return pair{e[0], -e[1]};});
    vector<int> g;
    for (auto &e : envelopes) {
        int h = e[1];
        // 严格递增
        auto it = ranges::lower_bound(g, h);
        if (it == g.end()) {
            g.emplace_back(h);
        } else {
            *it = h;
        }
    }
    return g.size();
}

// 1626无矛盾的最佳球队
// 贪心+二分用了答案不对, 贪心是长度最长, 这里要求分数最大,两者有矛盾
// 用动态规划
int bestTeamScore(vector<int>& scores, vector<int>& ages) {
    // score, age
    // 都按照从小到大的顺序排列
    vector<pair<int, int>> nums;
    int n = scores.size();
    for (int i = 0; i < n; i++) {
        nums.emplace_back(scores[i], ages[i]);
    }
    qDebug() << "1:" <<  nums;
    ranges::sort(nums, {}, [](pair<int, int> p) {return pair{p.first, p.second};} );
    qDebug() << "2:" << nums;

    // 记忆化搜索, 以i(第几个)为结尾的分数最大的
    vector<int> memo(n, -1);
    auto dfs = [&](this auto &&dfs, int i) -> int {
        int &res = memo[i];
        if (res != -1) {
            return res;
        }
        int ans = 0;
        for (int j = 0; j < i; j++) {
            // 确保了scores是递增的,现在只需要找到age的最大递增序列
            if (nums[j].second <= nums[i].second) {
                ans = max(ans, dfs(j));
            }
        }
        // 加上自己
        return res = ans + nums[i].first;
    };
    int ans = 0;
    for (int i = 0; i < n; i++) {
        ans = max(ans, dfs(i));
    }
    return ans;

    // // 递推
    // std::vector<int> f(n);
    // for (int i = 0; i < n; i++) {
    //     for (int j = 0; j < i; j++) {
    //         if (nums[j].second <= nums[i].second) {
    //             qDebug().noquote()
    //             << QString("f[%1]: %2").arg(i).arg(f[i])
    //             << QString(", f[%1]: %2").arg(j).arg(f[j]);
    //             f[i] = max(f[i], f[j]);
    //         }
    //     }
    //     f[i] += nums[i].first;
    // }
    // return *max_element(f.begin(), f.end());
}

// 1187使数组严格递增
int makeArrayIncreasing(vector<int>& a, vector<int>& b) {
    ranges::sort(b);
    const int n = a.size();
    vector<unordered_map<int, int>> memo(n);

    auto dfs = [&](this auto &&dfs, int i, int pre) ->int {
        if (i < 0) {
            return 0;
        }
        if (auto it = memo[i].find(pre); it != memo[i].end()) {
            return it->second;
        }
        // 不替换
        // 保证a[i] < pre, 如果可以只需要计算[0, i - 1]范围内严格递增许要的操作数
        int res = a[i] < pre ? dfs(i - 1, a[i]) : INT_MAX / 2;
        // 替换
        // a[i](即待替换的b[j])作为[0, i - 1]的pre, 要小于当前pre
        auto k = ranges::lower_bound(b, a[i]);// >= pre,need k--;
        if (k != b.begin()) {
            k--;
            res = min(res, dfs(i - 1, *k) + 1);
        }
        return res;
    };
    int res = dfs(n - 1, INT_MAX);
    return res < INT_MAX / 2 ? res : -1;
}
}
#endif // _9LONGESTSUBSEQUENCE_H
