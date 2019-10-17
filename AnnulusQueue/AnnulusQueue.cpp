/*************************************************************************
	> File Name: AnnulusQueue.cpp
	> Author: Mr.Miaow
	> Mail: miaopei163@163.com 
	> Created Time: 2019年10月17日 星期四 18时05分17秒
 ************************************************************************/

#include <iostream>

#include "AnnulusQueue.h"

namespace queue {

AnnulusQueue::AnnulusQueue(int nCapacity)
{
    m_nQueueCapacity = nCapacity;
    m_nQueueHead     = 0;
    m_nQueueTail     = 0;
    m_nQueueLen      = 0;

    //创建队列  //队列中存放的元素为int型，也可为其它复杂的结构体
    m_pQueue = new int[m_nQueueCapacity];
}

AnnulusQueue::~AnnulusQueue()
{
    delete []m_pQueue;
    m_pQueue = NULL;
}

void AnnulusQueue::ClearQueue()
{
    m_nQueueHead = 0;
    m_nQueueTail = 0;
    m_nQueueLen  = 0;
}

bool AnnulusQueue::IsQueueEmpty()const
{
    return m_nQueueLen == 0 ? true : false;
}

int AnnulusQueue::GetQueueLength()const
{
    return m_nQueueLen;
}

bool AnnulusQueue::IsQueueFull()const
{
    if (m_nQueueLen == m_nQueueCapacity){
        return true;
    }
    else  return false;
}

bool AnnulusQueue::InsertItemToQueue(int nItem)
{
    if (IsQueueFull()){
        return false;
    } else{
        m_pQueue[m_nQueueTail] = nItem;
        m_nQueueTail++;
        m_nQueueTail = m_nQueueTail % m_nQueueCapacity;
        m_nQueueLen++;
        return true;
    }
}

bool AnnulusQueue::DeleteItemFromQueue(int &nItem)
{
    if (IsQueueEmpty()){
        return false;
    } else {
        nItem = m_pQueue[m_nQueueHead];
        m_nQueueHead++;
        m_nQueueHead = m_nQueueHead % m_nQueueCapacity;
        m_nQueueLen--;
        return true;
    }
}

void AnnulusQueue::QueueTraverse()
{
    for (int i = m_nQueueHead;i < m_nQueueLen + m_nQueueHead;i++) {
        std::cout << m_pQueue[i % m_nQueueCapacity] << std::endl;
    }
}

}

