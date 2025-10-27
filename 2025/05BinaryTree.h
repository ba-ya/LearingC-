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

}
namespace BinaryTree_ThreeOrder {

}
namespace BinaryTree_Ancestor {

}
namespace BinaryTree_BFS {

}

#endif // _5BINARYTREE_H
