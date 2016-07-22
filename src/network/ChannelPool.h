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

namespace mars {

class ChannelPool : public NonCopyable {
public:
	ChannelPool();
	ChannelPool(int size);
	~ChannelPool();

	ChannelPtr acquire(EventLoop* event_loop, int sockfd, const IpAddress& local_address, const IpAddress& peer_address);

	int size()   const { return deque_.size();  }
	bool empty() const { return deque_.empty(); }

private:
	std::deque<Channel*> deque_;
};

}

#endif