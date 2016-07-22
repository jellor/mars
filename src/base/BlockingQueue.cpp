/*========================================================
 *
 *   文件名称 ：BlockingQueue.cpp
 *   创建日期 ：2016.5.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "BlockingQueue.h"

using namespace mars;

template<class T>
BlockingQueue<T>::BlockingQueue():
mutex_(),
condition_(mutex_),
queue_()
{

}

template<class T>
BlockingQueue<T>::~BlockingQueue() {

}

template<class T>
void BlockingQueue<T>::push(const T& element) {
	{
		Lock lock(mutex_);
		queue_.push_back(element);
	}
	condition_.signal();
}

template<class T>
T BlockingQueue<T>::pop() {
	Lock lock(mutex_);
	while (queue_.empty()) {
		condition_.wait();
	}
	T element = queue_.font();
	queue_.pop_font();
	return element;
}

