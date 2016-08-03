/*========================================================
 *
 *   文件名称 ：ChannelPool.h
 *   创建日期 ：2016.5.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __CHANNEL_POOL_H__
#define __CHANNEL_POOL_H__

#include "NonCopyable.h"
#include "Channel.h"
#include "EventLoop.h"
#include "IpAddress.h"
#include <deque>
#include <set>

namespace mars {

class ChannelPool : public NonCopyable {
public:
	ChannelPool(EventLoop* event_loop);
	ChannelPool(EventLoop* event_loop, int size);
	~ChannelPool();

	ChannelPtr acquire(EventLoop* event_loop, int sockfd, const IpAddress& local_address, const IpAddress& peer_address);
	void release(const ChannelPtr& channel_ptr);

	int size()   const { return deque_.size();  }
	bool empty() const { return deque_.empty(); }

private:
	EventLoop* event_loop_;
	std::deque<Channel*> deque_;
	std::set<ChannelPtr> channel_ptr_set_;
};

}

#endif