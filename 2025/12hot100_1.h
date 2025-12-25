#ifndef _2HOT1___H
#define _2HOT1___H

#include "00solution.h"

#include <unordered_set>

namespace hot100_1 {
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
    // set会去掉重复元素
    unordered_set<int> st(nums.begin(), nums.end());
    int ans = 0;
    // 这里要遍历哈希集合
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
            swap(x, nums[i0++]);
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
    // 定长滑动窗口[left, right]
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
        // 定长窗口, 满了之后要一直右移
        cnt_s[s[left] - 'a']--;// 左端点离开窗口
    }
    return ans;
}

/// 子串
// 303, 区域和检索数组-不可变
class NumArray {
    vector<int> sum;
public:
    NumArray(vector<int>& nums) {
        int n = nums.size();
        sum.resize(n + 1);
        sum[0] = 0;
        for (int i = 0; i < n; i++) {
            sum[i + 1] = sum[i] + nums[i];
        }
    }

    int sumRange(int left, int right) {
        // s[j] - s[i] = [i, j - 1], 一共j - i个
        // [0, right] - [0, left - 1]
        // [left, right]
        return sum[right + 1] - sum[left];
    }
};
// 560, 和为k的子数组
int subarraySum(vector<int>& nums, int k) {
    // 如果不要求连续就是  01背包求方案数
    // 这题是前缀和+两数之和
    // 枚举右端点,统计左端点
    int n = nums.size();
    vector<int> sum(n + 1);
    sum[0] = 0;
    for (int i = 0; i < n; i++) {
        sum[i + 1] = sum[i] + nums[i];
    }

    // 枚举j,选择i
    // s[j] - s[i] = k
    // s[i] = s[j] - k
    int ans = 0;
    // s[i], 对应出现次数
    unordered_map<int, int> cnt;
    for (auto sj : sum) {
        int val = sj - k;
        ans += cnt.contains(val) ? cnt[val] : 0;
        cnt[sj]++;
    }
    return ans;
}

// 239, 滑动窗口最大值 Monotonic_Queue
// 76, 最小覆盖子串 SlidingWindow

/// 普通数组
// 53, 最大子数组和
int maxSubArray(vector<int>& nums) {
    int pre_sum = 0;
    int min_pre_sum = 0;
    int ans = INT_MIN;
    for (auto x : nums) {
        // 至少有一个元素, 先计算前缀和-最小前缀和
        pre_sum += x;
        ans = max(ans, pre_sum - min_pre_sum);
        // 再更新最小前缀和
        min_pre_sum = min(min_pre_sum, pre_sum);
    }
    return ans;
}
int maxSubArray_2(vector<int>& nums) {
    // 仿照152,
    int n = nums.size();
    vector<int> f(n);
    f[0] = nums[0];
    for (int i = 1; i < n; i++) {
        // 最长子数组和,
        // nums[i]单数组
        // nums[i]与前面拼起来
        int x = nums[i];
        f[i] = max(f[i - 1] + x, x);
    }
    return ranges::max(f);
}

// 56, 合并区间
vector<vector<int>> merge(vector<vector<int>>& intervals) {
    ranges::sort(intervals);
    // 不需要额外变量存储左右端点,用ans.back()就知道了
    vector<vector<int>> ans;
    for (auto &pair : intervals) {
        int x = pair[0];
        int y = pair[1];
        // 更新右端点
        if (!ans.empty() && ans.back()[1] >= x) {
            ans.back()[1] = max(ans.back()[1], y);
        } else {
            // 不重叠,添加新区间
            ans.push_back(pair);
        }
    }
    return ans;
}

// 189, 轮转数组
void rotate(vector<int>& nums, int k) {
    // [i, j]闭区间
    auto reverse = [&](int i, int j) {
        while (i < j) {
            swap(nums[i++], nums[j--]);
        }
    };
    int n = nums.size();
    k = k % n;
    reverse(0, n - 1);
    reverse(0, k - 1);
    reverse(k, n - 1);
}

// 238, 除自身以外的数组的乘积
vector<int> productExceptSelf(vector<int>& nums) {
    int n = nums.size();
    vector<int> pre_prod(n, 1);
    vector<int> suf_prod(n, 1);
    for (int i = 1; i < n; i++) {
        pre_prod[i] = pre_prod[i - 1] * nums[i - 1];
    }
    for (int i = n - 2; i >= 0; i--) {
        suf_prod[i] = suf_prod[i + 1] * nums[i + 1];
    }
    vector<int> ans;
    for (int i = 0; i < n; i++) {
        ans.push_back(pre_prod[i] * suf_prod[i]);
    }
    return ans;
}
vector<int> productExceptSelf_2(vector<int>& nums) {
    // 把suf当作输出, 空间复杂度O(n)->O(1)
    int n =  nums.size();
    vector<int> suf(n, 1);
    for (int i = n - 2; i >= 0; i--) {
        suf[i] = suf[i + 1] * nums[i + 1];
    }

    int pre = 1;
    for (int i = 0; i < n; i++) {
        suf[i] *= pre;
        pre *= nums[i];
    }
    return suf;
}

// 41, 缺失的第一个正数
int firstMissingPositive(vector<int>& nums) {
    // 要求O(n)时间复杂度, O(1)空间复杂度
    int n = nums.size();
    // 学号从1开始, 所以这样是坐到对应位置了, nums[i](位置) = i + 1(学号)
    for (int i = 0; i < n; i++) {
        // 正数 + 当前位置i不是学号的真正的位置
        // 忽略非正数, 忽略重复数字, 忽略超过size的数字
        while (nums[i] > 0 && nums[i] < n && nums[i] != nums[nums[i] - 1]) {
            // 交换当前位置i,与学号真正对应的位置
            swap(nums[i] , nums[nums[i] - 1]);
        }
    }
    for (int i = 0; i < n; i++) {
        if (nums[i] != i + 1) {
            return i + 1;
        }
    }
    return n + 1;
}

/// 矩阵
// 73, 矩阵置零
void setZeroes(vector<vector<int>>& matrix) {
    int m = matrix.size();
    int n = matrix[0].size();
    // 额外记录第一行是否需要置零
    bool first_row_has_zero = ranges::contains(matrix[0], 0);
    // 第一行,第一列作为标志记录当前行,当前列是否需要置零
    for (int i = 1; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == 0) {
                matrix[0][j] = matrix[i][0] = 0;
            }
        }
    }
    // 遍历置零
    for (int i = 1; i < m; i++) {
        // 列倒着遍历, 避免matrix[i][0]提前置零造成误会
        for (int j = n - 1; j >= 0; j--) {
            if (matrix[i][0] == 0 || matrix[0][j] == 0) {
                matrix[i][j] = 0;
            }
        }
    }

    // 处理第一行
    if (first_row_has_zero) {
        ranges::fill(matrix[0], 0);
    }
}

// 54, 螺旋矩阵
vector<int> spiralOrder(vector<vector<int>>& matrix) {
    // 右(row, col++),下(row++, col),左(row, col--),上(row--, col)
    static constexpr int DIR[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    int m = matrix.size();
    int n = matrix[0].size();
    vector<int> ans(m * n);
    // 行, 列, 方向
    int i = 0, j = 0, dir = 0;
    for (int k = 0; k < m * n; k++) {
        ans[k] = matrix[i][j];
        // 标记成取不到的值
        matrix[i][j] = INT_MAX;
        int x = i + DIR[dir][0];
        int y = j + DIR[dir][1];
        // 判断是否要转向, 超过范围, 已经取过
        if (x < 0 || x >= m || y < 0 || y >= n || matrix[x][y] == INT_MAX) {
            dir = (dir + 1) % 4;
        }
        i += DIR[dir][0];
        j += DIR[dir][1];
    }
    return ans;
}

// 48, 旋转图像
void rotate(vector<vector<int>>& matrix) {
    int n = matrix.size();
    // target: (i, j) -> (j, n - i - 1)
    auto reverse = [&](vector<int> &row) {
        int i = 0;
        int j = n - 1;
        while (i < j) {
            swap(row[i++], row[j--]);
        }
    };

    // 翻转对角线上方的
    // 线性代数, 转置(i, j)->(j, i)
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            swap(matrix[i][j], matrix[j][i]);
        }
    }
    // 行翻转(j, i)->(j, n - i - 1)
    for (auto &row : matrix) {
        reverse(row);
    }
}
// 顺时针180
void rotate_2(vector<vector<int>>& matrix) {
    // (i, j) -> (n - i - 1, n - j - 1)
    int n = matrix.size();
    // 行沿中心线翻转(i, j)->(n - i - 1, j)
    for (int i = 0; i < n / 2; i++) {
        for (int j = 0; j < n; j++) {
            swap(matrix[i][j], matrix[n - i - 1][j]);
        }
    }
    for (auto &row : matrix) {
        // 列翻转(n - i - 1, j) -> (n - i - 1, n - j - 1)
        ranges::reverse(row);
    }
}
// 逆时针90
void rotate_3(vector<vector<int>>& matrix) {
    // (i, j) -> (n - j - 1, i)
    int n = matrix.size();
    // 转置(i, j) -> (n - j - 1, n - i - 1)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            swap(matrix[i][j], matrix[n - j - 1][n - i - 1]);
        }
    }
    // 列翻转 (n - j - 1, n - i - 1) ->  (n - i - 1, i)
    for (auto &row : matrix) {
        ranges::reverse(row);
    }
}

// 240, 搜索二维矩阵2
bool searchMatrix(vector<vector<int>>& matrix, int target) {
    int m = matrix.size();
    int n = matrix[0].size();
    // 右上角开始
    int i = 0, j = n - 1;
    while (i < m && j >= 0) {
        if (matrix[i][j] == target) {
            return true;
        }
        // 这一列都大于target, 可以排除
        if (matrix[i][j] > target) {
            j--;
        } else {
            // 这一行都小于target, 可以排除
            i++;
        }
    }
    return false;
}

/// 链表
// 160, 相交链表
ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
    // x + y + z路程一样
    ListNode *p = headA;
    ListNode *q = headB;
    // 把空节点也当作路程中一个节点,
    // 如果链表不相交,循环会在q = p = nullptr跳出
    while (p != q) {
        p = p ? p->next : headB;
        q = q ? q->next : headA;
    }
    return p;
}

// 206, 反转链表 LinkedList_Reverse
// 234, 回文链表 LinkedList_QuickNSlow
// 141, 环形链表 LinkedList_QuickNSlow
// 142, 环形链表2 LinkedList_QuickNSlow

// 88, 合并两个有序数组
void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
    int p = m + n - 1;
    int p1 = m - 1;
    int p2 = n - 1;
    while (p2 >= 0) {
        if (p1 >= 0 && nums1[p1] > nums2[p2]) {
            nums1[p--] = nums1[p1--];
        }  else {
            nums1[p--] = nums2[p2--];
        }
    }

}
// 21, 合并两个有序链表
ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
    ListNode dummy;
    ListNode *cur = &dummy;
    while (list1 && list2) {
        if (list1->val <= list2->val) {
            cur->next = list1;
            list1 = list1->next;
        } else {
            cur->next = list2;
            list2 = list2->next;
        }
        cur = cur->next;
    }
    cur->next = list1 ? list1 : list2;
    return dummy.next;
}

// 2, 两数相加 LinkedList_Reverse
// 19, 删除链表的倒数第N个结点 LinkedList_Delete
// 24, 两两交换链表中的节点 LinkedList_Reverse
// 25, K个一组翻转链表 LinkedList_Reverse

// 328, 奇偶链表
ListNode* oddEvenList(ListNode* head) {
    if (head == nullptr) {
        return nullptr;
    }
    ListNode *odd = head;
    ListNode *even = head->next;
    for (; odd && odd->next && odd->next->next; odd = odd->next) {
        ListNode *copy = odd->next;
        odd->next = odd->next->next;
        copy->next = copy->next->next;
    }
    // odd此时所处位置就是第一条链的end,next应该置空
    odd->next = nullptr;
    // 再接上even链
    odd->next = even;
    return even;
}
// 138, 随机链表的复制
Node* copyRandomList(Node* head) {
    if (head == nullptr) {
        return nullptr;
    }
    // 每个节点复制一个节点
    // 遍历原节点
    for (Node *cur = head; cur; cur = cur->next->next) {
        Node *next = cur->next;
        cur->next = new Node(cur->val, cur->next, nullptr);
        cur->next = next;
    }

    // 填充random节点
    for (Node *cur = head; cur; cur = cur->next->next) {
        Node *copy = cur->next;
        if (cur->random) {
            Node *random_copy = cur->random->next;
            copy->random = random_copy;
        }
    }

    // 拆开两条链
    Node *odd = head;
    Node *even = head->next;
    for (; odd && odd->next && odd->next->next; odd = odd->next) {
        Node *copy = odd->next;
        odd->next = odd->next->next;
        copy->next = copy->next->next;
    }
    // odd是奇数链结尾,断开多余的链接
    odd->next = nullptr;
    return even;
}

// 148, 排序链表
ListNode* sortList(ListNode* head) {
    // 找到中间节点, 同时把前一个链表的最后一个节点端口
    // 不存在重复节点了
    auto find_mid = [&](ListNode *head) {
        ListNode *slow = head;
        ListNode *fast = head;
        ListNode *pre = head;
        while (fast && fast->next) {
            pre = slow;
            slow = slow->next;
            fast = fast->next->next;
        }
        pre->next = nullptr;
        return slow;
    };

    // merge, 把两个链表合并成一个有序链表
    auto merge_lists = [&](ListNode *list1, ListNode *list2) {
        ListNode dummy(0);
        ListNode *cur = &dummy;
        while (list1 && list2) {
            if (list1->val <= list2->val) {
                cur->next = list1;
                list1 = list1->next;
            } else {
                cur->next = list2;
                list2 = list2->next;
            }
            cur = cur->next;
        }
        cur->next = list1 ? list1 : list2;
        return dummy.next;
    };
    auto sorted = [&](this auto &&sorted, ListNode *head) {
        if (head == nullptr || head->next == nullptr) {
            return head;
        }
        auto mid = find_mid(head);
        auto head1 = sorted(head);
        auto head2 = sorted(mid);
        return merge_lists(head1, head2);
    };
    return sorted(head);
}

// 23, 合并K个升序链表
ListNode* mergeKLists(vector<ListNode*>& lists) {
    auto merge_two_lists = [&](ListNode *list1, ListNode *list2) {
        ListNode dummy(0);
        ListNode *cur = &dummy;
        while (list1 && list2) {
            if (list1->val <= list2->val) {
                cur->next = list1;
                list1 = list1->next;
            } else {
                cur->next = list2;
                list2 = list2->next;
            }
            cur = cur->next;
        }
        cur->next = list1 ? list1 : list2;
        return dummy.next;
    };
    // [i, j)
    auto merge_k_lists = [&](this auto &&merge_k_lists, int i, int j) -> ListNode* {
        int m = j - i;
        if (m == 0) { // 空区间
            return nullptr;
        }
        if (m == 1) { // 只有一个链表
            return lists[i];
        }
        int mid = i + (j - i) / 2;
        auto left = merge_k_lists(i, mid);
        auto right = merge_k_lists(mid, j);
        return merge_two_lists(left, right);
    };
    return merge_k_lists(0, lists.size());
}

// 146, LRU缓存
// 标准库实现
class LRUCache {
    int capacity;
    // key, value
    list<pair<int, int>> cache_list;
    // key, iterator
    unordered_map<int, list<pair<int, int>>::iterator> key_to_iter;
public:
    // 不是按次数算最少使用, 是最久没被用
    // get和put都会把当前key作为最新的
    LRUCache(int capacity) {
        this->capacity = capacity;
    }

    int get(int key) {
        // 没有这本书, 返回-1
        auto umap_iter = key_to_iter.find(key);
        if (umap_iter == key_to_iter.end()) {
            return -1;
        }
        // 有这本书,把迭代器放最上面
        auto list_iter = umap_iter->second;
        cache_list.splice(cache_list.begin(), cache_list, list_iter);
        return list_iter->second;
    }

    void put(int key, int value) {
        // 有这本书,更新一下;放到最上面
        auto umap_iter = key_to_iter.find(key);
        if (umap_iter != key_to_iter.end()) {
            auto list_iter = umap_iter->second;
            // 更新值
            list_iter->second = value;
            // 把迭代器放到最前面
            // cache_list中list_iter迭代器,放到begin()之前
            // void splice( const_iterator pos, list& other, const_iterator it);
            cache_list.splice(cache_list.begin(), cache_list, list_iter);
            return;
        }
        // 没有这本书,插入到最前面
        cache_list.push_front({key, value});
        key_to_iter.insert({key, cache_list.begin()});
        // 超过capacity,就把最旧的pop出去
        if (cache_list.size() > capacity) {
            int old_key = cache_list.back().first;
            cache_list.pop_back();
            key_to_iter.erase(old_key);
        }
    }
};
// 手写链表
class LRUCache_2 {
    struct Node {
        int key;
        int value;
        Node *prev;
        Node *next;

        Node(int k, int v) : key(k), value(v) {}
    };

    int capacity;
    Node *dummy; //哨兵节点
    unordered_map<int, Node*> key_to_node;

    // 移除最后一个节点
    void remove(Node *node) {
        Node *pre = node->prev;
        Node *nxt = node->next;
        pre->next = nxt;
        nxt->prev = pre;
    }
    // 添加Node到首位置
    void push_front(Node *node) {
        // 移动到dummy的下一个
        Node *nxt = dummy->next;
        dummy->next = node;
        node->prev = dummy;
        node->next = nxt;
        nxt->prev = node;
    }
    // 如果不存在对应Node,返回空
    // 如果存在就把当前node放到最前面
    Node *get_node(int key) {
        auto umap_iter = key_to_node.find(key);
        if (umap_iter == key_to_node.end()) {
            return nullptr;
        }
        Node *node = umap_iter->second;
        remove(node);
        push_front(node);
        return node;
    }

public:
    LRUCache_2(int capacity) {
        this->capacity = capacity;
        dummy = new Node(0,0);
        dummy->next = dummy;
        dummy->prev = dummy;
    }

    int get(int key) {

    }

    void put(int key, int value) {

    }
};


/// 二叉树
// 94, 二叉树的中序遍历
vector<int> inorderTraversal(TreeNode* root) {
    // 左根右
    vector<int> vals;
    auto dfs = [&](this auto &&dfs, TreeNode *root) {
        if (root == nullptr) {
            return;
        }
        dfs(root->left);
        vals.emplace_back(root->val);
        dfs(root->right);
    };
    dfs(root);
    return vals;
}

// 104, 二叉树的最大深度
// 226, 翻转二叉树
// 101, 对称二叉树
// 543, 二叉树的直径
// 102, 二叉树的层序遍历

// 108, 将有序数组转换为二叉搜索树
TreeNode* sortedArrayToBST(vector<int>& nums) {
    // [left, right)
    auto dfs = [&](this auto &&dfs, int left, int right) -> TreeNode* {
        if (left == right) {
            return nullptr;
        }
        int mid = left + (right - left) / 2;
        // 左:[left, mid), 中:mid, 右:[mid+1, right)
        return new TreeNode(nums[mid], dfs(left, mid), dfs(mid + 1, right));
    };
    return dfs(0, nums.size());
}

// 98, 验证二叉搜索树
// 230, 二叉搜索树中第K小的元素
// 199, 二叉树的右视图

// 114, 二叉树展开为链表
void flatten(TreeNode* root) {
    // 尾巴节点, 右左根
    auto dfs = [&](this auto &&dfs, TreeNode *root) -> TreeNode* {
        if (root == nullptr) {
            return nullptr;
        }
        auto left_tail = dfs(root->left);
        auto right_tail = dfs(root->right);
        if (left_tail) {
            // 左子树链表的尾巴节点的右子树 -> 右子树
            left_tail->right = root->right;
            // 右子树 = 左子树
            root->right = root->left;
            // 左子树 = 空
            root->left = nullptr;
        }
        // 如果有右尾巴,返回右尾巴
        // 没有就返回左尾巴,如果左尾巴也没有就返回root
        return right_tail ? right_tail : left_tail ? left_tail : root;
    };
    dfs(root);
}

// 105, 从前序与中序遍历序列构造二叉树

// 437, 路径总和3
int pathSum(TreeNode* root, int targetSum) {
    int ans = 0;
    // 类似560, 连续子数组,适合用前缀和
    // 枚举路径终点, 统计路口起点
    // <值, 出现次数>
    // 初始化一个出现1一次的0,类似dummy node
    unordered_map<long long, int> cnts{{0, 1}};
    // s表示根节点到node节点的所有和, 不包含node
    auto dfs = [&](this auto &&dfs, TreeNode *node, long long s) {
        if (node == nullptr) {
            return;
        }
        // s是根节点到当前节点完整路径
        // targetSum是固定当前节点,有一些起点可能满足这个路径
        s += node->val;
        long long x = s - targetSum;
        ans += cnts.contains(x) ? cnts[x] : 0;

        cnts[s]++;
        dfs(node->left, s);
        dfs(node->right, s);
        cnts[s]--;
    };
    dfs(root, 0);
    return ans;
}

// 236, 二叉树的最近公共祖先
// 124, 二叉树的最大路径和

};
#endif // _2HOT1___H
