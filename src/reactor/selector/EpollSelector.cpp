/*========================================================
 *
 *   文件名称 ：EpollSelector.cpp
 *   创建日期 ：2016.5.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifdef __linux__
#include "EpollSelector.h"
#include "Log.h"
#include <unistd.h>

using namespace mars;

EpollSelector::EpollSelector():
epfd_(epoll_create(1024)),
handler_set_(),
active_event_list_(64)
{

}

EpollSelector::~EpollSelector() {
	close(epfd_);
}

void EpollSelector::dispatch(int64_t timeout_usec) {
	int active_count = 0;
	active_count = epoll_wait(epfd_, &*active_event_list_.begin(), active_event_list_.size(), timeout_usec / 1000);
	if (active_count < 0) {
		WARN << "Epoll Wait Failed";
	}
	for (int i = 0; i < active_count; i ++) {
		Handler* active_handler = (Handler*) active_event_list_[i].data.ptr;
		struct epoll_event& active_event = active_event_list_[i];
		if (active_event.events & EPOLLERR) {
			DEBUG << "EPOLLERR";
			active_handler->handleErrorEvent();
		}
		if (active_event.events & EPOLLHUP) {
			DEBUG << "EPOLLHUP";
			active_handler->handleCloseEvent();
		}
		if (active_event.events & EPOLLRDHUP) {
			DEBUG << "EPOLLRDHUP";
			active_handler->handleReadEvent();
		}
		if (active_event.events & EPOLLPRI) {
			DEBUG << "EPOLLPRI";
		}
		if (active_event.events & EPOLLIN) {
			DEBUG << "EPOLLIN";
			active_handler->handleReadEvent();
		}
		if (active_event.events & EPOLLOUT) {
			DEBUG << "EPOLLOUT";
			active_handler->handleWriteEvent();
		}
	}
	if (active_count == active_event_list_.size()) {
		active_event_list_.resize(active_event_list_.size() * 2);
	}
}

void EpollSelector::addHandler(Handler* handler) {
	struct epoll_event ev;
	ev.data.ptr = handler;
	ev.events   = EPOLLPRI | EPOLLRDHUP;
	if (handler->isWritable()) {
		ev.events |= EPOLLOUT;
	}
	if (handler->isReadable()) {
		ev.events |= EPOLLIN;
	}
	if (epoll_ctl(epfd_, EPOLL_CTL_ADD, handler->fd(), &ev) != 0) {
		WARN << "Cannot Add Event To Epoll";
		WARN << "errno => " << errno;
	}
	handler_set_.insert(handler);
	handler->setStatus(ADD);
	DEBUG << "AddHandler";
}

void EpollSelector::updateHandler(Handler* handler) {
	struct epoll_event ev;
	ev.data.ptr = handler;
	ev.events   = EPOLLPRI | EPOLLRDHUP;
	if (handler->isWritable()) {
		ev.events |= EPOLLOUT;
	}
	if (handler->isReadable()) {
		ev.events |= EPOLLIN;
	}
	if (epoll_ctl(epfd_, EPOLL_CTL_MOD, handler->fd(), &ev) != 0) {
		WARN << "Cannot Mod Event To Epoll";
		WARN << "errno => " << errno;
	}
	DEBUG << "UpdateHandler";
}

void EpollSelector::removeHandler(Handler* handler) {
	if (epoll_ctl(epfd_, EPOLL_CTL_DEL, handler->fd(), NULL) != 0) {
		WARN << "Cannot DEL Event To Epoll";
		WARN << "errno => " << errno;
	}
	handler_set_.erase(handler);
	handler->setStatus(DEL);
	DEBUG << "RemoveHandler";
}

#endif // __linux__
