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

using namespace mars;

SingleThreadEventLoop::SingleThreadEventLoop():
started(false),
thread_(),
channel_ptr_set_(nullptr),
event_loop_(nullptr)
{

}

SingleThreadEventLoop::~SingleThreadEventLoop() {
	event_loop_->stop();
}

void SingleThreadEventLoop::setThreadName(const std::string& name) {
	if (started) {
		return ;
	}
	thread_.setThreadName(name);
}

void SingleThreadEventLoop::start() {
	thread_.setThreadFunc(std::bind(&SingleThreadEventLoop::runInThread, this));
	thread_.start();
	while (event_loop_ == nullptr) {}
	started = true;
}

bool SingleThreadEventLoop::join() {
	return thread_.join();
}

EventLoop* SingleThreadEventLoop::getEventLoop() const {
	return event_loop_;
}

void SingleThreadEventLoop::runInThread() {
	channel_ptr_set_ = new std::set<ChannelPtr>();
	event_loop_      = new EventLoop();
	event_loop_->setContext(channel_ptr_set_);
	event_loop_->start();
	channel_ptr_set_->clear();
	delete channel_ptr_set_;
	delete event_loop_;
}