/*========================================================
 *
 *   文件名称 ：Handler.h
 *   创建日期 ：2016.5.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __HANDLER_H__
#define __HANDLER_H__

#include "EventLoop.h"
#include "NonCopyable.h"
#include <functional>
 
namespace mars {

class EventLoop;

typedef enum {
	    NEW = 0,
	    ADD = 1,
	    DEL = 2,
	} HandlerStatus;

class Handler : public NonCopyable {

	typedef std::function<void()> EventCallback;

public:
	Handler(EventLoop* event_loop, int fd = -1);
	~Handler();

	void setReadCallback (const EventCallback& cb) { read_callback_  = cb; }
	void setWriteCallback(const EventCallback& cb) { write_callback_ = cb; }
	void setCloseCallback(const EventCallback& cb) { close_callback_ = cb; }
	void setErrorCallback(const EventCallback& cb) { error_callback_ = cb; }
	void setReadCallback (EventCallback&& cb) { read_callback_  = std::move(cb); }
	void setWriteCallback(EventCallback&& cb) { write_callback_ = std::move(cb); }
	void setCloseCallback(EventCallback&& cb) { close_callback_ = std::move(cb); }
	void setErrorCallback(EventCallback&& cb) { error_callback_ = std::move(cb); }

	bool isReadable() const { return readable_; }
	bool isWritable() const { return writable_; }
	void enableRead()   { if (readable_) return ; readable_ = true;  update(); }
	void disableRead()  { if (!readable_)return ; readable_ = false; update(); }
	void enableWrite()  { if (writable_) return ; writable_ = true;  update(); }
	void disableWrite() { if (!writable_)return ; writable_ = false; update(); }
	

	void handleReadEvent()  { if (read_callback_  != nullptr) read_callback_();  }
	void handleWriteEvent() { if (write_callback_ != nullptr) write_callback_(); }
	void handleCloseEvent() { if (close_callback_ != nullptr) close_callback_(); }
	void handleErrorEvent() { if (error_callback_ != nullptr) error_callback_(); }

	void reset();
	void reset(EventLoop* event_loop, int fd = -1);
	void remove();
	int fd() const { return fd_; }
	void setStatus(HandlerStatus status) { status_ = status; }
	HandlerStatus getStatus() const { return status_; }

	void setEventLoop(EventLoop* event_loop) { event_loop_ = event_loop; }
	
private:
	void update();

	EventLoop* event_loop_;
	int fd_;
	HandlerStatus status_;
	bool readable_;
	bool writable_;

	EventCallback read_callback_;
	EventCallback write_callback_;
	EventCallback close_callback_;
	EventCallback error_callback_;
};

}

#endif
