#ifndef _1MONOTONIC_H
#define _1MONOTONIC_H

#include "00solution.h"

#include <stack>
namespace Monotonic_Stack {
// 739每日温度
vector<int> dailyTemperatures(vector<int>& temperatures) {
    // 从左到右
    int n = temperatures.size();
    vector<int> ans(n);
    stack<int> st;
    for (int i = 0; i < n; i++) {
        int t = temperatures[i];
        while (!st.empty() && t > temperatures[st.top()]) {
            int j = st.top();
            st.pop();
            ans[j] = i - j;
        }
        st.push(i);
    }
    return ans;
}

// 42接雨水
int trap(vector<int>& height) {
    // 从左到右,横着一条条看
    int n = height.size();
    stack<int> st;
    int ans = 0;
    for (int i = 0; i < n; i++) {
        int right_h = height[i];
        // 等号处理重复元素
        while (!st.empty() && height[st.top()] <= right_h) {
            int h = height[st.top()];
            st.pop();
            if (st.empty()) {
                break;
            }
            int j = st.top();
            int left_h = height[j];
            int dh = min(left_h, right_h) - h;
            ans += (i - j - 1) * dh;
        }
        st.push(i);
    }
    return ans;
}

// 496下一个更大的元素1
vector<int> nextGreaterElement(vector<int>& nums1, vector<int>& nums2) {
    // <nums,下一个更大的元素>
    unordered_map<int, int> values;
    stack<int> st;
    for (auto num : nums2) {
        while (!st.empty() && st.top() < num) {
            values[st.top()] = num;
            st.pop();
        }
        st.push(num);
    }

    vector<int> ans;
    for (auto num : nums1) {
        if (!values.contains(num)) {
            ans.push_back(-1);
            continue;
        }
        ans.push_back(values.at(num));
    }
    return ans;
}

// 503下一个更大元素2
vector<int> nextGreaterElements(vector<int>& nums) {
    int n = nums.size();
    stack<int> st;
    vector<int> ans(n, -1);
    for (int i = 0; i < 2 * n; i++) {
        int id = i % nums.size();
        while (!st.empty() && nums[id] > nums[st.top()]) {
            ans[st.top()] = nums[id];
            st.pop();
        }
        if (i < n) {
            st.push(id);
        }
    }
    return ans;
}

// 901股票价格跨度
class StockSpanner {
    // 上一个更大元素(不能相等)的位置, stack里面只存大值
    // 天数下标, 该天价格
    stack<pair<int, int>> st;
    int cur_day = -1;
public:
    StockSpanner() {
        st.emplace(-1, INT_MAX);
    }

    int next(int price) {
        while (price >= st.top().second) {
            st.pop();
        }
        cur_day++;
        int ans = cur_day - st.top().first;
        st.push({cur_day, price});
        return ans;
    }
};

// 1019链表中的下一个更大节点
vector<int> nextLargerNodes(ListNode* head) {
    vector<int> ans;
    // 下标, 对应的值
    stack<pair<int, int>> st;
    for (auto cur = head; cur; cur = cur->next) {
        int x = cur->val;
        while (!st.empty() && x > st.top().second) {
            ans[st.top().first] = x;
            st.pop();
        }
        st.push({ans.size(), x});
        // 占位,防止下一次while数组id越界
        ans.push_back(0);
    }
    return ans;
}

// 1944队列中可以看到的人数
vector<int> canSeePersonsCount(vector<int>& heights) {
    // 从右往左看
    int n = heights.size();
    vector<int> ans(n);
    stack<int> st;
    for (int i = n - 1; i >= 0; i--) {
        int h = heights[i];
        // h[j]比h[i]大, 说明h[i]会被h[j]挡住,要停止
        while (!st.empty() && st.top() < h) {
            // 栈顶小于h[i], 满足条件,可以看见
            ans[i] += 1;
            st.pop();
        }
        if (!st.empty()) {
            ans[i] += 1;
        }
        st.push(h);
    }
    return ans;
}
};
namespace Monotonic_Queue {

};
#endif // _1MONOTONIC_H
