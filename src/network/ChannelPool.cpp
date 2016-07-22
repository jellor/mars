/*========================================================
 *
 *   文件名称 ：ChannelPool.cpp
 *   创建日期 ：2016.5.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "ChannelPool.h"

using namespace mars;

ChannelPool::ChannelPool():
deque_()
{

}

ChannelPool::~ChannelPool() {
	while (!deque_.empty()) {
		Channel* ptr = deque_.front();
		delete ptr;
		deque_.pop_front();
	}
}

ChannelPtr ChannelPool::acquire(EventLoop* event_loop, int sockfd, const IpAddress& local_address, const IpAddress& peer_address) {
	if (deque_.empty()) {
		ChannelPtr channelPtr(new Channel(event_loop, sockfd, local_address, peer_address), [this] (Channel* p) {
			p->reset();
			deque_.push_front(p);
		});
		return channelPtr;
	} else {
		Channel* ptr = deque_.front();
		ptr->setEventLoop(event_loop);
		ptr->setSocket(sockfd);
		ChannelPtr channelPtr(ptr, [this] (Channel* p) {
			p->reset();
			deque_.push_front(p);
		});
		deque_.pop_front();
		return channelPtr;
	}
}