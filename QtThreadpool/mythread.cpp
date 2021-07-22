#include "mythread.h"
#include <QElapsedTimer>
#include <QDebug>
#include <QThread>

Generate::Generate(QObject *parent) :   QObject(parent), QRunnable()
{
    setAutoDelete(true);
}

void Generate::recvNum(int num)
{
    m_num = num;
}

void Generate::run()
{
    qDebug() << "生成随机数线程的线程地址：" << QThread::currentThread();
    QVector<int> list;
    QElapsedTimer time;
    time.start();
    for(int i = 0; i < m_num; ++i)
    {
        list.push_back(qrand() % 100000);
    }
    int milsec = time.elapsed();
    qDebug() << "生成" << m_num << "个随机数总共用时：" << milsec << "毫秒";
    emit sendArray(list);
}

BubbleSort::BubbleSort(QObject *parent) : QObject(parent), QRunnable()
{
     setAutoDelete(true);
}

void BubbleSort::recvArray(QVector<int> list)
{
    m_list = list;

}

void BubbleSort::run()
{
    qDebug() << "冒泡排序线程的线程地址：" << QThread::currentThread();
    QElapsedTimer time;
    time.start();
    int temp;
    for(int i = 0; i < m_list.size(); ++i){
        for(int j = 0; j < m_list.size()-i-1; ++j)
        {
            if(m_list[j] > m_list[j+1]){
                temp = m_list[j];
                m_list[j] = m_list[j+1];
                m_list[j+1] = temp;
            }
        }
    }

    int milsec = time.elapsed();
    qDebug() << "冒泡排序总共用时：" << milsec << "毫秒";
    emit finish(m_list);
}

QuickSort::QuickSort(QObject *parent) : QObject(parent), QRunnable()
{
     setAutoDelete(true);

}

void QuickSort::recvArray(QVector<int> list)
{
    m_list = list;
}

void QuickSort::run()
{
    qDebug() << "快速排序线程的线程地址：" << QThread::currentThread();
    QElapsedTimer time;
    time.start();
    quickSort(m_list, 0, m_list.size()-1 );

    int milsec = time.elapsed();
    qDebug() << "快速排序总共用时：" << milsec << "毫秒";
    emit finish(m_list);
}

void QuickSort::quickSort(QVector<int> &s, int l, int r)
{
    if(l < r)
    {
        int i = l, j = r;
        int x = s[l];
        while(i < j)
        {
            while(i <j && s[j] >= x)
            {
                j--;

            }
            if(i <j)
            {
                s[i++] = s[j];
            }

            while(i < j &&s[i] < x)
            {
                i++;
            }
            if(i < j)
            {
                s[j--] = s[i];
            }


        }

        s[i] = x;

        quickSort(s,l, i -1);
        quickSort(s,i+1, r);

    }

}
