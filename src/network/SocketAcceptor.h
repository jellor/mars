/*========================================================
 *
 *   文件名称 ：SocketAcceptor.h
 *   创建日期 ：2016.6.2
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __SOCKET_ACCEPTOR_H__
#define __SOCKET_ACCEPTOR_H__

#include "NonCopyable.h"
#include "Socket.h"
#include "EventLoop.h"
#include "Handler.h"
#include "IpAddress.h"

namespace mars {

class SocketAcceptor : public NonCopyable {

	typedef std::function<void(int fd, const IpAddress& local_address, const IpAddress& peer_address)> AcceptCallback;
	
public:
	SocketAcceptor(EventLoop* event_loop, const IpAddress& ip_address);
	~SocketAcceptor();

	void setAcceptCallback(const AcceptCallback& cb) { accept_callback_ =  cb; }
	int listen();
	void listenInEventLoop();
	
private:
	void onAccept();

	int null_fd_;
	EventLoop* event_loop_;
	IpAddress ip_address_;
	Socket socket_;
	Handler handler_;
	AcceptCallback accept_callback_;	
};

}

#endif