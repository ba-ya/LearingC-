#include <QApplication>
#include "01TwoPointersTowards.h"
#include "02SlidingWindow.h"
#include "03BinarySearch.h"
#include "04LinkedList.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::vector<int> vec = {4,3,3,4};
    std::vector<std::vector<int>> vecs = {{1}, {3}};
    int target = 8;

    ListNode dummy(0, new ListNode());
    ListNode *head = &dummy;
    for (auto v : vec) {
        head->next = new ListNode(v);
        head = head->next;
    }
    LinkedList_QuickNSlow::isPalindrome(dummy.next);
    return a.exec();
}
