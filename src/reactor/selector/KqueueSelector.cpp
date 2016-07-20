/*========================================================
 *
 *   文件名称 ：KqueueSelector.cpp
 *   创建日期 ：2016.5.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifdef __APPLE__
#include "KqueueSelector.h"
#include <unistd.h>

using namespace starnet;

KqueueSelector::KqueueSelector():
kqfd_(kqueue()),
handler_set_(),
active_event_list_(64)
{

}

KqueueSelector::~KqueueSelector() {
	close(kqfd_);
}

void KqueueSelector::select(int timeout_ms) {
	int active_count = 0;

	struct timespec timeout;
	timeout.tv_sec = timeout_ms / 1000;
	timeout.tv_nsec = (timeout_ms % 1000) * 1000000;

	active_count = kevent(kqfd_, NULL, 0, &*active_event_list_.begin(), active_event_list_.size(), &timeout);

	for (int i = 0; i < active_count; i ++) {
		struct kevent& active_event = active_event_list_[i];
		Handler* active_handler = (Handler*) active_event.udata;

		if (active_event.flags & EV_ERROR) {

		}
		if (active_event.flags & EV_EOF) {

		}
		if (active_event.filter == EVFILT_READ) {
			active_handler->handleReadEvent();
		}
		if (active_event.filter == EVFILT_WRITE) {
			active_handler->handleWriteEvent();
		}
	}

	if (active_count == active_event_list_.size()) {
		active_event_list_.resize(active_event_list_.size() * 2);
	}
}

void KqueueSelector::addHandler(Handler* handler) {
	struct kevent event[2];
	if (handler->isReadable()) {
		EV_SET(&event[0], handler->fd(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, handler);	
	} else {
		EV_SET(&event[0], handler->fd(), EVFILT_READ, EV_ADD | EV_DISABLE, 0, 0, handler);
	}
	if (handler->isWritable()) {
		EV_SET(&event[1], handler->fd(), EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, handler);
	} else {
		EV_SET(&event[1], handler->fd(), EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, handler);
	}
	kevent(kqfd_, event, 2, NULL, 0, NULL);
	handler_set_.insert(handler);
	handler->setStatus(ADD);
}

void KqueueSelector::updateHandler(Handler* handler) {
	struct kevent event[2];
	if (handler->isReadable()) {
		EV_SET(&event[0], handler->fd(), EVFILT_READ, EV_ENABLE, 0, 0, handler);	
	} else {
		EV_SET(&event[0], handler->fd(), EVFILT_READ, EV_DISABLE, 0, 0, handler);
	}
	if (handler->isWritable()) {
		EV_SET(&event[1], handler->fd(), EVFILT_WRITE, EV_ENABLE, 0, 0, handler);
	} else {
		EV_SET(&event[1], handler->fd(), EVFILT_WRITE, EV_DISABLE, 0, 0, handler);
	}
	kevent(kqfd_, event, 2, NULL, 0, NULL);
}

void KqueueSelector::removeHandler(Handler* handler) {
	struct kevent event[2];
	EV_SET(&event[0], handler->fd(), EVFILT_READ, EV_DELETE, 0, 0, handler);
	EV_SET(&event[1], handler->fd(), EVFILT_WRITE, EV_DELETE, 0, 0, handler);
	kevent(kqfd_, event, 2, NULL, 0, NULL);
	handler_set_.erase(handler);
	handler->setStatus(DEL);
}

#endif // __APPLE__