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
