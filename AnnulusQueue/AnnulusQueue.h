/*************************************************************************
	> File Name: AnnulusQueue.h
	> Author: Mr.Miaow
	> Mail: miaopei163@163.com 
	> Created Time: 2019年10月17日 星期四 17时44分12秒
 ************************************************************************/
#ifndef __ANNULUSQUEUE_H__
#define __ANNULUSQUEUE_H__

#include <iostream>

namespace queue {

class AnnulusQueue
{
public:
    AnnulusQueue(int nCapacity);        // 创建队列
    virtual ~AnnulusQueue();
    void ClearQueue();
    bool IsQueueEmpty()const;
    bool IsQueueFull()const;
    int GetQueueLength()const;
    bool InsertItemToQueue(int nItem);      // 向队尾中插入元素
    bool DeleteItemFromQueue(int &nItem);   // 删除队首元素
    void QueueTraverse();                   // 遍历队列
private:
    int* m_pQueue;          // 队列数组指针。（int型 是队列中存放元素的类型）
    int m_nQueueLen;        //队列中的元素个数
    int m_nQueueCapacity;   //队列的总容量
    int m_nQueueHead;       //队列头的索引
    int m_nQueueTail;       //队列尾的索引
};

} // namespace queue

#endif // __ANNULUSQUEUE_H__

