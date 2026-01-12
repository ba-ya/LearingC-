#include <QApplication>

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
    vec = {0,1,0,3,2,3};
    // 09
    LIS::lengthOfLIS_2(vec);

    vec = {4,5,6,5};
    std::vector<int> vec2 = {2,1,2,1};
    LIS::bestTeamScore(vec, vec2);

    // 11
    vec = {1,3,4,3,2};
    Monotonic_Stack::largestRectangleArea(vec);
    Monotonic_Stack::largestRectangleArea_2(vec);
*/
    auto func2 = [&](int *p) {
        qDebug() << "p addr:"<< &p << ", p val:" << p;
    };
    int var = 1;
    int *p = &var;
    qDebug() << "var addr:" << &var;
    qDebug() << "p addr:"<< &p << ", p val:" << p;
    func2(p);

    return a.exec();
}
