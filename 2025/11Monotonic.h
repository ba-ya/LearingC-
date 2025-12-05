#ifndef _1MONOTONIC_H
#define _1MONOTONIC_H

#include "00solution.h"

#include <stack>
namespace Monotonic_Stack {
// 739每日温度,
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
        while (!st.empty() && right_h >= height[st.top()]) {
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
        while (!st.empty() && num > st.top()) {
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
        while (!st.empty() && h > st.top()) {
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

// 84柱状图中最大的矩形
int largestRectangleArea(vector<int>& heights) {
    // 枚举每个height,
    // 找出左边下一个更小, 右边下一个更小
    int n = heights.size();

    stack<int> st;
    vector<int> left(n, -1);
    for (int i = 0; i < n; i++) {
        int h = heights[i];
        // 从左往右, 弹出的都是把h当作下个更小元素的,已经可以确定身份了
        while (!st.empty() && h <= heights[st.top()]) {
            st.pop();
        }
        // 剩下的都是小于h的, 不能作为矩形区域的一部分
        if (!st.empty()) {
            left[i] = st.top();
        }
        st.push(i);
    }

    st = stack<int>();
    vector<int> right(n, n);
    for (int i = n - 1; i >= 0; i--) {
        int h = heights[i];
        while (!st.empty() && h <= heights[st.top()]) {
            st.pop();
        }
        if (!st.empty()) {
            right[i] = st.top();
        }
        st.push(i);
    }
    qDebug() << "-------" << __FUNCTION__;
    int ans = 0;
    for (int i = 0; i < n; i++) {
        ans = max(ans, heights[i] * (right[i] - left[i] - 1));
        qDebug().noquote() << QString("%1 : %2 * (%3 - %4 - 1) = %5")
                                  .arg(i).arg(heights[i]).arg(right[i]).arg(left[i])
                                  .arg(heights[i] * (right[i] - left[i] - 1));
    }
    return ans;
}
int largestRectangleArea_2(vector<int>& heights) {
    int n = heights.size();
    vector<int> left(n, -1);
    vector<int> right(n, n);
    stack<int> st;
    for (int i = 0; i < n; i++) {
        int h = heights[i];
        while (!st.empty() && h <= heights[st.top()]) {
            // 如果heights存在元素,第一个元素的right对应的下标会是重复元素下标
            // 计算面积的时候会小一个条
            // 但走到其他重复元素,会得到正确的面积
            right[st.top()] = i;
            st.pop();
        }
        if (!st.empty()) {
            left[i] = st.top();
        }
        st.push(i);
    }
    qDebug() << "-------" << __FUNCTION__;
    int ans = 0;
    for (int i = 0; i < n; i++) {
        ans = max(ans, heights[i] * (right[i] - left[i] - 1));
        qDebug().noquote() << QString("%1 : %2 * (%3 - %4 - 1) = %5")
                                  .arg(i).arg(heights[i]).arg(right[i]).arg(left[i])
                                  .arg(heights[i] * (right[i] - left[i] - 1));
    }
    return ans;
}

// 1793好子数组的最大分数
int maximumScore(vector<int>& nums, int k) {
    int n = nums.size();
    vector<int> left(n, -1);
    vector<int> right(n, n);
    stack<int> st;
    for (int i = 0; i < n; i++) {
        int x = nums[i];
        while (!st.empty() && x <= nums[st.top()]) {
            right[st.top()] = i;
            st.pop();
        }
        if (!st.empty()) {
            left[i] = st.top();
        }
        st.push(i);
    }

    int ans = 0;
    for (int i = 0; i < n; i++) {
        // left 和 right 是矩形区域外的坐标, k是在(left, right)开区间里面
        if (left[i] < k && k < right[i]) {
            ans = max(ans, nums[i] * (right[i] - left[i] - 1));
        }
    }
    return ans;
}

// 85最大矩形
int maximalRectangle(vector<vector<char>>& matrix) {
    auto get_max_rectangle = [&](vector<int> heights) {
        int n = heights.size();
        stack<int> st;
        vector<int> left(n, -1);
        vector<int> right(n, n);
        for (int i = 0; i < n; i++) {
            int h = heights[i];
            while (!st.empty() && h <= heights[st.top()]) {
                right[st.top()] = i;
                st.pop();
            }
            if (st.size()) {
                left[i] = st.top();
            }
            st.push(i);
        }
        int ans = 0;
        for (int i = 0; i < n; i++) {
            ans = max(ans, heights[i] * (right[i] - left[i] -1));
        }
        return ans;
    };
    int ans = 0;
    int m = matrix.size();
    int n = matrix[0].size();
    // 每一行按照84求一次最大矩形面积
    vector<int> heights(n, 0);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] != '0') {
                heights[j]++;
            } else {
                heights[j] = 0;
            }
        }
        ans = max(ans, get_max_rectangle(heights));
    }
    return ans;
}
};
namespace Monotonic_Queue {

};
#endif // _1MONOTONIC_H
