/*========================================================
 *
 *   文件名称 ：EventLoop.cpp
 *   创建日期 ：2016.5.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "EventLoop.h"
#include "Selector.h"

using namespace mars;

EventLoop::EventLoop():
selector_(Selector::newSelector()),
started_(false) 
{

}

EventLoop::~EventLoop() {

}

void EventLoop::start() {
	if (started_) {

		return;
	}
	started_ = true;
	loop();
}

void EventLoop::stop() {
	started_ = false;
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

void EventLoop::loop() {
	while (started_) {
		selector_->select(1024);
	}
}











