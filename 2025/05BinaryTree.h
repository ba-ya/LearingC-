#ifndef _5BINARYTREE_H
#define _5BINARYTREE_H

#include "00solution.h"

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

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
    // 叶子节点
    if (root->left == nullptr && root->right == nullptr) {
        return limit > 0 ? nullptr : root;
    }
    if (root->left) root->left = sufficientSubset(root->left, limit);
    if (root->right) root->right = sufficientSubset(root->right, limit);
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
}
namespace BinaryTree_Ancestor {

}
namespace BinaryTree_BFS {

}

#endif // _5BINARYTREE_H
