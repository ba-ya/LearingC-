#ifndef SLIDINGWINDOW_H
#define SLIDINGWINDOW_H

#include "00solution.h"

namespace SlidingWindow {
// 209长度最小的子数组(>=target)
int minSubArrayLen(int target, vector<int>& nums) {
    int n = nums.size();
    int ans = n + 1;
    int left = 0;
    int s = 0;
    for (int right = 0; right < n; ++right) {
        s += nums[right];
        // 满足条件就要更新一下ans,
        // 满足->不满足
        while (s >= target) {
            ans = min(ans, right - left + 1);
            s -= nums[left];
            left++;
        }
    }
    return ans <= n ? ans : 0;
}

// 3无重复字符的最长子串长度
int lengthOfLongestSubstring(string s) {
    int ans = 0;
    int n = s.size();
    int left = 0;
    map<char, int> hash;
    for (int right = 0; right < n; ++right) {
        hash[s[right]] += 1;
        // 不满足->满足
        while (hash[s[right]] > 1) {
            hash[s[left]] -= 1;
            left++;
        }
        ans = max(ans, right - left + 1);
    }
    return ans;
}

// 713乘积小于K的子数组数目, 都是正数
int numSubarrayProductLessThanK(vector<int>& nums, int k) {
    // 或者while (prod >= k) {->while (left <= right && prod >= k)
    if (k <= 1) {
        return 0;
    }
    int n = nums.size();
    int ans = 0;
    int left = 0;
    int prod = 1;
    for (int right = 0; right < n; ++right) {
        prod *= nums[right];
        while (prod >= k) {
            prod /= nums[left];
            left++;
        }
        ans += right - left + 1;
    }
    return ans;
}

// 2958最多K个重复元素的最长子数组
int maxSubarrayLength(vector<int>& nums, int k) {
    int ans = 0;
    int n = nums.size();
    // nums, count
    map<int, int> freq;
    int left = 0;
    for (int right = 0; right < n; ++right) {
        int id_r = nums[right];
        freq[id_r] += 1;
        while (freq[id_r] > k) {
            int id_l = nums[left];
            freq[id_l] -= 1;
            left++;
        }
        ans = max(ans, right - left + 1);
    }
    return ans;
}

// 2730找到最长的半重复子字符串, 参考
int longestSemiRepetitiveSubstring(string s) {
    // 至少有一个字符
    int ans = 1;
    int same = 0;
    int left = 0;
    for (int right = 1; right < s.size(); ++right) {
        // 新加进来的判断一下
        if (s.at(right) == s.at(right - 1)) {
            same += 1;
        }
        if (same > 1) {
            left++;
            // 移除左边两个重复字符的第一个字符
            while (s[left] != s[left - 1]) {
                left++;
            }
            same = 1;
        }
        ans = max(ans, right - left + 1);
    }
    return ans;
}

// 2779数组的最大美丽值,参考
int maximumBeauty(vector<int>& nums, int k) {
    int ans = 0;
    sort(nums.begin(), nums.end());
    int left = 0;
    for (int right = 0; right < nums.size(); right++) {
        // while 不满足
        // 满足条件是 x+k >= y-k ->  y - x <= 2k,
        // 最左边的最大值大于最右边的最小值,即存在交集
        while (nums[right] - nums[left] > 2 * k) {
            left++;
        }
        ans = max(ans, right - left + 1);
    }
    return ans;
}

// 1004最大连续1的个数, own + 参考
int longestOnes(vector<int>& nums, int k) {
    // 方法1, 根据翻转次数,自己写的
#if 0
        int ans = 0;
        int flip = 0;
        int left = 0;
        for (int right = 0; right < nums.size(); ++right) {
            if (nums[right] == 0) {
                flip++;
            }
            if (flip > k) {
                while (left < right && nums[left] == 1) {
                    left++;
                }
                left++;
                flip--;
            }
            ans = max(ans, right - left + 1);
        }
        return ans;
#endif

    // 方法2, 转化为子数组内0的个数<=k, 参考
    int ans = 0;
    int cnt0 = 0;
    int left = 0;
    for (int right = 0; right < nums.size(); ++right) {
        cnt0 += 1 - nums[right];
        while (cnt0 > k) {
            cnt0 -= 1 - nums[left];
            left++;
        }
        ans = max(ans, right - left + 1);
    }
    return ans;

}

// 2962统计最大元素出现至少K次的子数组,
// 思路有,但while和ans+的地方一开始写错了
long long countSubarrays(vector<int>& nums, int k) {
    long long ans = 0;
    // target max elment
    auto it = max_element(nums.begin(), nums.end());
    int target = *it;
    // slide window
    long long left = 0;
    int cnt = 0;
    for (long long right = 0; right < nums.size(); ++right) {
        cnt += nums[right] == target;
        // 满足->不满足
        while (cnt == k) {
            cnt -= nums[left] == target;
            left++;
        }
        // 当前[left - 1, right]数组是满足条件的最小数组
        // 前面可以加上0,1,..., left - 2
        // 即0到left-1个
        ans += left;
    }
    return ans;
}

// 2302统计得分小于K的子数组数目
long long countSubarrays(vector<int>& nums, long long k) {
    long long ans = 0;
    long long left = 0;
    long long score = 0;
    for (long long right = 0; right < nums.size(); ++right) {
        score += nums[right];
        while (score * (right - left + 1) >= k) {
            score -= nums[left];
            left++;
        }
        ans += right - left + 1;
    }
    return ans;
}

// 1658将x减到0的最小操作数, 参考
int minOperations(vector<int>& nums, int x) {
    int n = nums.size();
    // 逆向思考,求满足 nums之和 - x 的最长子数组
    int target = reduce(nums.begin(), nums.end(), 0) - x;
    if (target < 0) {
        return -1;
    }

    int left = 0;
    int ans = -1;// 求最长,可以以-1作为起始值
    int sum = 0;
    for (int right = 0; right < n; ++right) {
        sum += nums[right];
        while (sum > target) {
            sum -= nums[left];
            left++;
        }
        if (sum == target) {
            ans = max(ans, right - left + 1);
        }
    }
    return ans < 0 ? -1 : n - ans;
}

// 1234最小替换子串得到平衡字符串, 参考(不太懂)
int balancedString(string s) {
    // 记录的是待替换子串的出现次数
    int cnt['W' + 1]{};
    for (char c : s) {
        cnt[c]++;
    }

    int n = s.size();
    int target = n / 4;
    if (cnt['Q'] == target && cnt['W'] == target
        && cnt['E'] == target && cnt['R'] == target){
        return 0;
    }

    int ans = n, left = 0;
    for (int right = 0; right < n; right++) {
        cnt[s.at(right)]--;
        // 替换子串之外,即待替换子串都是正常的或者待填充的
        while (cnt['Q'] <= target && cnt['W'] <= target
               && cnt['E'] <= target && cnt['R'] <= target) {
            ans = min(ans, right - left + 1);
            cnt[s[left]]++;
            left++;// 从替换子串挪一个字符到待替换子串
        }
    }
    return ans;

}

// 76最小覆盖子串,参考,
// 写了版错误的,测试用例能通过144 / 268
string minWindow(string s, string t) {
    auto is_covered = [](int cnt_s[], int cnt_t[]) {
        for (int i = 'a'; i <= 'z'; ++i) {
            if (cnt_s[i] < cnt_t[i]) {
                return false;
            }
        }
        for (int i = 'A'; i <= 'Z'; ++i) {
            if (cnt_s[i] < cnt_t[i]) {
                return false;
            }
        }
        return true;
    };
    // 128是asc码的数目
    int cnt_s[128]{0};
    int cnt_t[128]{0};
    for (char c : t) {
        cnt_t[c] += 1;
    }

    int ans_left = -1;
    int ans_right = s.size();
    int left = 0;
    for (int right = 0; right < s.size(); right++) {
        cnt_s[s.at(right)] += 1;
        // 笔误,之前两个参数都写成cnt_s了
        while (is_covered(cnt_s, cnt_t)) {
            if (right - left < ans_right - ans_left) {
                ans_right = right;
                ans_left = left;
            }
            cnt_s[s.at(left)] -= 1;
            left++;
        }
    }
    return ans_left < 0 ? "" : s.substr(ans_left, ans_right - ans_left + 1);
}
}
#endif // 02SLIDINGWINDOW_H
