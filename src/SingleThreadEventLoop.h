/*========================================================
 *
 *   文件名称 ：SingleThreadEventLoop.h
 *   创建日期 ：2016.6.19
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __SINGLE_THREAD_EVENT_LOOP_H__
#define __SINGLE_THREAD_EVENT_LOOP_H__

#include "Thread.h"
#include "EventLoop.h"
#include "Channel.h"

namespace mars {

class SingleThreadEventLoop {
public:
	SingleThreadEventLoop();
	~SingleThreadEventLoop();

	void start();
	bool join();
	EventLoop* getEventLoop() const;
	bool isStarted() const { return event_loop_ != nullptr; }
	void setThreadName(const std::string& name);

private:
	void runInThread();
	std::atomic<bool> started_;
	Thread thread_;	
	EventLoop* event_loop_;
};

}

#endif