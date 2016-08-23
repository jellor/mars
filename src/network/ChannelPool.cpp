/*========================================================
 *
 *   文件名称 ：ChannelPool.cpp
 *   创建日期 ：2016.5.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "ChannelPool.h"
#include "Log.h"

using namespace mars;

ChannelPool::ChannelPool(EventLoop* event_loop):
event_loop_(event_loop),
deque_(),
channel_ptr_set_()
{

}

ChannelPool::ChannelPool(EventLoop* event_loop, int size):
event_loop_(event_loop),
deque_(size),
channel_ptr_set_()
{
	for (int i = 0; i < size; i ++) {
		deque_.push_front(new Channel());
	}
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
		ChannelPtr channel_ptr(new Channel(event_loop, sockfd, local_address, peer_address), [this] (Channel* p) {
			// DEBUG << "Release Channel";
			p->reset();
			deque_.push_front(p);
		});
		channel_ptr_set_.insert(channel_ptr);
		// channel_ptr->reset(event_loop, sockfd, local_address, peer_address);
		return channel_ptr;
	} else {
		Channel* ptr = deque_.front();
		
		ChannelPtr channel_ptr(ptr, [this] (Channel* p) {
			// DEBUG << "Release Channel";
			p->reset();
			deque_.push_front(p);
		});
		deque_.pop_front();
		channel_ptr_set_.insert(channel_ptr);

		channel_ptr->reset(event_loop, sockfd, local_address, peer_address);

		return channel_ptr;
	}
}

void ChannelPool::release(const ChannelPtr& channel_ptr) {
	std::set<ChannelPtr>::const_iterator it =  channel_ptr_set_.find(channel_ptr);
	// DEBUG << "Deque Count = > " << size();
	// DEBUG << "Set Count = > " << channel_ptr_set_.size();
	// DEBUG << "Use Count = > " << channel_ptr.use_count();
	if (it != channel_ptr_set_.end()) {
		channel_ptr_set_.erase(it);
	}
	// DEBUG << "Use Count = > " << channel_ptr.use_count();
	// DEBUG << "Set Count = > " << channel_ptr_set_.size();
	// DEBUG << "Deque Count = > " << size();
}

	




