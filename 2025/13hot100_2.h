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

}
/// 回溯

/// 二分查找

/// 栈

/// 堆

/// 贪心算法

/// 动态规划

/// 多维动态规划

/// 技巧
}

#endif // _3HOT1___2_H
