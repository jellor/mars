/*========================================================
 *
 *   文件名称 ：Acceptor.h
 *   创建日期 ：2016.6.2
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__

#include "NonCopyable.h"
#include "SocketAcceptor.h"
#include "EventLoop.h"
#include "EventLoopGroup.h"
#include "Handler.h"
#include "IpAddress.h"
#include "Channel.h"
#include <vector>

namespace mars {

typedef std::function<bool(int fd, const IpAddress& local_address, const IpAddress& peer_address)> FilterCallback;
typedef std::function<void(const ChannelPtr&)> EventCallback;

class Acceptor : public NonCopyable {
public:
	Acceptor(const std::vector<IpAddress*>& ip_address_list, int acceptor_count, int worker_count);
	~Acceptor();

	void start();
	bool join();
	int getThreadCount();

	void setFilterCallback(const FilterCallback& cb) { filter_callback_  = cb; }
	void setReadCallback(const EventCallback& cb) 	 { read_callback_    = cb; }
	void setWriteCallback(const EventCallback& cb)   { write_callback_ = cb;   }
	void setCloseCallback(const EventCallback& cb)   { close_callback_ = cb;   }
	void setErrorCallback(const EventCallback& cb)   { error_callback_ = cb;   }

private:
	
	void handleRead(const ChannelPtr& channel_ptr)  {
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
	
	void handleAcceptEvent(int fd, const IpAddress& local_address, const IpAddress& peer_address);

	EventLoopGroup acceptor_group_;
	EventLoopGroup worker_group_;
	std::vector<IpAddress*> ip_address_list_;
	std::vector<SocketAcceptor*> socket_acceptor_list_;

	FilterCallback filter_callback_;
	EventCallback read_callback_;
	EventCallback write_callback_;
	EventCallback close_callback_;
	EventCallback error_callback_;
};

}

#endif