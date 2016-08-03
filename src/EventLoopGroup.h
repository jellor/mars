/*========================================================
 *
 *   文件名称 ：EventLoopGroup.h
 *   创建日期 ：2016.6.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __EVENT_LOOP_GROUP_H__
#define __EVENT_LOOP_GROUP_H__

#include "NonCopyable.h"
#include "EventLoop.h"
#include "SingleThreadEventLoop.h"
#include "IpAddress.h"
#include "Channel.h"

namespace mars {

class EventLoopGroup : private NonCopyable {
public:
	EventLoopGroup(unsigned int thread_count = 1);
	~EventLoopGroup();

	void start();
	bool join();
	bool isStarted() const { return started; }
	EventLoop* next(); // before should invoke start().  
	ChannelPtr push(int fd, const IpAddress& local_address, const IpAddress& peer_address);
	SingleThreadEventLoop* getThread(int index);
	unsigned int size() const { return thread_count_; }

private:
	void runInEventLoop(const ChannelPtr& channel_ptr);
	const unsigned int thread_count_;
	std::atomic<bool> started;
	int thread_index_;
	SingleThreadEventLoop** threads_;
};

}

#endif