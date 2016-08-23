/*========================================================
 *
 *   文件名称 ：SingleThreadEventLoop.cpp
 *   创建日期 ：2016.6.19
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "SingleThreadEventLoop.h"
#include "Log.h"
#include "ChannelPool.h"

using namespace mars;

SingleThreadEventLoop::SingleThreadEventLoop():
started_(false),
thread_(),
event_loop_(nullptr)
{

}

SingleThreadEventLoop::~SingleThreadEventLoop() {
	event_loop_->stop();
}

void SingleThreadEventLoop::setThreadName(const std::string& name) {
	if (started_) {
		return ;
	}
	thread_.setThreadName(name);
}

void SingleThreadEventLoop::start() {
	thread_.setThreadFunc(std::bind(&SingleThreadEventLoop::runInThread, this));
	thread_.start();
	while (started_ == false) {} // ? lock
	//assert(started_ == true);
}

bool SingleThreadEventLoop::join() {
	return thread_.join();
}

EventLoop* SingleThreadEventLoop::getEventLoop() const {
	return event_loop_;
}

void SingleThreadEventLoop::runInThread() {

	event_loop_      			= new EventLoop();
	ChannelPool* channel_pool	= new ChannelPool(event_loop_, 10000);
	event_loop_->setContext(channel_pool);

	started_ = true;

	event_loop_->start();

	delete event_loop_;
	delete channel_pool;
	event_loop_		 = nullptr;
	channel_pool	 = nullptr;
	started_ 		 = false;
}

