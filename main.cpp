#include <QApplication>
#include "01TwoPointersTowards.h"
#include "02SlidingWindow.h"
#include "03BinarySearch.h"
#include "04LinkedList.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::vector<int> vec = {30,11,23,4,20};
    std::vector<std::vector<int>> vecs = {{1}, {3}};
    int target = 8;
    BinarySearch2::searchMatrix(vecs, 3);
    return a.exec();
}
