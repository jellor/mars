/*========================================================
 *
 *   文件名称 ：ThreadPool.h
 *   创建日期 ：2016.6.9
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include "NonCopyable.h"
#include "Thread.h"
#include "Task.h"
#include "Condition.h"
#include "Log.h"
#include <queue>

namespace mars {

class ThreadPool : public NonCopyable {
public:
	explicit ThreadPool(unsigned int thread_count);
	~ThreadPool();
	
	void start();
	void stop();
	void push(const Task& task);
	unsigned int size() const { return thread_count_; }
private:
	void runThread();

	const unsigned int thread_count_; 
	bool started_;
	std::queue<Task> tasks_;
	Mutex mutex_;
	Condition condition_;
	Thread** threads_;
};

}

#endif