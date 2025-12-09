#ifndef _2HOT1___H
#define _2HOT1___H

#include "00solution.h"

#include <unordered_set>

namespace hot100 {
/// 哈希
// 1,两数之和
vector<int> twoSum(vector<int>& nums, int target) {
    // 枚举j(右),寻找i(左)
    // idx存储左边数字和最近的下标
    unordered_map<int, int> idx;
    for (int j = 0; j < nums.size(); j++) {
        int val = nums[j];
        auto it = idx.find(target - val);
        if (it != idx.end()) {
            return {it->second, j};
        }
        idx[val] = j;
    }
    return {};
}

// 49,字母异位词分组
vector<vector<string>> groupAnagrams(vector<string>& strs) {
    // sorted_string, 实际string
    // 字母异位词按字母排序后相等
    unordered_map<string, vector<string>> map;
    for (auto &s : strs) {
        string sorted_s = s;
        ranges::sort(sorted_s);
        map[sorted_s].push_back(s);
    }
    vector<vector<string>> ans;
    for (auto &it : map) {
        ans.push_back(it.second);
    }
    return ans;
}

// 128,最长连续序列
int longestConsecutive(vector<int>& nums) {
    unordered_set<int> st(nums.begin(), nums.end());
    int ans = 0;
    for (auto x : st) {
        // x不是序列起点,跳过
        if (st.contains(x - 1)) {
            continue;
        }
        // x是序列起点
        // y起点是x还是x+1都可以
        int y = x;
        // 进去循环,最后一个y是不合条件的, 区间是[x, y-1]对应y-1-x+1个
        while (st.contains(y)) {
            y++;
        }
        ans = max(ans, y - x);
    }
    return ans;
}

/// 双指针
// 283,移动零
void moveZeroes(vector<int>& nums) {
    // 非0就压入栈
    int stack_size = 0;
    for (auto &x : nums) {
        if (x) {
            nums[stack_size++] = x;
        }
    }
    fill(nums.begin() + stack_size, nums.end(), 0);
}
void moveZeroes_2(vector<int>& nums) {
    int i0 = 0;
    for (auto &x : nums) {
        if (x) {
            swap(x, nums[i0]);
            i0++;
        }
    }
}

// 11,盛最多水的容器, TwoPointersTowards2
// 15,三数之和, TwoPointersTowards1
// 42,接雨水, TwoPointersTowards2

/// 滑动窗口
// 3,无重复字符的最长子串长度, SlidingWindow

// 438,找到字符串中所有字母异位词
vector<int> findAnagrams(string s, string p) {
    // 只有小写字母, 数组大小26
    auto check = [&](int cnt_s[], int cnt_p[]) {
        for (int i = 0; i < 26; i++) {
            if (cnt_p[i] != cnt_s[i]) {
                return false;
            }
        }
        return true;
    };
    int cnt_p[26];
    for (auto &c : p) {
        cnt_p[c - 'a']++;
    }
    vector<int> ans;
    int cnt_s[26]{0};
    // 定长滑动窗口
    for (int right = 0; right < s.size(); right++) {
        cnt_s[s[right] - 'a']++;
        int left = right - p.size() + 1;
        if (left < 0) {
            // 窗口没满跳过
            continue;
        }
        if (check(cnt_s, cnt_p)) {
            ans.push_back(left);
        }
        cnt_s[s[left] - 'a']--;// 左端点离开窗口
    }
    return ans;
}
/// 子串

/// 普通数组

/// 矩阵

/// 链表

/// 二叉树

/// 图论

/// 回溯

/// 二分查找

/// 栈

/// 堆

/// 贪心算法

/// 动态规划

/// 多维动态规划

/// 技巧

}
#endif // _2HOT1___H
