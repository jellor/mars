/*========================================================
 *
 *   文件名称 ：Bootstrap.h
 *   创建日期 ：2016.7.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __BOOTSTRAP_H__
#define __BOOTSTRAP_H__

#include "NonCopyable.h"
#include "IpAddress.h"
#include "Acceptor.h"
#include "Connector.h"
#include "ThreadPool.h"
#include "HandlerChain.h"
#include <vector>

namespace mars {

class Bootstrap : public NonCopyable {
public:
	Bootstrap(const std::vector<IpAddress*>& listen_address_list, const std::vector<IpAddress*>& connect_address_list, 
	int acceptor_count, int worker_count, int worker_thread_count);
	~Bootstrap();

	void start();
	void join();

	void addChain(HandlerChain* channel_chain);

	void setFilterCallback(const FilterCallback& cb) { filter_callback_  = cb; }
	void setConnectCallback(const EventCallback& cb) { connect_callback_ = cb; }
	void setReadCallback(const EventCallback& cb) 	 { read_callback_    = cb; }
	void setWriteCallback(const EventCallback& cb)   { write_callback_ = cb;   }
	void setCloseCallback(const EventCallback& cb)   { close_callback_ = cb;   }
	void setErrorCallback(const EventCallback& cb)   { error_callback_ = cb;   }

private:

	HandlerChain* activeHandlerChain(const ChannelPtr& channel_ptr) const;

	bool handleFilter(int fd, const IpAddress& local_address, const IpAddress& peer_address);
	void handleConnect(const ChannelPtr& channel_ptr);
	void handleRead(const ChannelPtr& channel_ptr);
	void handleClose(const ChannelPtr& channel_ptr);
	void handleWrite(const ChannelPtr& channel_ptr);
	void handleError(const ChannelPtr& channel_ptr);

	Acceptor*  acceptor_;
	Connector* connector_;
	ThreadPool threadPool_;

	FilterCallback filter_callback_;
	EventCallback connect_callback_;
	EventCallback read_callback_;
	EventCallback write_callback_;
	EventCallback close_callback_;
	EventCallback error_callback_;

	std::vector<HandlerChain*> chain_list_;
};


}

#endif