/*========================================================
 *
 *   文件名称 ：ThreadPool.cpp
 *   创建日期 ：2016.6.9
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *   描 述 ：
 *
 *=======================================================*/

#include "ThreadPool.h"
#include "Lock.h"

using namespace mars;

ThreadPool::ThreadPool(unsigned int thread_count):
thread_count_(thread_count),
started_(false),
tasks_(),
mutex_(),
condition_(mutex_),
threads_(nullptr)
{
	threads_ = new Thread* [thread_count_];
}

ThreadPool::~ThreadPool() {
	if (threads_ != NULL) {
		DEBUG << "ThreadPool Destructoer";
		if (started_) stop();
		delete[] threads_;
	}
}

void ThreadPool::start() {
	if (started_) return;
	started_ = true;
	for (int i = 0; i < thread_count_; i ++) {
		char buf[32];
		snprintf(buf, sizeof(buf), "ThreadPool Index -> %d", i);
		threads_[i] = new Thread(std::bind(&ThreadPool::runInThread, this), buf);
		threads_[i]->start();
	}
}

void ThreadPool::stop() {
	{
		Lock lock(mutex_);
		started_ = false;	
	}
	//condition_.broadcast();
	for(int i = 0; i < thread_count_; i ++) {
		//threads_[i]->cancel();
		if (threads_[i]->join()) {
			DEBUG << "Join Ok";
		} else {
			DEBUG << "Join Fail";
		}
	}
	for (int i = 0; i < thread_count_; i ++) {
		delete threads_[i];
	}
}

void ThreadPool::push(const Task& task) {
	{
		Lock lock(mutex_);
		tasks_.push(task);
	}
	condition_.signal();
}

void ThreadPool::runInThread() {
	Task task;
	while (true) {
		{
			Lock lock(mutex_);
			while (tasks_.empty() && started_) {
				DEBUG << "Wait";
				condition_.wait();
			}
			DEBUG << "Before Break";
			if (!started_) break;
			DEBUG << "After  Break";
			task = tasks_.front();
			tasks_.pop();
		}
		DEBUG << "task.run()";
		task.run();
		DEBUG << "task.run() After";
	}
}













