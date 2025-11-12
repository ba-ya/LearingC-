#include <QApplication>
#include "01TwoPointersTowards.h"
#include "02SlidingWindow.h"
#include "03BinarySearch.h"
#include "04LinkedList.h"
#include "05BinaryTree.h"
#include "08ZeroOneBackbag.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#if 0
    std::vector<int> vec = {5,2,13,3,8};
    ListNode dummy(0, new ListNode());
    ListNode *cur = &dummy;
    for (auto v : vec) {
        cur->next = new ListNode(v);
        cur = cur->next;
    }
    LinkedList_Delete::removeNodes(dummy.next);
#endif
    ZeroOneBackbag::numSquares(1e4);
    return a.exec();
}
