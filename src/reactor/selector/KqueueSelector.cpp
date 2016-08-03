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
#include "Log.h"
#include <unistd.h>

using namespace mars;

KqueueSelector::KqueueSelector():
kqfd_(kqueue()),
handler_set_(),
active_event_list_(64)
{

}

KqueueSelector::~KqueueSelector() {
	close(kqfd_);
}

void KqueueSelector::dispatch(int64_t timeout_usec) {
	int active_count = 0;

	if (timeout_usec <= 0) {
		active_count = kevent(kqfd_, NULL, 0, &*active_event_list_.begin(), active_event_list_.size(), NULL);
	} else {
		struct timespec timeout;
		timeout.tv_sec  = timeout_usec / 1000000;
		timeout.tv_nsec = (timeout_usec % 1000000) * 1000;
		active_count = kevent(kqfd_, NULL, 0, &*active_event_list_.begin(), active_event_list_.size(), &timeout);
	}

	if (active_count < 0) {
		WARN << "Epoll Wait Failed";
		WARN << "errno => " << errno;
		WARN << "errno => " << Log::getError();
	}

	for (int i = 0; i < active_count; i ++) {
		struct kevent& active_event = active_event_list_[i];
		Handler* active_handler = (Handler*) active_event.udata;

		DEBUG << "errno => " << errno;
		DEBUG << "errno => " << Log::getError();
		DEBUG << "fd    => " << active_handler->fd();
		DEBUG << "status=> " << active_handler->getStatus();

		if (active_event.flags & EV_ERROR) {
			DEBUG << "EV_ERROR";
			active_handler->handleErrorEvent();
		}
		if (active_event.flags & EV_EOF) {
			DEBUG << "EV_EOF";
			//active_handler->handleCloseEvent();
		}
		if (active_event.filter == EVFILT_READ) {
			DEBUG << "EVFILT_READ";
			active_handler->handleReadEvent();
		}
		if (active_event.filter == EVFILT_WRITE) {
			DEBUG << "EVFILT_WRITE";
			active_handler->handleWriteEvent();
		}
	}

	if (active_count == active_event_list_.size()) {
		active_event_list_.resize(active_event_list_.size() * 2);
	}
}

void KqueueSelector::addHandler(Handler* handler) {
	DEBUG << "errno => " << errno;
	DEBUG << "errno => " << Log::getError();
	DEBUG << "fd => " << handler->fd();
	DEBUG << "status=> " << handler->getStatus();
	DEBUG << "readable " << handler->isReadable();
	DEBUG << "writable " << handler->isWritable();
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
	DEBUG << "errno => " << errno;
	DEBUG << "errno => " << Log::getError();
	DEBUG << "fd => " << handler->fd();
	DEBUG << "status=> " << handler->getStatus();
	DEBUG << "readable " << handler->isReadable();
	DEBUG << "writable " << handler->isWritable();
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
	DEBUG << "errno => " << errno;
	DEBUG << "errno => " << Log::getError();
	DEBUG << "fd => " << handler->fd();
	DEBUG << "status=> " << handler->getStatus();
	DEBUG << "readable " << handler->isReadable();
	DEBUG << "writable " << handler->isWritable();

	struct kevent event[2];

	EV_SET(&event[0], handler->fd(), EVFILT_READ, EV_DELETE, 0, 0, handler);
	kevent(kqfd_, &event[0], 1, NULL, 0, NULL);

	
	DEBUG << "errno => " << errno;
	DEBUG << "errno => " << Log::getError();
	DEBUG << "fd => " << handler->fd();
	DEBUG << "status=> " << handler->getStatus();
	DEBUG << "readable " << handler->isReadable();
	DEBUG << "writable " << handler->isWritable();

	EV_SET(&event[1], handler->fd(), EVFILT_WRITE, EV_DELETE, 0, 0, handler);
	kevent(kqfd_, &event[1], 1, NULL, 0, NULL);

	handler_set_.erase(handler);
	handler->setStatus(DEL);
	DEBUG << "errno => " << errno;
	DEBUG << "errno => " << Log::getError();
	DEBUG << "fd => " << handler->fd();
	DEBUG << "status=> " << handler->getStatus();
}

#endif // __APPLE__