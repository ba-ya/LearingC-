#ifndef _4LINKEDLIST_H
#define _4LINKEDLIST_H

#include "00solution.h"

#include <unordered_set>

namespace LinkedList_Reverse {
// 206反转链表
// 时间复杂度:O(n), n是节点个数, 空间复杂度:O(1)
ListNode* reverseList(ListNode* head) {
    ListNode *pre = nullptr;
    ListNode *cur = head;
    while (cur) {
        ListNode *next = cur->next;
        cur->next = pre;
        pre = cur;
        cur = next;
    }
    return pre;
}

// 92反转链表2
// 时间复杂度:O(right), 空间复杂度:O(1)
ListNode* reverseBetween(ListNode* head, int left, int right) {
    ListNode dummy(0, head);
    ListNode * p0 = &dummy;
    // p0 指向反转列表的第一个的前一个
    for (int i = 0; i < left - 1; i++) {
        p0 = p0->next;
    }

    ListNode *pre = nullptr;
    ListNode *cur = p0->next;
    for (int i = 0; i < right - left + 1; i++) {
        ListNode *next = cur->next;
        cur->next = pre;
        pre = cur;
        cur = next;
    }
    p0->next->next = cur;
    p0->next = pre;
    return dummy.next;
}

// 25k个一组反转链表
ListNode* reverseKGroup(ListNode* head, int k) {
    int cnt = 0;
    for (ListNode *cur = head; cur; cur = cur->next) {
        cnt++;
    }
    int groups = cnt / k;

    ListNode dummy(0, head);
    ListNode *p0 = &dummy;
    ListNode *pre = nullptr;
    ListNode *cur = p0->next;
    for (int i = 0; i < groups; i++) {
        for (int j = 0; j < k; j++) {
            ListNode *next = cur->next;
            cur->next = pre;
            pre = cur;
            cur = next;
        }
        ListNode *next = p0->next;
        p0->next->next = cur;
        p0->next = pre;
        p0 = next;
    }
    return dummy.next;
}

// 24两两交换链表中的节点
ListNode* swapPairs(ListNode* head) {
    int n = 0;
    for (ListNode *cur = head; cur; cur = cur->next) {
        n++;
    }

    ListNode dummy(0, head);
    ListNode *p0 = &dummy;
    int k = 2;
    ListNode *pre = nullptr;
    ListNode *cur = p0->next;
    for (; n >= k; n -= k) {
        for (int i = 0; i < k; i++) {
            ListNode *next = cur->next;
            cur->next = pre;
            pre = cur;
            cur = next;
        }
        ListNode *next = p0->next;
        p0->next->next = cur;
        p0->next = pre;
        p0 = next;
    }
    return dummy.next;
}

// 445两数相加2
ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
    auto reverse_list = [](ListNode* head) {
        ListNode dummy(0, head);
        ListNode *p0 = &dummy;
        ListNode *pre = nullptr;
        ListNode *cur = p0->next;
        while (cur) {
            ListNode *next = cur->next;
            cur->next = pre;
            pre = cur;
            cur = next;
        }
        return dummy.next;
    };
    auto add_two = [](ListNode* l1, ListNode* l2, int carry = 0) -> ListNode * {
        if (l1 == nullptr && l2 == nullptr && carry == 0) {
            return nullptr;
        }
        ListNode dummy(0);
        ListNode *cur = &dummy;
        while (l1 || l2 || carry) {
            if (l1) {
                carry += l1->val;
                l1 = l1->next;
            }
            if (l2) {
                carry += l2->val;
                l2 = l2->next;
            }
            cur->next = new ListNode(carry % 10);
            cur = cur->next;
            carry /= 10;
        }
        return dummy.next;
    };

    l1 = reverse_list(l1);
    l2 = reverse_list(l2);
    auto l3 = add_two(l1, l2);
    return reverse_list(l3);
}

// 2816翻倍以链表形式
ListNode* doubleIt(ListNode* head) {
    auto reverse_list = [](ListNode *head) {
        ListNode *pre = nullptr;
        ListNode *cur = head;
        while (cur) {
            ListNode *next = cur->next;
            cur->next = pre;
            pre = cur;
            cur = next;
        }
        return pre;
    };
    auto add_own = [](ListNode *l) {
        int carry = 0;
        ListNode dummy;
        ListNode *cur = &dummy;
        while (l || carry) {
            if (l) {
                carry += (l->val) * 2;
                l = l->next;
            }
            cur->next = new ListNode(carry % 10);
            cur = cur->next;

            carry /= 10;
        }
        return dummy.next;
    };
    head = reverse_list(head);
    ListNode *res = add_own(head);
    return reverse_list(res);
}
}

namespace LinkedList_QuickNSlow {
// 876链表的中间节点
ListNode* middleNode(ListNode* head) {
    ListNode *slow = head;
    ListNode *fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

// 141环形链表
bool hasCycle(ListNode *head) {
    ListNode *slow = head;
    ListNode *fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            return true;
        }
    }
    return false;
}

// 142环形链表2
ListNode *detectCycle(ListNode *head) {
    // 假设
    // a = head到环入口的距离
    // b = head到相遇点的距离, slow走过的路程
    // c = 环长
    // 2b = b + kc
    ListNode *slow = head;
    ListNode *fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        // 在环内的相遇点
        // slow现在在环内走过的距离是 b - a = kc - a,
        // 再+a就能得到一个完整环
        // 所有head与slow一起走,相遇的点就是入环口
        if (slow == fast) {
            while (slow != head) {
                slow = slow->next;
                head = head->next;
            }
            return slow;
        }
    }
    return nullptr;
}

// 143重排链表
void reorderList(ListNode* head) {
    auto reverse_list = [](ListNode* head) {
        ListNode *pre = nullptr;
        ListNode *cur = head;
        while (cur) {
            ListNode *next = cur->next;
            cur->next = pre;
            pre = cur;
            cur = next;
        }
        return pre;
    };
    auto find_mid = [](ListNode* head) {
        ListNode *slow = head;
        ListNode *fast = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow;
    };
    ListNode *mid = find_mid(head);
    ListNode *head2 = reverse_list(mid);
    // 1,2,3,4,5 -> mid(3) 1,2,3 和 5,4,3
    // 1,2,3,4   -> mid(3) 1,2,3 和 4,3
    // head2的中间节点head1已经有了,所有head2走到倒数第二个就行了
    while (head2->next) {
        ListNode *next1 = head->next;
        ListNode *next2 = head2->next;
        head->next = head2;
        head2->next = next1;
        head = next1;
        head2 = next2;
    }
}

// 234回文链表, 参考
bool isPalindrome(ListNode* head) {
    auto reverse_list = [](ListNode* head) {
        ListNode *pre = nullptr;
        ListNode *cur = head;
        while (cur) {
            ListNode *next = cur->next;
            cur->next = pre;
            pre = cur;
            cur = next;
        }
        return pre;
    };
    auto find_mid = [](ListNode* head) {
        ListNode *slow = head;
        ListNode *fast = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow;
    };
    ListNode *mid = find_mid(head);
    ListNode *head2 = reverse_list(mid);
    // 1,2,3,4,5 -> mid(3) 1,2,3 和 5,4,3
    // 1,2,3,4   -> mid(3) 1,2,3 和 4,3
    // 循环需要走到head的倒数第二个while(head->next)或者head2走完while(head2)
    while (head2) {
        if (head->val != head2->val) {
            return false;
        }
        head = head->next;
        head2 = head2->next;
    }
    return true;
}
}

namespace LinkedList_Delete {
// 237删除链表中的节点
void deleteNode(ListNode* node) {
    ListNode *next = node->next;
    // 解引用, 把值和指针都给node了
    // 解引用, 此时是第二个指针不用
    *node = *next;
    delete next;
}

// 19删除链表倒数第N个节点
ListNode* removeNthFromEnd(ListNode* head, int n) {
    // need dummy
    // 如果遇到需要删除头节点的题目，添加哨兵节点可以简化代码逻辑
    ListNode dummy(0, head);
    ListNode *right = &dummy;
    ListNode *left = &dummy;
    for (int i = 0; i < n; i++) {
        right = right->next;
    }

    // 如果要遍历到最后一个节点，需要写 while node;
    // 如果要遍历到倒数第二个节点，需要写 while node.next。
    while (right->next) {
        left = left->next;
        right = right->next;
    }
    // 此时left是倒数第n+1个节点
    ListNode *next = left->next;
    // 赋值, 此时是第一个指针不用
    left->next = left->next->next;
    delete next;
    return dummy.next;
}

// 83删除排序链表中的重复元素
ListNode* deleteDuplicates(ListNode* head) {
    if (head == nullptr) {
        return nullptr;
    }
    ListNode *cur = head;
    while (cur->next) {
        if (cur->val == cur->next->val) {
            ListNode *next = cur->next;
            cur->next = cur->next->next;
            delete next;
        } else {
            cur = cur->next;
        }
    }
    return head;
}

// 82删除排序链表中的重复元素2
ListNode* deleteDuplicates2(ListNode* head) {
    ListNode dummy(0, head);
    ListNode *cur = &dummy;
    // 比较后面两个值
    while (cur->next && cur->next->next) {
        int val = cur->next->val;
        if (val == cur->next->next->val) {
            while (cur->next && cur->next->val == val) {
                ListNode *next = cur->next;
                cur->next = cur->next->next;
                delete next;
            }
        } else {
            cur = cur->next;
        }
    }
    return dummy.next;
}

// 203移除链表元素
ListNode* removeElements(ListNode* head, int val) {
    ListNode dummy(0, head);
    ListNode *cur = &dummy;
    while (cur->next) {
        ListNode *next = cur->next;
        if (cur->next->val == val) {
            cur->next = cur->next->next;
            delete next;
        } else {
            cur = next;
        }
    }
    return dummy.next;
}

// 3217从链表中移除在数组中存在的节点
ListNode* modifiedList(vector<int>& nums, ListNode* head) {
    // set查找时间复杂度是O(1)
    unordered_set<int> st(nums.begin(), nums.end());
    ListNode dummy(0, head);
    ListNode *cur = &dummy;
    while (cur->next) {
        ListNode *next = cur->next;
        if (st.find(cur->next->val) != st.end()) {
            cur->next = cur->next->next;
            // 应该是力扣自己回收了,添加delete会报错
            // delete next;
        } else {
            cur = next;
        }
    }
    return dummy.next;
}

// 2487从链表中移除节点
ListNode* removeNodes(ListNode* head) {
    auto reverse_list = [](ListNode* head) {
        ListNode *pre = nullptr;
        ListNode *cur = head;
        while (cur) {
            ListNode *next = cur->next;
            cur->next = pre;
            pre = cur;
            cur = next;
        }
        return pre;
    };
    head = reverse_list(head);
    ListNode *cur = head;
    while (cur->next) {
        ListNode *next = cur->next;
        if (cur->val > cur->next->val) {
            cur->next = cur->next->next;
            delete next;
        } else {
            cur = next;
        }
    }
    return reverse_list(head);
}
}

#endif // _4LINKEDLIST_H
