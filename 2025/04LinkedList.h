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
}

namespace LinkedList_QuickNSlow {

}

namespace LinkedList_Delete {

}

#endif // _4LINKEDLIST_H
