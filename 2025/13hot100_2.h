#ifndef _3HOT1___2_H
#define _3HOT1___2_H

#include "00solution.h"

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


/// 二分查找

/// 栈

/// 堆

/// 贪心算法

/// 动态规划

/// 多维动态规划

/// 技巧
}

#endif // _3HOT1___2_H
