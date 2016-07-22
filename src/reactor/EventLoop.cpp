/*========================================================
 *
 *   文件名称 ：EventLoop.cpp
 *   创建日期 ：2016.5.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "EventLoop.h"
#include "Timestamp.h"
#include "Log.h"
#include "Handler.h"
#include <functional>
#include <unistd.h>

using namespace mars;

EventLoop::EventLoop():
started_(false),
thread_id_(Thread::getCurrentThreadId()),
context_(nullptr),
selector_(Selector::newSelector()),
funcs_(),
calling_funcs_(false),
heap_(new MinHeap<Timer>()),
read_handler_(nullptr),
write_handler_(nullptr)
{

}

EventLoop::~EventLoop() {
	if (read_handler_ != nullptr) {
        delete read_handler_;
    }
    if (write_handler_ != nullptr) {
        delete write_handler_;
    }
}

void EventLoop::start() {
	if (started_) {

		return;
	}

	int fds[2];
	pipe(fds);
	read_handler_  = new Handler(this, fds[0]);
	write_handler_ = new Handler(this, fds[1]);
	read_handler_->enableRead();
	read_handler_->setReadCallback(std::bind(&EventLoop::consume, this));

	started_ = true;	
	loop();
}

void EventLoop::stop() {
	started_ = false;
	if (!isInSelfThread()) {
		wakeup();
	}
}

void EventLoop::loop() {
	int64_t timeout;
	int64_t now;
	int64_t nextTimeout;
	while (started_) {
		timeout = 0;
		now = Timestamp::now().macrosecond();
		// ? not use lock
		{
			Lock lock(mutex_);
			if (!heap_->empty()) {
				nextTimeout = (heap_->top())->timeout();
				timeout = nextTimeout > now ? nextTimeout - now : 0; 
			}
		}
		selector_->dispatch(timeout);
		doTimeout(Timestamp::now().macrosecond());
		doFuncs();
	}
}


void EventLoop::doTimeout(int64_t now) {
	int64_t nextTimeout;
	Timer* curTimer;	
	while (!heap_->empty()) {
		nextTimeout = (heap_->top())->timeout();
		if (nextTimeout > now) break;
		curTimer = heap_->pop();
		curTimer->run();
		if (curTimer->isValid()) {
			doFunc(std::bind(&EventLoop::pushTimer, this, curTimer));
		} 
	}
}

void EventLoop::doFuncs() {
	calling_funcs_ = true;
	std::vector<Func> funcs;
	{
		Lock lock(mutex_);
		funcs_.swap(funcs);
	}
	for (size_t i = 0; i < funcs.size(); i ++) {
		funcs[i]();
	}
	calling_funcs_ = false;
}

TimerId EventLoop::addTimer(const TimerCallback& cb, int64_t timeout, double interval, int counter) {
	Timer* timer = new Timer(cb, timeout, interval, counter);
	doFunc(std::bind(&EventLoop::pushTimer, this, timer));
	return TimerId(timer, timer->getSequence());
}

void EventLoop::removeTimer(const TimerId& timerId) {
	Timer* timer = timerId.getTimer();
	doFunc(std::bind(&EventLoop::eraseTimer, this, timer));
}

void EventLoop::pushTimer(Timer* timer) {
	heap_->push(timer);
}

void EventLoop::eraseTimer(Timer* timer) {
	heap_->erase(timer);
	delete timer;
}

void EventLoop::doFunc(const Func& func) {
	if (isInSelfThread()) {
		func();
	} else {
		pushFunc(func);
	}
}

void EventLoop::pushFunc(const Func& func) {
	{
		Lock lock(mutex_);
		funcs_.push_back(func);
	}
	if (!isInSelfThread() || calling_funcs_) {
		wakeup();
	}
}

void EventLoop::wakeup() {
	int value = 1;
    write(write_handler_->fd(), &value, sizeof(value));
    DEBUG << "Write Wakeup";
}

void EventLoop::consume() {
    int value;
    read(read_handler_->fd(), &value, sizeof(value));
    DEBUG << "Read Wakeup";
}

void EventLoop::addHandler(Handler* handler) { 
	selector_->addHandler(handler);    
}

void EventLoop::updateHandler(Handler* handler) {
	selector_->updateHandler(handler); 
}

void EventLoop::removeHandler(Handler* handler) { 
	selector_->removeHandler(handler); 
}


















