#ifndef _4LINKEDLIST_H
#define _4LINKEDLIST_H

#include "00solution.h"

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

namespace LinkedList_Reverse {
// 206反转链表
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
    ListNode dummy(0, head);
    ListNode *p0 = &dummy;
    int cnt = 0;
    while (p0->next) {
        p0 = p0->next;
        cnt++;
    }

    p0 = &dummy;
    int groups = cnt / k;
    ListNode *pre;
    ListNode *cur;
    ListNode *p1;
    for (int i = 0; i < groups; i++) {
        int left = i * k;
        int right = (i + 1) * k - 1;
        pre = nullptr;
        cur = p0->next;
        p1 = cur;
        for (int j = 0; j < right - left + 1; j++) {
            ListNode *next = cur->next;
            cur->next = pre;
            pre = cur;
            cur = next;
        }
        p0->next->next = cur;
        p0->next = pre;
        p0 = p1;
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

}

namespace LinkedList_Delete {

}

#endif // _4LINKEDLIST_H
