/*========================================================
 *
 *   文件名称 ：BlockingQueue.h
 *   创建日期 ：2016.6.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __BLOCKING_QUEUE_H__
#define __BLOCKING_QUEUE_H__

#include "NonCopyable.h"
#include "Lock.h"
#include "Condition.h"
#include <deque>

namespace mars {

template<class T>
class BlockingQueue : private NonCopyable {
public:
	BlockingQueue();
	~BlockingQueue();

	void push(const T& element);
	T pop();

private:
	Mutex mutex_;
	Condition condition_;
	std::deque<T> queue_;
};

}

#endif