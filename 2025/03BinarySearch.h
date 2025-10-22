#ifndef _3BINARYSEARCH_H
#define _3BINARYSEARCH_H

#include "00solution.h"

namespace BinarySearch1 {
// 34在排序数组查找元素第一个和最后一个位置
vector<int> searchRange(vector<int>& nums, int target) {
    // 返回 >= target的一个元素id
    auto lower_bound = [](vector<int>& nums, int target) {
        // 双开区间(left, right)
        int left = -1;
        int right = nums.size();
        while (left + 1 < right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] >= target) {
                right = mid;
            } else {
                left = mid;
            }
        }
        return right;
    };
    // >= target
    int start = lower_bound(nums, target);
    if (start == nums.size() || nums[start] != target) {
        return {-1, -1};
    }
    // <= target 即 >target 即 >= (target + 1) 对应id的左边一个元素
    // 有start,end必存在,不用判断end == -1
    int end = lower_bound(nums, target + 1) - 1;
    return {start, end};
}

// 2529正整数和负整数的最大计数
int maximumCount(vector<int>& nums) {
    // (left, right), >= target
    auto lower_bound = [](vector<int>& nums, int target) {
        int left = -1;
        int right = nums.size();
        while (left + 1 < right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] >= target) {
                right = mid;
            } else {
                left = mid;
            }
        }
        return right;
    };

    int n = nums.size();
    int target = 0;
    // < target -> (>= target) - 1
    int start_0 = lower_bound(nums, target) - 1;
    // > target -> >= (target + 1)
    int end_0 = lower_bound(nums, target + 1);
    return max(start_0 + 1, n - end_0);
}

// 2300咒语和药水的成功对数
vector<int> successfulPairs(vector<int>& spells, vector<int>& potions, long long success) {
    int n = spells.size();
    int m = potions.size();
    sort(potions.begin(), potions.end());
    vector<int> ans(n, 0);
    for (int i = 0; i < n; i++) {
        int left = -1;
        int right = m;
        while (left + 1 < right) {
            int mid = left + (right - left) / 2;
            if ((long long)potions[mid] * (long long)spells[i] >= success) {
                right = mid;
            } else {
                left = mid;
            }
        }
        ans[i] = m - right;
    }
    return ans;
}

// 2563统计公平数对的数目,参考
// 自己复制vector超时了
long long countFairPairs(vector<int>& nums, int lower, int upper) {
    // 第一个满足>=target的位置, 这里传的参数是[left, right)
    auto lower_bound = [](vector<int> &nums, int left, int right, int target) {
        int l = left - 1;
        int r = right;
        while (l + 1 < r) {
            int mid = l + (r - l) / 2;
            if (nums[mid] >= target) {
                r = mid;
            } else {
                l = mid;
            }
        }
        return r;
    };
    // 第一个满足>target(等价于>= target + 1)的位置
    auto upper_bound = [lower_bound](vector<int> &nums, int left, int right, int target){
        return lower_bound(nums, left, right, target + 1);
    };

    long long ans = 0;
    sort(nums.begin(), nums.end());
    int n = nums.size();
    for (int i = 0; i < n; i++) {
        int start = lower_bound(nums, 0, i, lower - nums[i]);
        int end = upper_bound(nums, 0, i, upper - nums[i]);
        // [start, end - 1]区间所有位置都满足
        // cnt = end - 1 - start + 1 = end - start
        ans += end - start;
    }
    return ans;
}

// 2080区间查询数字的频率, 参考
// 存每个数字出现的下标,下标数组是递增的
class RangeFreqQuery {
private:
    // 每个数字的下标列表
    unordered_map<int, vector<int>> pos;

public:
    RangeFreqQuery(vector<int>& arr) {
        for (int i = 0; i < arr.size(); ++i) {
            pos[arr[i]].push_back(i);
        }
    }

    int query(int left, int right, int value) {
        auto it = pos.find(value);
        if (it == pos.end()) {
            return 0;
        }
        auto &vec = it->second;
        int start = lower_bound(vec, left);
        int end = upper_bound(vec, right);
        return end - start;
    }

private:
    // 第一个>=target
    int lower_bound(vector<int> &nums, int target) {
        int l = - 1;
        int r = nums.size();
        while (l + 1 < r) {
            int mid = l + (r - l) / 2;
            if (nums[mid] >= target) {
                r = mid;
            } else {
                l = mid;
            }
        }
        return r;
    }
    // 第一个>target
    int upper_bound(vector<int> &nums, int target) {
        return lower_bound(nums, target + 1);
    }
};

// 275, H指数, 参考
int hIndex(vector<int>& citations) {
    int n = citations.size();
    // 1篇必定h值为1, 二分范围可以是[1, n]
    int ans = 0;
    // 二分答案
    // 循环不变量：
    // left - 1的回答一定为「是」
    // right + 1 的回答一定为「否」
    int left= 0;// 不满足条件
    int right = n; // 满足条件
    while (left + 1 < right) {
        int mid = left + (right - left) / 2;
        // 至少有mid篇大于mid, 右边较大的mid个数需要大于mid
        if (citations[n - mid] >= mid) {
            // 此时可以判断是来,mid左边都是满足条件的,left可以更新
            left = mid;
        } else {
            right = mid;
        }
    }
    return left;
}

// 875爱吃香蕉的珂珂, 参考
int minEatingSpeed(vector<int>& piles, int h) {
    // 吃完某堆香蕉用的时间, p / k 向上取整 等价于  (p - 1) / k + 1 向下取整,
    // 从 [0, n - 1]范围内, 所有的和 <= h
    auto check = [&](int k) {
        int sum = 0;
        for (auto p : piles) {
            sum += (p - 1) / k + 1;
            if (sum > h) {
                return false;
            }
        }
        return true;
    };

    int max = *max_element(piles.begin(), piles.end());
    // 二分的是答案k(每小时吃的香蕉数), 区间是[1, max]
    int left = 0;// 循环不变量, 一定不满足条件
    int right = max;// 循环不变量, 一定满足条件
    while (left + 1 < right) {
        int mid = left + (right - left) / 2;
        if (check(mid)) {
            right = mid;
        } else {
            left = mid;
        }
    }
    return right;
}

// 2187完成旅途的最少时间
long long minimumTime(vector<int>& time, int totalTrips) {
    // k / t 向下取整
    // 所有的和是旅程数,需要大于等于totalTrips
    auto check = [&](long long k) {
        long long sum = 0;
        for (auto &t : time) {
            sum += k / t;
        }
        return sum >= totalTrips;
    };

    long long min = *min_element(time.begin(), time.end());
    // 同样二分答案, 区间[1, 最小时间*totalTrips]
    long long left = 0; // 循环不变量, check(left)必不满足条件
    long long right = (long long)min * totalTrips;// 循环不变量, check(right)必满足条件
    while (left + 1 < right) {
        long long mid = left + (right - left) / 2;
        if (check(mid)) {
            right = mid;
        } else {
            left = mid;
        }
    }
    return right;
}

// 2861最大合金数, 参考
int maxNumberOfAlloys(int n, int k, int budget,// n类金属,k台设备
                      vector<vector<int>>& composition, // 制造合金需要的每类金属的数目
                      vector<int>& stock, //已有的
                      vector<int>& cost) {// 每类金属的价格
    // 满足条件的最左边一个数, 一份合金也需要,必满足
    int ans = 0;
    // 满足条件的最右边一个数,
    // 假设合金只需要一份金属,价格为1,此时合金数目最多就是 min(存量) + 预算/ 1
    int max = *min_element(stock.begin(), stock.end()) + budget;
    auto check = [&](int comp_id, int cnt_rst) {
        auto &need = composition[comp_id];
        long long sum = 0;
        for (int i = 0; i < need.size(); i++) {
            long long needs = (long long)need[i] * cnt_rst;
            if (needs > stock[i]) {
                sum += (needs - stock[i]) * cost[i];
                if (sum > budget) {
                    return false;
                }
            }
        }
        return true;
    };
    // 设备数目K
    for (int i = 0; i < k; i++) {
        // 0已经知道是必定满足条件的了,二分区间可以不包含了
        // 所以left = 0, 这里把left作为满足条件, right作为不满足的
        // 二分区间(0, max + 1),
        int left = ans;// 循环不变量, 必满足
        int right = max + 1; // 循环不变量, 必不满足
        while (left + 1 < right) {
            int mid = left + (right - left) / 2;
            (check(i, mid) ? left : right) = mid;
        }
        ans = left;
    }
    return ans;

}

// 2439最大化数组里的最小值, 参考
// 自己写的超时了,但思路是对的,第一个数是最大值的时候就找到答案了
int minimizeArrayValue(vector<int>& nums) {
    // 所有超过limit的部分从右向左移动,
    // 到最左边的时候能不能小于limit
    auto check = [&](int limit) ->bool {
        long long extra = 0;
        for (int i = nums.size() - 1; i > 0; i--) {
            extra = max(nums[i] + extra - limit, 0LL);
        }
        return nums[0] + extra <= limit;
    };

    int min = *min_element(nums.begin(), nums.end());
    int max = *max_element(nums.begin(), nums.end());
    int left = min - 1;// 循环不变量, left一定不大于数组内所有数, 不满足
    int right = max; // 循环不变量, right一定大于数组内所有数,满足
    while (left + 1 < right) {
        int mid = left + (right - left) / 2;
        (check(mid) ? right : left) = mid;
    }
    return right;

}

// 2517礼盒的最大甜蜜度, 参考
int maximumTastiness(vector<int>& price, int k) {
    // 甜蜜度越大, 可选的种类k越小
    auto f = [&](int d) ->int {
        int cnt = 1, pre = price[0];
        for (auto p : price) {
            if (p - pre >= d) {
                cnt++;
                pre = p;
            }
        }
        return cnt;
    };

    sort(price.begin(), price.end());
    // price[0] + (k - 1) * d <= price[n - 1]
    // d <= (price[n - 1] - price[0]) / (k - 1)
    // f(d + 1) < k
    int left = 0;// f(left) >= k,
    int right = (price.back() - price[0]) / (k - 1) + 1;// f(right) < k
    while (left + 1 < right) {
        int mid = left + (right - left) / 2;
        (f(mid) >= k ? left : right) = mid;
    }
    return left;
}
}

namespace BinarySearch2 {
// 162寻找峰值
int findPeakElement(vector<int>& nums) {
    // [0, n - 2]->(-1, n - 1)
    // while里面有mid + 1,所有范围限制为n-2
    int n = nums.size();
    int left = -1;
    int right = n - 1;
    while (left + 1 < right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] > nums[mid + 1]) {
            right = mid;
        } else {
            left = mid;
        }
    }
    return right;
}

// 153寻找旋转排序数组中的最小值
int findMin(vector<int>& nums) {
    // [0, n-2]
    int n = nums.size();
    int left = -1;//不满足
    int right = n - 1;// 满足
    while (left + 1 < right) {
        int mid = left + (right - left) / 2;
        (nums[mid] > nums.back() ? left : right) = mid;
    }
    return nums[right];
}

// 33搜索旋转排序数组
int search(vector<int>& nums, int target) {
    // 只讨论 target <= x的情况
    // [0, n - 2]
    int last = nums.back();
    // 返回x在target的右侧吗
    auto check = [&](int i) {
        int x = nums[i];
        if (x > last) {
            return target > last && x >= target;
        }
        return target > last || x >= target;
    };

    int n = nums.size();
    int left = -1;
    int right = n - 1;
    while (left + 1 < right) {
        int mid = left + (right - left) / 2;
        (check(mid) ? right : left) = mid;
    }
    return nums[right] == target ? right : -1;
}

// 74搜索二维矩阵
bool searchMatrix(vector<vector<int>>& matrix, int target) {
    auto lower_bound = [&](int i, int target) {
        auto &vec = matrix[i];
        int left = -1;
        int right = vec.size() - 1;
        while (left + 1 < right) {
            int mid = left + (right - left ) / 2;
            (vec[mid] >= target ? right : left) = mid;
        }
        return right;
    };
    int m  = matrix.size();
    int n = matrix[0].size();
    // [0, m - 1]
    int left = -1;
    int right = m - 1;
    while (left + 1 < right) {
        int mid = left + (right - left) / 2;
        (matrix[mid].back() >= target ? right : left) = mid;
    }
    int k = lower_bound(right, target);
    return k > 0 && k < n && matrix[right][k] == target;

}

// 1901寻找峰值2
vector<int> findPeakGrid(vector<vector<int>>& mat) {
    auto index_of_max = [](vector<int> &vec) -> int {
        return max_element(vec.begin(), vec.end()) - vec.begin();
    };
    // 二分区间[0, m - 2]
    int m = mat.size();
    int left = -1;
    int right = m - 1;
    while (left + 1 < right) {
        int i = left + (right - left) / 2;
        int j = index_of_max(mat[i]);
        (mat[i][j] >= mat[i + 1][j] ? right : left) = i;
    }
    return {right, index_of_max(mat[right])};
}

// 154寻找旋转排序数组里的最小值, 可能有重复元素
int findMin2(vector<int>& nums) {
    // 把nums[right]当作最大值
    int n = nums.size();
    int left = -1;
    int right = n - 1;
    while (left + 1 < right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] == nums[right]) {
            right--;
        } else if (nums[mid] > nums[right]) {
            right = mid;
        } else {
            left = mid;
        }
    }
    return nums[right];
}

}

#endif // _3BINARYSEARCH_H
