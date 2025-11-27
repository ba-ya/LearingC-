#ifndef _5BINARYTREE_H
#define _5BINARYTREE_H

#include "00solution.h"

#include <queue>
#include <unordered_set>

namespace BinaryTree_Understand {
// 104二叉树的最大深度
int maxDepth(TreeNode* root) {
    if (root == nullptr) {
        return 0;
    }
    int depth_l = maxDepth(root->left);
    int depth_r = maxDepth(root->right);
    return max(depth_l, depth_r) + 1;
}

// 111二叉树的最小深度
int minDepth(TreeNode* root) {
    // 与两个子节点进行比较
    int ans = INT_MAX;
    std::function<void(TreeNode*, int)> dfs;
    dfs = [&](TreeNode* root, int cnt) {
        if (root == nullptr) {
            return;
        }
        cnt++;
        if (root->left == nullptr && root->right == nullptr) {
            ans = min(ans, cnt);
            return;
        }
        dfs(root->left, cnt);
        dfs(root->right, cnt);
    };
    dfs(root, 0);
    return root ? ans : 0;
}

// 404左叶子之和
int sumOfLeftLeaves(TreeNode* root) {
    int ans = 0;
    std::function<void(TreeNode *)> dfs;
    dfs = [&](TreeNode *node) {
        if (node == nullptr) {
            return;
        }
        // 左叶子节点
        if (node->left != nullptr
            && node->left->left == nullptr
            && node->left->right == nullptr) {
            ans += node->left->val;
        }
        dfs(node->left);
        dfs(node->right);
    };
    dfs(root);
    return ans;
}

// 112路径总和
bool hasPathSum(TreeNode* root, int targetSum) {
    if (root == nullptr) {
        return false;
    }
    targetSum -= root->val;
    if (root->left == nullptr && root->right == nullptr) {
        return targetSum == 0;
    }
    return hasPathSum(root->left, targetSum) || hasPathSum(root->right, targetSum);
}

// 129求根节点到叶节点数字之和
int sumNumbers(TreeNode* root) {
    int ans = 0;
    std::function<void(TreeNode *, int)> dfs;
    dfs = [&](TreeNode *root, int x) {
        if (root == nullptr) {
            return;
        }
        x = x * 10 + root->val;
        if (root->left == nullptr && root->right == nullptr) {
            ans += x;
            return;
        }
        dfs(root->left, x);
        dfs(root->right, x);
    };

    dfs(root, 0);
    return ans;
}

// 1448统计二叉树中好节点的数目
int goodNodes(TreeNode* root) {
    int ans = 0;
    std::function<void(TreeNode *, int)> dfs;
    dfs = [&](TreeNode *root, int pre_max) {
        if (root == nullptr) {
            return;
        }
        ans += root->val >= pre_max;
        pre_max = max(pre_max, root->val);

        dfs(root->left, pre_max);
        dfs(root->right, pre_max);
    };
    dfs(root, INT_MIN);
    return ans;
}

// 987二叉树的垂序遍历
vector<vector<int>> verticalTraversal(TreeNode* root) {
    // col, row, val
    vector<tuple<int, int, int>> data;
    std::function<void(TreeNode*, int row, int col)> dfs =
        [&](TreeNode* root, int row, int col) {
            if (root == nullptr) {
                return;
            }
            data.emplace_back(col, row, root->val);
            dfs(root->left, row + 1, col - 1);
            dfs(root->right, row + 1, col + 1);
    };
    dfs(root, 0, 0);

    vector<vector<int>> ans;
    // col, row, val都是从小到大排序
    ranges::sort(data);
    int cur_col = INT_MIN;
    for (auto &[col, row, val] : data) {
        // 新列的数组
        if (cur_col != col) {
            cur_col = col;
            ans.push_back({});
        }
        ans.back().push_back(val);
    }
    return ans;
}
}
namespace BinaryTree_Apply {
// 100相同的树
bool isSameTree(TreeNode* p, TreeNode* q) {
    if (p == nullptr || q == nullptr) {
        return p == q;
    }
    return (p->val == q->val)
           && isSameTree(p->left, q->left)
           && isSameTree(p->right, q->right);
}

// 101对称二叉树
bool isSymmetric(TreeNode* root) {
    if (root == nullptr) {
        return false;
    }
    function<bool(TreeNode *, TreeNode *)> is_same =
        [&](TreeNode *p, TreeNode *q) {
            if (p == nullptr || q == nullptr) {
                return p == q;
            }
            return p->val == q->val && is_same(p->left, q->right) && is_same(p->right, q->left);
        };
    return is_same(root->left, root->right);
}

// 110平衡二叉树
bool isBalanced(TreeNode* root) {
    function<int(TreeNode* root)> get_height = [&](TreeNode* root) {
        if (root == nullptr) {
            return 0;
        }
        int left_h = get_height(root->left);
        if (left_h == -1) {
            return -1;// 提前退出递归
        }
        int right_h = get_height(root->right);
        if (right_h == -1 || abs(left_h - right_h) > 1) {
            return -1;
        }
        return max(left_h, right_h) + 1;
    };
    return get_height(root) != -1;
}

// 199二叉树的右视图
vector<int> rightSideView(TreeNode* root) {
    vector<int> ans;
    function<void(TreeNode *root, int dep)> dfs = [&](TreeNode *root, int dep) {
        if (root == nullptr) {
            return;
        }
        if (ans.size() == dep) {
            ans.emplace_back(root->val);
        }
        dfs(root->right, dep + 1);
        dfs(root->left, dep + 1);
    };
    dfs(root, 0);
    return ans;
}

// 226翻转二叉树
TreeNode* invertTree(TreeNode* root) {
    if (root == nullptr) {
        return root;
    }
    swap(root->left, root->right);
    invertTree(root->left);
    invertTree(root->right);
    return root;
}

// 617合并二叉树
TreeNode* mergeTrees(TreeNode* root1, TreeNode* root2) {
    if (root1 == nullptr) {
        return root2;
    } else if (root2 == nullptr) {
        return root1;
    } else {
        return new TreeNode(root1->val + root2->val,
                            mergeTrees(root1->left, root2->left),
                            mergeTrees(root1->right, root2->right));
    }
}

// 1026 节点与祖父之间的最大差值
int maxAncestorDiff(TreeNode* root) {
    int ans = 0;
    // 以node为根的最小值和最大值
    // 返回[min, max]
    function<pair<int, int>(TreeNode * node)> dfs =
        [&](TreeNode* node) -> pair<int, int> {
        if (node == nullptr) {
            return {INT_MAX, INT_MIN};// 返回最大值和最小.避免有影响
        }
        auto [l_min, l_max] = dfs(node->left);
        auto [r_min, r_max] = dfs(node->right);
        int mn = min(node->val, min(l_min, r_min));
        int mx = max(node->val, max(l_max, r_max));
        ans = max(ans, max(node->val - mn, mx - node->val));
        return {mn, mx};
    };
    dfs(root);
    return ans;
}

// 1080根到叶路径的不足节点
TreeNode* sufficientSubset(TreeNode* root, int limit) {
    limit -= root->val;
    // 叶子节点, 走过的路程<给定的限制,就要删除
    if (root->left == nullptr && root->right == nullptr) {
        return limit > 0 ? nullptr : root;
    }
    if (root->left) root->left = sufficientSubset(root->left, limit);
    if (root->right) root->right = sufficientSubset(root->right, limit);
    // 如果非叶子节点的左右节点都被删除,说明经过node的所有路径(根节点-叶子节点)都小于limit
    // 所以node也要被删除,
    return root->left == nullptr && root->right == nullptr ? nullptr : root;

}
}
namespace BinaryTree_ThreeOrder {
// 98验证二叉搜索树
bool isValidBST(TreeNode* root) {
    function<pair<long long, long long>(TreeNode *node)> dfs = [&](TreeNode *node) -> pair<long long, long long> {
        if (node == nullptr) {
            // 是二叉搜索树
            return {LLONG_MAX, LLONG_MIN};
        }
        auto [l_min, l_max] = dfs(node->left);
        auto [r_min, r_max] = dfs(node->right);
        long long x = node->val;
        // 这种情况下不是二叉搜索树
        if (x <= l_max || x >= r_min) {
            return {LLONG_MIN, LLONG_MAX};
        }
        // 二叉搜索树, 左边小于根小于右边
        return {min(l_min, x), max(r_max, x)};
    };
    // 根据结果与{LLONG_MIN, LLONG_MAX}不匹配就是二叉搜索树
    return dfs(root).first != LLONG_MIN;
}

// 938二叉搜索树的范围和
int rangeSumBST(TreeNode* root, int low, int high) {
    if (root == nullptr) {
        return 0;
    }
    int val = root->val;
    if (val >= low && val <= high) {
        return val + rangeSumBST(root->left, low, val) + rangeSumBST(root->right, val, high);
    } else {
        return rangeSumBST(root->left, low, high) + rangeSumBST(root->right, low, high);
    }
}

// 2476二叉搜索树最近节点查询
vector<vector<int>> closestNodes(TreeNode* root, vector<int>& queries) {
    vector<int> a;
    long long pre = LLONG_MAX;
    // 知道中序遍历,但不知道怎么实现
    function<void(TreeNode*)> dfs = [&](TreeNode* node){
        if (node == nullptr) {
            return;
        }
        dfs(node->left);
        a.push_back(node->val);
        dfs(node->right);
    };
    // 第一个 >= x 的位置
    auto lower_bound = [&](vector<int> &nums, int x) {
        int left = -1;
        int right = nums.size();
        while (left + 1 < right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] >= x) {
                right = mid;
            } else {
                left = mid;
            }
        }
        return right;
    };
    dfs(root);
    int n = a.size();
    vector<vector<int>> ans;
    for (auto x : queries) {
        int id = lower_bound(a, x);
        // max
        int mx = id < n ? a[id] : -1;
        // min
        int mn;
        if (id < n && a[id] == x) {
            mn = a[id];
        } else {
            mn = id ? a[id - 1] : -1;
        }
        ans.push_back({mn, mx});
    }
    return ans;
}

// 230二叉搜索树中第K小的元素
int kthSmallest(TreeNode* root, int k) {
    vector<int> nums;
    function<void(TreeNode*)> dfs = [&](TreeNode *node) {
        if (node == nullptr) {
            return;
        }
        dfs(node->left);
        nums.push_back(node->val);
        dfs(node->right);
    };
    dfs(root);
    int n = nums.size() - 1;
    int id = min(k - 1, n);
    return nums[id];
}

// 1373 二叉搜索子树的最大键值和
int maxSumBST(TreeNode* root) {
    int ans = 0;
    // 后序遍历, 最小值, 最大值, 总和
    function<tuple<int, int, int>(TreeNode*)> dfs = [&](TreeNode *node) -> tuple<int, int, int> {
        if (node == nullptr) {
            // 不成立的数
            return {INT_MAX, INT_MIN, 0};
        }
        auto [l_min, l_max, l_sum] = dfs(node->left);
        auto [r_min, r_max, r_sum] = dfs(node->right);
        int val = node->val;
        if (val <= l_max || val >= r_min) {
            // 不是二叉搜索树
            return {INT_MIN, INT_MAX, 0};
        }
        int s = l_sum + r_sum + val;
        ans = max(ans, s);
        return {min(l_min, val), max(r_max, val), s};
    };
    dfs(root);
    return ans;
}

// 105从前序与中序遍历序列构造二叉树
TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
    if (preorder.empty()) {
        return nullptr;
    }
    // [left, right)
    int left_size = ranges::find(inorder, preorder[0]) - inorder.begin();
    vector<int> pre1(preorder.begin() + 1, preorder.begin() + 1 + left_size);
    vector<int> pre2(preorder.begin() + 1 + left_size, preorder.end());
    vector<int> in1(inorder.begin(), inorder.begin() + left_size);
    vector<int> in2(inorder.begin() + 1 + left_size, inorder.end());
    TreeNode *left = buildTree(pre1, in1);
    TreeNode *right = buildTree(pre2, in2);
    return new TreeNode(preorder[0], left, right);
}

// 106从中序与后序遍历序列构造二叉树
TreeNode* buildTree2(vector<int>& inorder, vector<int>& postorder) {
    if (postorder.empty()) {
        return nullptr;
    }
    int left_size = ranges::find(inorder, postorder.back()) - inorder.begin();
    vector<int> in1(inorder.begin(), inorder.begin() + left_size);
    vector<int> in2(inorder.begin() + left_size + 1, inorder.end());
    vector<int> post1(postorder.begin(), postorder.begin() + left_size);
    vector<int> post2(postorder.begin() + left_size, postorder.end() - 1);
    TreeNode *left = buildTree2(in1, post1);
    TreeNode *right = buildTree2(in2, post2);
    return new TreeNode(postorder.back(), left, right);
}

// 889从前序与后序遍历序列构造二叉树
TreeNode* constructFromPrePost(vector<int>& preorder, vector<int>& postorder) {
    if (preorder.empty()) {
        return nullptr;
    }
    if (preorder.size() == 1) {
        return new TreeNode(preorder[0]);
    }
    // 前序第二个值是左子树的根
    // [left, right), left_size是第一个找到preorder[1]的id(从0开始)
    int left_size = ranges::find(postorder, preorder[1]) - postorder.begin();
    vector<int> pre1(preorder.begin() + 1, preorder.begin() + 1 + left_size + 1);
    vector<int> pre2(preorder.begin() + 1 + left_size + 1, preorder.end());
    vector<int> post1(postorder.begin(), postorder.begin() + left_size + 1);
    vector<int> post2(postorder.begin() + left_size + 1, postorder.end() - 1);
    TreeNode *left = constructFromPrePost(pre1, post1);
    TreeNode *right = constructFromPrePost(pre2, post2);
    return new TreeNode(preorder.front(), left, right);
}

// 1110删点成林
vector<TreeNode*> delNodes(TreeNode* root, vector<int>& to_delete) {
    unordered_set<int> s;
    for (auto a : to_delete) {
        s.insert(a);
    }
    vector<TreeNode*> ans;
    function<TreeNode*(TreeNode*)> dfs = [&](TreeNode *node) ->TreeNode* {
        if (node == nullptr) {
            return nullptr;
        }
        node->left = dfs(node->left);
        node->right = dfs(node->right);
        // 当前节点没有被删除, 返回本身,
        if (s.count(node->val) == 0) {
            return node;
        }
        // 如果被删除, 左(右)节点存在就是孤立的树,需要放入ans
        if (node->left) ans.push_back(node->left);
        if (node->right) ans.push_back(node->right);
        // 被删除了,返回空节点
        return nullptr;
    };
    auto node = dfs(root);
    if (node) {
        ans.push_back(node);
    }
    return ans;
}
}
namespace BinaryTree_Ancestor {
// 236二叉树的最近公共祖先
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    if (root == nullptr || root == p || root == q) {
        return root;
    }
    TreeNode *left = lowestCommonAncestor(root->left, p, q);
    TreeNode *right = lowestCommonAncestor(root->right, p, q);
    if (left && right) {
        return root;
    }
    // left不存在, right存在返回right, right不存在返回nullptr(right也是nullptr)
    return left ? left : right;
}

// 235二叉搜索树的最近公共祖先
TreeNode* lowestCommonAncestor2(TreeNode* root, TreeNode* p, TreeNode* q) {
    if (root == nullptr) {
        return nullptr;
    }
    int x = root->val;
    if (p->val < x && q->val < x) {
        return lowestCommonAncestor2(root->left, p, q);
    }
    if (p->val > x && q->val > x) {
        return lowestCommonAncestor2(root->right, p, q);
    }
    return root;
}

// 1123最深叶节点的最近公共祖先
TreeNode* lcaDeepestLeaves(TreeNode* root) {
    function<pair<int, TreeNode*>(TreeNode *)> dfs = [&](TreeNode *node) -> pair<int, TreeNode*> {
        if (node == nullptr) {
            return {0, nullptr};
        }
        // height 是根节点到深度,
        auto [l_height, lca] = dfs(node->left);
        auto [r_height, rca] = dfs(node->right);
        if (l_height > r_height) {
            // +1 是因为要把root节点的深度加上
            return {l_height + 1, lca};
        }
        if (l_height < r_height) {
            return {r_height + 1, rca};
        }
        return {l_height + 1, node};
    };
    return dfs(root).second;
}
}
namespace BinaryTree_BFS {
// 102二叉树的层序遍历
vector<vector<int>> levelOrder(TreeNode* root) {
    if (root ==nullptr) {
        return {};
    }
    vector<vector<int>> ans;
    vector<TreeNode*> cur{root};

    while (!cur.empty()) {
        vector<TreeNode*> next;
        vector<int> val;
        for (TreeNode *node : cur) {
            val.push_back(node->val);
            if (node->left) next.push_back(node->left);
            if (node->right) next.push_back(node->right);
        }
        cur = std::move(next);
        ans.emplace_back(val);
    }
    return ans;
}

// 103二叉树的锯齿形层序遍历
vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
    if (root == nullptr) {
        return {};
    }
    vector<vector<int>> ans;
    vector<TreeNode *> cur{root};
    while (cur.size()) {
        vector<TreeNode*> next;
        vector<int> val;
        for (TreeNode *node : cur) {
            val.push_back(node->val);
            if (node->left) next.push_back(node->left);
            if (node->right) next.push_back(node->right);
        }
        cur = std::move(next);
        // ans前面有奇数层,当前就是偶数层需要翻转
        if (ans.size() % 2) ranges::reverse(val);
        ans.emplace_back(val);
    }
    return ans;
}

// 513找树左下角的值
int findBottomLeftValue(TreeNode* root) {
    TreeNode * node;
    queue<TreeNode *> q;
    q.push(root);
    while (q.size()) {
        int n = q.size();
        for (int i = 0; i < n; i++) {
            node = q.front();
            q.pop();
            if (node->right) q.push(node->right);
            if (node->left) q.push(node->left);
        }
    }
    return node->val;
}

// 二叉树的层序遍历2
vector<vector<int>> levelOrderBottom(TreeNode* root) {
    if (root == nullptr) {
        return {};
    }
    vector<vector<int>> ans;
    queue<TreeNode *> q;
    q.push(root);
    while (q.size()) {
        vector<int> val;
        int n = q.size();
        for (int i = 0; i < n; i++) {
            TreeNode *node = q.front();
            q.pop();
            val.push_back(node->val);
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        ans.push_back(val);
    }
    ranges::reverse(ans);
    return ans;
}

// 116填充每个节点的下一个右侧节点指针
Node* connect(Node* root) {
    if (root == nullptr) {
        return nullptr;
    }
    Node *node;
    queue<Node *> q;
    q.push(root);
    while (q.size()) {
        int n = q.size();
        for (int i = 0; i < n; i++) {
            node = q.front();
            q.pop();
            if (i < n - 1) {
                node->next = q.front();
            }
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
    }
    return root;
}

// 117填充每个节点的下一个右侧节点指针
Node* connect2(Node* root) {
    if (root == nullptr) {
        return nullptr;
    }
    Node dummy;
    // 根节点本身满足条件,不需要处理
    Node *cur = root;
    while (cur) {
        dummy.next = nullptr;
        // next从每一层的起始点开始
        Node *next = &dummy;
        // 每一层若干个节点
        while (cur) {
            if (cur->left) {
                next->next = cur->left;
                next = cur->left;
                // cur = cur->left;
            }
            if (cur->right) {
                next->next = cur->right;
                next = cur->right;
            }
            cur = cur->next;
        }
        // 下一层的起始点
        cur = dummy.next;
    }
    return root;
}

// 2415反转二叉树的奇数层
TreeNode* reverseOddLevels(TreeNode* root) {
    if (root == nullptr) {
        return nullptr;
    }
    vector<TreeNode*> cur{root};
    bool odd = true;
    while (cur.size()) {
        vector<TreeNode*> next{};
        vector<int> val;
        for (TreeNode *node : cur) {
            if (node->left) {
                next.push_back(node->left);
                if (odd) {
                    val.push_back(node->left->val);
                }
            }
            if (node->right) {
                next.push_back(node->right);
                if (odd) {
                    val.push_back(node->right->val);
                }
            }
        }
        ranges::reverse(val);
        for (int i = 0; i < val.size(); i++) {
            next[i]->val = val[i];
        }
        cur = std::move(next);
        odd = !odd;
    }
    return root;
}

// 2641二叉树的堂兄弟节点2
TreeNode* replaceValueInTree(TreeNode* root) {
    if (root == nullptr) {
        return nullptr;
    }
    root->val = 0;
    vector<TreeNode*> cur{root};
    while (cur.size()) {
        vector<TreeNode *> next;
        vector<int> vals;
        for (TreeNode *node : cur) {
            int val = 0;
            if (node->left) {
                val += node->left->val;
                next.push_back(node->left);
            }
            if (node->right) {
                val += node->right->val;
                next.push_back(node->right);
            }
            vals.push_back(val);
        }
        int sum = reduce(vals.begin(), vals.end(), 0);
        for (int i = 0; i < cur.size(); i++) {
            TreeNode *node = cur[i];
            int adj = sum - vals[i];
            if (node->left) {
                node->left->val = adj;
            }
            if (node->right) {
                node->right->val = adj;
            }
        }
        cur = std::move(next);
    }
    return root;
}
}

#endif // _5BINARYTREE_H
