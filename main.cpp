#include <QApplication>
#include "09LongestSubSequence.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::vector<int> vec = {5,2,13,3,8};
/*
    ListNode dummy(0, new ListNode());
    ListNode *cur = &dummy;
    for (auto v : vec) {
        cur->next = new ListNode(v);
        cur = cur->next;
    }
    //04
    LinkedList_Delete::removeNodes(dummy.next);
    delete cur;
    // 08
    ZeroOneBackbag::numSquares(1e4);
*/
    vec = {0,1,0,3,2,3};
    // 09
    LIS::lengthOfLIS_2(vec);
    return a.exec();
}
