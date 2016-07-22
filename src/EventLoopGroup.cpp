/*========================================================
 *
 *   文件名称 ：EventLoopGroup.cpp
 *   创建日期 ：2016.6.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "EventLoopGroup.h"
#include "Thread.h"
#include "Log.h"

using namespace mars;

EventLoopGroup::EventLoopGroup(unsigned int thread_count):
thread_count_(thread_count),
started(false),
thread_index_(-1),
threads_(nullptr)
{
	if (thread_count_ == 0) return ;
	threads_ = new SingleThreadEventLoop* [thread_count_];
	if (threads_ == nullptr) {

	}
	for (int i = 0; i < thread_count_; i ++) {
		threads_[i] = new SingleThreadEventLoop();
	}
}

EventLoopGroup::~EventLoopGroup() {
	if (threads_ == nullptr) {
		for (int i = 0; i < thread_count_; i ++) {
			delete threads_[i];
		}
		delete[] threads_;
	} 	
}

void EventLoopGroup::start() {
	if (started) return;
	for (int i = 0; i < thread_count_; i ++) {
		threads_[i]->start();
	}
	started = true;
}

bool EventLoopGroup::join() {
	for (int i = 0; i < thread_count_; i ++) {
		if (!threads_[i]->join()) return false;
	}
	return true;
}

EventLoop* EventLoopGroup::next() {
	if (thread_count_ == 0) return nullptr;
	thread_index_ = (thread_index_ + 1) % thread_count_;
	return threads_[thread_index_]->getEventLoop();
}

SingleThreadEventLoop* EventLoopGroup::getThread(int index) {
	return index >= 0 && index < thread_count_ ? threads_[index] : nullptr;
}

ChannelPtr EventLoopGroup::push(int fd, const IpAddress& local_address, const IpAddress& peer_address) {
	EventLoop* event_loop = next();
	if (event_loop == nullptr) return nullptr;
	ChannelPtr channel_ptr(new Channel(event_loop, fd, local_address, peer_address));
	event_loop->doFunc(std::bind(&EventLoopGroup::runInEventLoop, this, channel_ptr));
	return channel_ptr;
}

void EventLoopGroup::runInEventLoop(ChannelPtr channel_ptr) {
	DEBUG << "........................";
	DEBUG << "Thread Name " << Thread::getCurrentThreadName();
	DEBUG << "Fd " << channel_ptr->getSocket()->fd();
	DEBUG << "Local Address " << channel_ptr->getLocalAddress().toString();
	DEBUG << "Peer  Address " << channel_ptr->getPeerAddress().toString();
	//ChannelHandlerPtr channelHandlerPtr(new ChannelHandler(event_loop, fd, localAddr, peerAddr));
	(static_cast<std::set<ChannelPtr>*>(channel_ptr->getEventLoop()->getMutableContext()))
		->insert(channel_ptr);
	DEBUG << "ChannelSet Size " << (static_cast<std::set<ChannelPtr>*>(channel_ptr
								->getEventLoop()->getMutableContext()))->size();
	DEBUG << "........................";
}



