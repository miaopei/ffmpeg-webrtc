/*************************************************************************
	> File Name: main.cpp
	> Author: Mr.Miaow
	> Mail: miaopei163@163.com 
	> Created Time: 2019年10月17日 星期四 18时21分19秒
 ************************************************************************/

#include <iostream>

#include "AnnulusQueue.h"

using namespace std;
using namespace queue;

int main(int argc, char *argv[])
{
    AnnulusQueue queue = AnnulusQueue(8);

    queue.InsertItemToQueue(2);
    queue.InsertItemToQueue(3);
    queue.InsertItemToQueue(4);
    queue.InsertItemToQueue(5);
    queue.InsertItemToQueue(6);
    queue.InsertItemToQueue(7);
    queue.InsertItemToQueue(8);
    queue.InsertItemToQueue(9);

    cout << "Queue length: " << queue.GetQueueLength() << endl;

    queue.QueueTraverse();

    return 0;
}

