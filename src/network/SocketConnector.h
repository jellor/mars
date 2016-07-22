/*========================================================
 *
 *   文件名称 ：SocketConnector.h
 *   创建日期 ：2016.6.2
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __SOCKET_CONNECTOR_H__
#define __SOCKET_CONNECTOR_H__

#include "EventLoop.h"
#include "Socket.h"
#include "IpAddress.h"
#include "Handler.h"
#include "ChannelPool.h"

namespace mars {

class SocketConnector : public NonCopyable {
	typedef std::function<void(ChannelPtr)> ConnectCallback;
	typedef std::function<void(void)> ErrorCallback;
public:
	SocketConnector(EventLoop* event_loop, const IpAddress& peer_address);
	~SocketConnector();

	void connect();
	void setConnectCallback(const ConnectCallback& cb) { connect_callback_ = cb; }
	void setErrorCallback(const ErrorCallback& cb) { error_callback_ = cb; }

private:
	void run();
	void onWritable();
	void handleConnectCallback(ChannelPtr channel_ptr) { if (connect_callback_ != nullptr) connect_callback_(channel_ptr); }
	void handleErrorCallback() { if (error_callback_ != nullptr) error_callback_(); }

	EventLoop* event_loop_;
	IpAddress peer_address_;
	std::unique_ptr<Socket> socket_;
	std::unique_ptr<Handler> handler_; 
	ConnectCallback connect_callback_;
	ErrorCallback error_callback_;
};

}

#endif