#ifndef _1MONOTONIC_H
#define _1MONOTONIC_H

#include "00solution.h"

#include <stack>
namespace Monotonic_Stack {
// 739, 每日温度,
vector<int> dailyTemperatures(vector<int>& temperatures) {
    // 下一个更大, 递减栈
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

// 42, 接雨水
int trap(vector<int>& height) {
    // 前后缀是竖着看, 单调栈是横着看
    // 从左到右,横着一条条看
    int n = height.size();
    stack<int> st;
    int ans = 0;
    for (int i = 0; i < n; i++) {
        int right_h = height[i];
        // 等号处理重复元素, 有相等的就会把旧值pop出来,新值push进去,
        // 从而保存栈中没有下标连续的重复元素
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

// 496, 下一个更大的元素1
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

// 503, 下一个更大元素2
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

// 901, 股票价格跨度
class StockSpanner {
    // 上一个更大元素(不能相等)的位置, stack里面只存大值, 递减栈
    // 天数下标, 该天价格
    stack<pair<int, int>> st;
    int cur_day = -1;
public:
    StockSpanner() {
        st.emplace(-1, INT_MAX);
    }

    int next(int price) {
        // 求上一个更大或相等的元素， 这里是>=
        while (price >= st.top().second) {
            st.pop();
        }
        cur_day++;
        int ans = cur_day - st.top().first;
        st.push({cur_day, price});
        return ans;
    }
};

// 1019, 链表中的下一个更大节点
vector<int> nextLargerNodes(ListNode* head) {
    // 递减栈
    vector<int> ans;
    // 下标, 对应的值, 缺少信息就可以向st里放pair
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

// 1944, 队列中可以看到的人数
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
vector<int> canSeePersonsCount_2(vector<int>& heights) {
    // 从左往右看
    // 下一个更大元素
    int n = heights.size();
    vector<int> ans(n, 0);
    stack<int> st;
    for (int i = 0; i < n; i++) {
        int h = heights[i];
        while (!st.empty() && heights[st.top()] < h) {
            // h对应的可以看见
            ans[st.top()] += 1;
            st.pop();
        }
        // 特殊情况, 栈顶 > h > 中间
        // 虽然此时栈顶比h大,但因为中间都小,所以可以看见h
        if (!st.empty()) {
            ans[st.top()] += 1;
        }
        st.push(i);
    }
    return ans;
}

// 84, 柱状图中最大的矩形
int largestRectangleArea(vector<int>& heights) {
    // 枚举每个height,
    // 找出左边下一个更小, 右边下一个更小
    int n = heights.size();

    stack<int> st;
    vector<int> left(n, -1);
    for (int i = 0; i < n; i++) {
        int h = heights[i];
        // 从左往右, 弹出的都是把h当作下个更小元素的,已经可以确定身份了
        while (!st.empty() && heights[st.top()] >= h) {
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
        while (!st.empty() && heights[st.top()] >= h) {
            // 如果heights存在元素,第一个元素的right对应的下标会是重复元素下标
            // 计算面积的时候会小一个条
            // 但走到其他重复元素,会得到正确的面积
            // 对st.top()来说, i是下一个更小的
            right[st.top()] = i;
            st.pop();
        }
        if (!st.empty()) {
            // 对i来说, st.top是上一个更小的
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
int largestRectangleArea_3(vector<int>& heights) {
    int ans = 0;
    // 枚举height,作为矩形最右边的根
    heights.push_back(-1);
    int n = heights.size();
    stack<int> st;
    st.push(-1);
    for (int right = 0; right < n; right++) {
        int r_h = heights[right];
        // 计算多次,每个可能的答案都算一次ans
        // right必须是st栈顶下一个更小的
        while (st.size() > 1 && r_h < heights[st.top()]) {
            int i = st.top();
            int h = heights[i];// 矩形的高
            st.pop();
            int left = st.top();
            ans = max(ans, h * (right - left - 1));
        }
        st.push(right);
    }
    return ans;
}

// 1793, 好子数组的最大分数
int maximumScore(vector<int>& nums, int k) {
    // 上/下一个更小的, 递增栈
    // left和right是开区间更小的,对于中间区间nums[i]是最小的
    int n = nums.size();
    vector<int> left(n, -1);
    vector<int> right(n, n);
    stack<int> st;
    for (int i = 0; i < n; i++) {
        int x = nums[i];
        // 新来的x小,说明栈顶的下一个更小就找到了,可以弹出
        // >= 可以让栈内元素无重复元素
        while (!st.empty() && nums[st.top()] >= x) {
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
        // (left, right) -> [l, r]
        int l = left[i] + 1;
        int r = right[i] - 1;
        if (l <= k && k <= r) {
            ans = max(ans, nums[i] * (r - l + 1));
        }
    }
    return ans;
}

// 85, 最大矩形
int maximalRectangle(vector<vector<char>>& matrix) {
    auto get_max_rectangle = [&](vector<int> heights) {
        int n = heights.size();
        stack<int> st;
        vector<int> left(n, -1);
        vector<int> right(n, n);
        for (int i = 0; i < n; i++) {
            int h = heights[i];
            while (!st.empty() && heights[st.top() >= h]) {
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
// 239, 滑动窗口最大值
vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    int n = nums.size();
    vector<int> ans(n - k + 1);
    deque<int> q;
    for (int i = 0; i < n; i++) {
        int x = nums[i];
        // 1.右边入
        while (!q.empty() && nums[q.back()] <= x) {
            q.pop_back();
        }
        q.push_back(i);
        // 2.左边出
        int left = i - k + 1;
        if (q.front() < left) {
            q.pop_front();
        }
        // 3.填入答案
        if (left >= 0) {
            ans[left] = nums[q.front()];
        }
    }
    return ans;
}

// 2398预算内最多的机器人数量
int maximumRobots(vector<int>& chargeTimes, vector<int>& runningCosts, long long budget) {
    int n = chargeTimes.size();
    deque<int> q;
    int ans = 0, left = 0;
    long long sum = 0;
    for (int right = 0; right < n; right++) {
        // 入,
        while (q.size() && chargeTimes[q.back()] <= chargeTimes[right]) {
            q.pop_back();
        }
        q.push_back(right);
        sum += runningCosts[right];

        // 出
        while (q.size() && chargeTimes[q.front()] + (right - left + 1) * sum > budget) {
            if (q.front() == left) {
                q.pop_front();
            }
            sum -= runningCosts[left++];
        }

        // 答案
        ans = max(ans, right - left + 1);
    }
    return ans;
}

// 862和至少为k的最短子数组
int shortestSubarray(vector<int>& nums, int k) {
    // 求和小于target的最长子数组(X)
    // 有负数,并不是数越多,sum越大,不能这样写
    int n = nums.size();
    // 前缀和s[i] = 表示[0, i-1]所有数字的和
    // [0, i - 1] - [0, j - 1]
    // s[i] - s[j] 表示[j, i)
    vector<long> s(n + 1);
    s[0] = 0L;
    for (int i = 0; i < n; i++) {
        s[i + 1] = s[i] + nums[i];
    }

    int ans = n + 1;
    deque<int> q;
    // [left, right), 个数是right - left
    // 这里位置可以取到n
    for (int i = 0; i <= n; i++) {
        long x = s[i];
        while (q.size() && x - s[q.front()] >= k) {
            ans = min(ans, i - q.front());
            q.pop_front();
        }
        while (q.size() && s[q.back()] >= x) {
            q.pop_back();
        }
        q.push_back(i);
    }
    return ans > n ? -1 : ans;
}

// 1499满足不等式的最大值
int findMaxValueOfEquation(vector<vector<int>>& points, int k) {
    int ans = INT_MIN;
    // yi + yj + |xi - xj|
    // = yi + yj + xj - xi
    // = xj + yj + (yi - xi), 枚举j,
    deque<pair<int, int>> q;
    for (auto &p : points) {
        int x = p[0];
        int y = p[1];
        // 处理front, front < x - k, 出界了
        while (q.size() && q.front().first < x - k) {
            q.pop_front();
        }
        if (q.size()) {
            ans = max(ans, x + y + q.front().second);
        }
        // 处理back, 栈顶比不过(<=)新来的就要出栈
        while (q.size() && q.back().second <= y - x) {
            q.pop_back();
        }
        q.push_back({x, y - x});
    }
    return ans;
}

// 1696跳跃游戏4
int maxResult(vector<int>& nums, int k) {
    // 爬楼梯, 不同情况加起来
    // 但这个数据量比较大,需要优化一点, 改成单调队列
    int n = nums.size();
    vector<int> f(n);
    f[0] = nums[0];
    deque<int> q{0};// 队列里也有个下标0
    // f[0]已经有值了,下面从1开始算
    for (int i = 1; i < n; i++) {
        // 递推
        // f[i] = *max_element(f.begin() + max(i - k, 0), f.begin() + i) + nums[i];

        // 出
        while (q.size() && q.front() < i - k) {
            q.pop_front();
        }
        // 更新, q的首元素是最大的情况,且合法(存在front元素)
        f[i] = f[q.front()] + nums[i];
        // 入
        while (q.size() && f[q.back()] <= f[i]) {
            q.pop_back();
        }
        q.push_back(i);
    }
    return f[n - 1];
}

// 2944购买水果需要的最少金币数
int minimumCoins(vector<int>& prices) {
    int n = prices.size();
    vector<int> memo(n, -1);
    // dfs(i)表示购买第i个水果,获得第i个水果与其后面所有水果的金币数
    // i从1开始
    auto dfs = [&](this auto &&dfs, int i) -> int {
        // 购买i,能获得[i, 2i]之间所有水果
        // 2i >= n 即 i >= n/2向上取整 = (n - 1) / 2 + 1
        if (i >= (n + 1) / 2) {
            return prices[i - 1];
        }
        int &res = memo[i];
        if (res != -1) {
            return res;
        }
        int mn = INT_MAX;
        for (int j = i + 1; j <= 2 * i + 1; j++) {
            mn = min(mn, dfs(j));
        }
        res = mn + prices[i - 1];
        return res;
    };
    return dfs(1);
}
};
#endif // _1MONOTONIC_H
