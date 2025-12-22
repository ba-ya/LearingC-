#ifndef _3HOT1___2_H
#define _3HOT1___2_H

#include "00solution.h"

#include <queue>
#include <stack>
#include <unordered_set>

namespace hot100_2 {
/// 图论
// 200, 岛屿数量
int numIslands(vector<vector<char>>& grid) {
    int m = grid.size();
    int n = grid[0].size();
    // 方向矩阵, 上下左右
    int DIR[4][2] = {{1, 0}, {-1, 0}, {0, -1}, {0, 1}};
    // 插旗
    // 出界, 插过旗, 遇到水
    auto dfs = [&](this auto &&dfs, int i, int j) {
        if (i < 0 || i >= m || j < 0 || j >= n || grid[i][j] != '1') {
            return;
        }
        // 先在自己陆地插上旗
        grid[i][j] = '2';
        for (int k = 0; k < 4; k++) {
            dfs(i + DIR[k][0], j + DIR[k][1]);
        }
    };
    int ans = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == '1') {
                // 陆地都插旗了
                dfs(i, j);
                ans += 1;
            }
        }
    }
    return ans;
}

// 994, 腐烂的橙子
int orangesRotting(vector<vector<int>>& grid) {
    // 上下左右
    int DIR[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    int m = grid.size();
    int n = grid[0].size();
    // 一开始就腐烂的
    vector<vector<int>> rots;
    int fresh = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == 1) {
                fresh++;
            } else if (grid[i][j] == 2) {
                rots.push_back({i, j});
            }
        }
    }
    int ans = 0;
    while (fresh && rots.size()){
        vector<vector<int>> rots_2;
        for (auto &rot : rots) {
            int i = rot[0];
            int j = rot[1];
            for (int k = 0; k < 4; k++) {
                int row = i + DIR[k][0];
                int col = j + DIR[k][1];
                if (row < 0 || row >= m || col < 0 || col >= n || grid[row][col] != 1) continue;
                fresh--;
                grid[row][col] = 2;
                rots_2.push_back({row, col});
            }
        }
        ans++;
        rots = std::move(rots_2);
    };
    return fresh ? -1 : ans;
}

// 207, 课程表
bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
    // 构建有向图
    vector<vector<int>> g(numCourses);
    // [a, b], a的前置是b, 即b->a
    for (auto &prer : prerequisites) {
        g[prer[1]].emplace_back(prer[0]);
    }

    // 0:未访问, 1:正在访问, 2:访问结束
    vector<int> colors(numCourses);
    // true,存在多个同时访问的节点
    auto dfs = [&](this auto &&dfs, int x) -> bool {
        // 正在访问
        colors[x] = 1;
        // 遍历x的邻居y
        for (auto &y : g[x]) {
            if (colors[y] == 1 || (colors[y] == 0 && dfs(y))) {
                return true;
            }
        }
        colors[x] = 2;
        return false;
    };
    for (int i = 0; i < numCourses; i++) {
        // dfs(i)会改变其他colors值,加上colors[i] == 0可以过滤一些情况
        if (colors[i] == 0 && dfs(i)) {
            // 存在环, 不能学完全部课程
            return false;
        }
    }
    return true;
}

// 208, 实现Trie
class Trie {
private:
    struct Node {
        // 不完整类型（incomplete type）的合法使用:
        // C++ 允许在类/结构体内部使用自身的指针或引用类型
        // 允许指针数组
        Node* son[26]{};
        bool end = false;
    };


    Node *root = new Node();
    int find(string word) {
        Node *cur = root;
        for (char c : word) {
            c -= 'a';
            if (cur->son[c] == nullptr) {
                return 0;
            }
            cur = cur->son[c];
        }
        // 2表示完全匹配,1表示前缀匹配
        return cur->end ? 2 : 1;
    }
    void destory(Node *node) {
        if (node == nullptr) {
            return;
        }
        for (Node * son : node->son) {
            destory(son);
        }
        delete node;
    }
public:
    Trie() {

    }
    ~Trie(){
        destory(root);
    }

    void insert(string word) {
        Node *cur = root;
        for (char c : word) {
            c -= 'a';
            if (cur->son[c] == nullptr) {
                cur->son[c] = new Node();
            }
            cur = cur->son[c];
        }
        cur->end = true;
    }

    bool search(string word) {
        return find(word) == 2;
    }

    bool startsWith(string prefix) {
        return find(prefix) != 0;
    }
};

/// 回溯
// 46, 全排列
// 78, 子集
// 17, 电话号码的字母组合
// 39, 组合总和
// 22, 括号生成

// 79, 单词搜索
bool exist(vector<vector<char>>& board, string word) {
    // board中字母出现次数
    unordered_map<char, int> cnt_b;
    for (auto &row : board) {
        for (char c : row) {
            cnt_b[c]++;
        }
    }

    // 优化1
    // board某个字母个数<word中某个字母个数, 一定没有路径满足word
    unordered_map<char, int> cnt_w;
    for (char c : word) {
        cnt_w[c]++;
        if (cnt_w[c] > cnt_b[c]) {
            return false;
        }
    }

    // 优化2
    // 如果word结尾的字母在board中出现次数更少,翻转过来更快
    if (cnt_b[word.back()] < cnt_b[word.front()]) {
        ranges::reverse(word);
    }

    // 上下左右
    static int DIR[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    int m = board.size();
    int n = board[0].size();
    // dfs(i, j, k)表示从board[i][j]出发,能不能满足接下来的路径能走到k = word.size()
    auto dfs = [&](this auto &&dfs, int i, int j, int k) -> bool {
        if (board[i][j] != word[k]) {
            return false;
        }
        if (k == word.size() - 1) {
            return true;
        }
        // 当前位置插旗
        board[i][j] = 0;
        // 4个方向前进
        for (auto &[dx, dy] : DIR) {
            int row = i + dx;
            int col = j + dy;
            if (row >= 0 && row < m && col >= 0 && col < n && dfs(row, col, k + 1)) {
                return true;
            }
        }
        // 恢复现场
        board[i][j] = word[k];
        return false;
    };
    // 每个位置都有可能
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (dfs(i, j, 0)) {
                return true;
            }
        }
    }
    return false;
}

// 131, 分割回文串
// 51, N皇后

/// 二分查找
// 35, 搜索插入位置
int searchInsert(vector<int>& nums, int target) {
    // 二分查找
    int n = nums.size();
    int left = -1;
    int right = n;
    while (left + 1 < right) {
        int mid = left + (right - left) / 2;
        (nums[mid] >= target ? right : left) = mid;
    }
    return right;
}

// 74, 搜索二维矩阵
// 34, 在排序数组查找元素的第一个和最后一个位置
// 33, 搜索旋转排序数组
// 153, 寻找旋转排序数组中的最小值

// 4, 寻找两个正序数组的中位数
double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
    if (nums1.size() > nums2.size()) {
        swap(nums1, nums2);
    }
    int m = nums1.size();
    int n = nums2.size();
    // 有插入,时间复杂度是O(m+n)
    nums1.insert(nums1.begin(), INT_MIN);
    nums2.insert(nums2.begin(), INT_MIN);
    nums1.push_back(INT_MAX);
    nums2.push_back(INT_MAX);

    // j = (m + n + 1) / 2 - i;
    int i = 0;
    int j = (m + n + 1) / 2;
    // 可以判断a[i+1]和b[j]大小来跳出循环
    // 跳出循环的上一个就可以满足a[i] <= b[j+1] 且 a[i+1] > b[j]
    while (nums1[i + 1] <= nums2[j + 1]) {
        i++;
        j--;
    }
    int mx = max(nums1[i], nums2[j]);
    int mn = min(nums1[i + 1], nums2[j + 1]);
    // 奇数取第一组最大的, 偶数平分第一组最大和第二组最小的
    return (m + n) % 2 ? mx : (mx + mn) / 2.0;
}
double findMedianSortedArrays_2(vector<int>& nums1, vector<int>& nums2) {
    // 二分确定i位置, 不能插入INT_MIN和INT_MAX辅助
    // 时间复杂度达到O(log(min(m, n)))
    if (nums1.size() > nums2.size()) {
        swap(nums1, nums2);
    }
    // 原来 j = (m + n + 1) / 2 - i;
    // 现在 (j + 1) = (m + n + 1) / 2 - (i + 1);
    int m = nums1.size();
    int n = nums2.size();

    // 循环不变量,
    // nums1[left] <= nums2[j + 1]
    // nums1[right] > nums2[j + 1]
    int left = -1;
    int right = m;
    while (left + 1 < right) {
        int i = left + (right - left) / 2;
        int j = (m + n + 1) / 2 - i - 2;
        (nums1[i] > nums2[j + 1] ? right : left) = i;
    }
    int i = left;
    int j = (m + n + 1) / 2 - i - 2;
    int ai = i >= 0 ? nums1[i] : INT_MIN;
    int bj = j >= 0 ? nums2[j] : INT_MIN;
    int ai1 = i + 1 < m ? nums1[i + 1] : INT_MAX;
    int bj1 = j + 1 < n ? nums2[j + 1] : INT_MAX;
    int mx = max(ai, bj);
    int mn = min(ai1, bj1);
    return (m + n) % 2 ? mx : (mx + mn) / 2.0;
}

/// 栈
// 20, 有效的括号
bool isValid(string s) {
    if (s.size() % 2 != 0) {
        return false;
    }
    unordered_map<char, char> match = {{')', '('},
                                       {'}', '{'},
                                       {']', '['}};
    stack<char> st;
    for (char c : s) {
        // 左括号, 入栈
        if (!match.contains(c)) {
            st.push(c);
        } else {
            // 不匹配, 返回false
            if (st.empty() || st.top() != match[c]) {
                return false;
            }
            st.pop();
        }
    }
    return st.empty();
}

// 155, 最小栈
class MinStack {
    // 不是手写栈, 可以用标库
    // value, 所有val的最小值
    stack<pair<int, int>> st;
public:
    MinStack() {
        st.emplace(0, INT_MAX);
    }

    void push(int val) {
        st.push({val, min(getMin(), val)});
    }

    void pop() {
        st.pop();
    }

    int top() {
        return st.top().first;
    }

    int getMin() {
        return st.top().second;
    }
};

// 394, 字符串解码
string decodeString(string s) {
    int n = s.size();
    int i = 0;
    auto decode = [&](this auto &&decode) ->string {
        // 从i位置开始的res
        string res;
        int k = 0;
        while (i < n) {
            char c = s[i];
            i++;
            if (isalpha(c)) {
                res += c;
            } else if (isdigit(c)) {
                k = k * 10 + c - '0';
            } else if (c == '[') {
                // 递归得到[]内的字符
                string t = decode();
                for (; k > 0; k--) {
                    res += t;
                }
            } else {
                // ']'
                break;
            }
        }
        return res;
    };
    return decode();
}
string decodeString_2(string s) {
    // 用栈模拟递归
    // 字符串,对应出现次数
    stack<pair<string, int>> st;
    string res;
    int k = 0;
    for (char c : s) {
        if (isalpha(c)) {
            res += c;
        } else if (isdigit(c)) {
            k = k * 10 + c - '0';
        } else if (c == '[') {
            // move后, res为空了
            st.emplace(std::move(res), k);
            k = 0;
        } else {
            // 此时res是[]内的字符串
            auto [pre_res, pre_k] = st.top();
            st.pop();
            for (; pre_k > 0; pre_k--) {
                pre_res += res;
            }
            res = std::move(pre_res);
            // cout << res << ", " << pre_res<< ", " << pre_k << endl;
            // "3[a]2[bc]"  "3[a2[c]]"
            // a, , 3       c, a, 2
            // bc, aaa, 2   acc, , 3
        }
    }
    return res;
}

// 739, 每日温度
// 84, 柱状图中最大的矩形

/// 堆
// 215, 数组中第K个最大元素
int findKthLargest(vector<int>& nums, int k) {
    // 快速选择算法
    srand(time(NULL));
    // 分区,将[left, right]按照随机选择的pivot,
    // nums[i]<=pivot, pivot>=nums[i]
    // 返回pivot的下标
    auto partition = [&](int left, int right) -> int {
        int i = left + rand() % (right - left + 1);
        int pivot = nums[i];

        // 将pivot与第一个元素交换
        // 剩下的排完序,把pivot与j位置的元素交换
        swap(nums[left], nums[i]);

        // 双指针
        i = left + 1;// pivot不算
        int j = right;
        while (true) {
            // 这里<, 不能带=
            while (i <= j && nums[i] < pivot) {
                i++;
            }
            // 此时 nums[i] >= pivot

            while (i <= j && nums[j] > pivot) {
                j--;
            }
            // 此时 nums[j] <= pivot

            if (i >= j) {
                break;
            }
            // 不满足条件, 交换
            swap(nums[i++], nums[j--]);
        }

        // pivot放到对应位置,
        swap(nums[left], nums[j]);
        return j;
    };

    int n = nums.size();
    int left = 0;
    int right = n - 1;
    while (true) {
        int index = partition(left, right);
        if (index == n - k) {
            return nums[index];
        }
        if (index > n - k) {
            // index对应的值大了,右边界可以缩小
            right = index - 1;
        } else {
            left = index + 1;
        }
    }
    return 0;
}

// 347, 前K个高频元素
vector<int> topKFrequent(vector<int>& nums, int k) {
    // 统计次数
    unordered_map<int, int> cnts;
    int max_cnt = 0;
    for (auto x : nums) {
        cnts[x]++;
        max_cnt = max(max_cnt, cnts[x]);
    }

    // 出现次数相同的元素,放到一个桶里
    // 最大有[0, max_cnt] max_cnt + 1种桶
    vector<vector<int>> buckets(max_cnt + 1);
    for (auto it : cnts) {
        buckets[it.second].push_back(it.first);
    }

    // 前k个是答案个数是k个
    vector<int> ans;
    for (int i = max_cnt; i >= 0 && ans.size() < k; i--) {
        ans.insert(ans.end(), buckets.at(i).begin(), buckets.at(i).end());
    }
    return ans;
}

// 295, 数据流的中位数
class MedianFinder {
    // 最大堆, 堆顶是最大值
    priority_queue<int> left;
    // 最小堆, 堆顶是最小值
    priority_queue<int, vector<int>, greater<>> right;
public:
    MedianFinder() {

    }

    void addNum(int num) {
        // 中位数, 将数值集合划分为相等的两个部分
        // 奇数, 把多的一个放到第一个
        if (left.size() == right.size()) {
            // 相等,就把num先放入right
            // 再把right最小值放入left
            right.push(num);
            left.push(right.top());
            right.pop();
        } else {
            // left多一个, 先把num放入left
            // 再把left最大值放入right中
            left.push(num);
            right.push(left.top());
            left.pop();
        }
    }

    double findMedian() {
        return left.size() == right.size() ? (left.top() + right.top()) / 2.0 : left.top();
    }
};
/// 贪心算法
// 121, 买卖股票的最佳时机
int maxProfit(vector<int>& prices) {
    // 不能当天买,当天卖
    // 枚举卖出价格, 维护买入最小值
    int min_price = prices[0];
    int ans = INT_MIN;
    for (int p : prices) {
        ans = max(ans, p - min_price);
        min_price = min(min_price, p);
    }
    return ans;
}

// 55, 跳跃游戏
bool canJump(vector<int>& nums) {
    // 维护最右可到达位置, 保证下一个i在区间内
    int mx = 0;
    for (int i = 0; i < nums.size(); i++) {
        // 跳不到下一个坐标
        if (i > mx) {
            return false;
        }
        // nums[i]是非负数
        mx = max(mx, i + nums[i]);
    }
    return true;
}

// 45, 跳跃游戏2
int jump(vector<int>& nums) {
    int ans = 0;
    int cur_right = 0;
    int next_right = 0;
    // [0, n - 1], i < cur_right
    for (int i = 0; i < nums.size() - 1; i++) {
        next_right = max(next_right, i + nums[i]);
        if (i == cur_right) {
            // 无路可走,造桥
            cur_right = next_right;
            ans++;
        }
    }
    return ans;
}

// 763, 划分字母区间
vector<int> partitionLabels(string s) {
    int n = s.size();
    int last[26];
    // 记录每个字母的最右下标
    for (int i = 0; i < n; i++) {
        last[s[i] - 'a'] = i;
    }

    vector<int> ans;
    int start = 0, end = 0;
    for (int i = 0; i < n; i++) {
        end = max(end, last[s[i] - 'a']);
        if (i == end) {
            ans.push_back(end - start + 1);
            start = end + 1;
        }
    }
    return ans;
}
/// 动态规划
// 70, 爬楼梯

// 118, 杨辉三角
vector<vector<int>> generate(int numRows) {
    // [1]
    // [1, 1]
    // [1, 2, 1]
    // [1, 3, 3, 1]
    // [1, 4, 6, 4, 1]
    vector<vector<int>> c(numRows);
    for (int i = 0; i < numRows; i++) {
        c[i].resize(i + 1, 1);
        for (int j = 1; j < i; j++) {
            c[i][j] = c[i - 1][j - 1] + c[i - 1][j];
        }
    }
    return c;
}

// 198, 打家劫舍
// 279, 完全平方数
// 322, 零钱兑换

// 139, 单词拆分
bool wordBreak(string s, vector<string>& wordDict) {
    // 划分型DP
    // 限制最大长度(string种类有最多20种, wordDict字典个数最多1000个)
    // 知道最大长度就好, [j, i)尾巴字符串最长只能是max_len
    // j = i - 1, i - 2, i - 3, max(i - max_len, 0), 列举到这里就行
    int max_len = ranges::max(wordDict, {}, &string::length).length();
    unordered_set<string> st(wordDict.begin(), wordDict.end());
    int n = s.length();
    vector<int> memo(n + 1, -1);
    auto dfs = [&](this auto &&dfs, int i) -> bool {
        if (i == 0) {
            // 还剩0个字符串需要划分, 拆分成功
            return true;
        }
        int &res = memo[i];
        if (res != -1) {
            return res;
        }
        // 所有情况中存在某个字符串在字典中,当前拆分成功,继续递归
        for (int j = i - 1; j >= max(0, j - max_len); j--) {
            if (st.contains(s.substr(j, i - j)) && dfs(j)) {
                return res = true;
            }
        }
        return res = false;
    };
    return dfs(n);
}

// 300, 最长递增子序列

// 152, 乘积最大子数组
int maxProduct(vector<int>& nums) {
    // 连续性子集问题
    int n = nums.size();
    // 以nums[i]为结尾的最大子数组乘积,最小子数组乘积
    // 三种情况,
    // nums[i]单数组,
    // 与前面的拼起来(nums[i]>0, 与前面最大的拼起来
    // <0, 与前面最小的拼起来)
    vector<int> f_max(n), f_min(n);
    f_max[0] = f_min[0] = nums[0];
    for (int i = 1; i < n; i++) {
        int x = nums[i];
        f_max[i] = max({f_min[i - 1] * x, f_max[i - 1] * x ,x});
        f_min[i] = min({f_min[i - 1] * x, f_max[i - 1] * x, x});
    }
    return ranges::max(f_max);
}

// 416, 分割等和子集

// 32, 最长有效括号
int longestValidParentheses(string s) {
    stack<int> st;
    st.push(-1);// 作为红线
    int n = s.size();
    int ans = 0;
    for (int i = 0; i < n; i++) {
        // '('作为炸弹入栈
        if (s[i] == '(') {
            st.push(i);
        } else if (st.size() > 1) {
            // ')', 可以正常拆弹
            st.pop();
            // (st.top(), i]是正常的括号子串长度
            ans = max(ans, i - st.top());
        } else {
            // ')', 只有红线,拆不了,更新红线
            st.top() = i;
        }
    }
    return ans;
}

/// 多维动态规划
// 62, 不同路径
int uniquePaths(int m, int n) {
    vector<vector<int>> memo(m, vector<int>(n, -1));
    // dfs(i, j)表示从(i, j)->(0, 0)的路径数(反过来看)
    auto dfs = [&](this auto &&dfs, int i, int j) -> int {
        if (i < 0 || j < 0) {
            return 0;
        }
        if (i == 0 && j == 0) {
            return 1;
        }
        int &res = memo[i][j];
        if (res != -1) {
            return res;
        }
        return res = dfs(i - 1, j) + dfs(i, j - 1);
    };
    return dfs(m - 1, n - 1);
}
// 64, 最小路径和

// 5, 最长回文子串
string longestPalindrome(string s) {
    int n = s.size();
    // 开区间更通用一点,不会忽略一个字符的情况
    // [ans_left, ans_right)
    int ans_left = 0, ans_right = 0;
    // 奇回文串
    // 枚举i = 0, 1, 2,..., n - 1
    for (int i = 0; i < n; i++) {
        int l = i;
        int r = i;
        // 中心扩展,左边向左走,右边向右走
        while (l >= 0  && r < n && s[l] == s[r]) {
            l--;
            r++;
        }
        // 循环结束后[l+1, r-1]是回文串
        if (ans_right - ans_left < r - l - 1) {
            ans_left = l + 1;
            ans_right = r;
        }
    }

    // 偶回文串
    // 枚举i = 0, 1, 2, ..., n - 2,
    // j = i + 1
    for (int i = 0; i < n - 1; i++) {
        int l = i;
        int r = i + 1;
        while (l >= 0  && r < n && s[l] == s[r]) {
            l--;
            r++;
        }
        // 循环结束, [l+1, r-1]是回文串
        if (ans_right - ans_left < r - l - 1) {
            ans_left = l + 1;
            ans_right = r;
        }
    }
    return s.substr(ans_left, ans_right - ans_left);
}

// 1143, 最长公共子序列
// 72, 编辑距离

/// 技巧
// 136, 只出现一次的数字

// 169, 多数元素

// 75, 颜色分类

// 31, 下一个排列

// 287, 寻找重复数
}

#endif // _3HOT1___2_H
