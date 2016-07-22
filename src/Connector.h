/*========================================================
 *
 *   文件名称 ：Connector.h
 *   创建日期 ：2016.6.2
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __CONNECTOR_H__
#define __CONNECTOR_H__

#include "EventLoop.h"
#include "IpAddress.h"
#include "Thread.h"
#include "SocketConnector.h"
#include <set>


namespace mars {

class Connector : public NonCopyable {
	typedef std::function<void(ChannelPtr)> EventCallback;
public:
	Connector(const std::vector<IpAddress*>& ip_address_list);
	~Connector();

	void start();
	void join();
	bool started() const { return started_; }
	void setConnectCallback(const EventCallback& cb) { connect_callback_ = cb; }
	void setReadCallback(const EventCallback& cb) 	 { read_callback_    = cb; }
	void setWriteCallback(const EventCallback& cb)   { write_callback_ = cb;   }
	void setCloseCallback(const EventCallback& cb)   { close_callback_ = cb;   }
	void setErrorCallback(const EventCallback& cb)   { error_callback_ = cb;   }

private:
	void handleConnect(const ChannelPtr& channel_ptr) {
		if (connect_callback_ != nullptr) connect_callback_(channel_ptr);
	}

	void handleRead(const ChannelPtr& channel_ptr) {
		if (read_callback_ != nullptr) read_callback_(channel_ptr);
	}

	void handleClose(const ChannelPtr& channel_ptr) {
		if (close_callback_ != nullptr) close_callback_(channel_ptr);
	}

	void handleWrite(const ChannelPtr& channel_ptr) {
		if (write_callback_ != nullptr) write_callback_(channel_ptr);
	}

	void handleError(const ChannelPtr& channel_ptr) {
		if (error_callback_ != nullptr) error_callback_(channel_ptr);
	}
	void runInThread();
	void handleConnectEvent(ChannelPtr channel_ptr);

	std::atomic<bool> started_;
	EventLoop* event_loop_;
	std::vector<IpAddress*> ip_address_list_;
	std::vector<SocketConnector*> socket_connector_list_;
	std::set<ChannelPtr> channel_ptr_set_;
	Thread thread_;
	EventCallback connect_callback_;
	EventCallback read_callback_;
	EventCallback write_callback_;
	EventCallback close_callback_;
	EventCallback error_callback_;
};

}

#endif