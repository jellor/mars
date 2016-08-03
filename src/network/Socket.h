/*========================================================
 *
 *   文件名称 ：Socket.h
 *   创建日期 ：2016.5.21
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "NonCopyable.h"
#include "IpAddress.h"

namespace mars {

class Socket {
public:
	Socket();
	Socket(int sockfd);
	~Socket();

	void reset() {
		close();
		sockfd_ = -1;
	}

	void reset(int sockfd) {
		close();
		sockfd_ = sockfd;
	}

	int fd() const { return sockfd_; }
	void setFd(int sockfd) { sockfd_ = sockfd; }
	bool available() const { return sockfd_ != -1 ? true : false; }
	
	int	bind(const IpAddress& IpAddress);
	int listen();
	int accept(IpAddress* IpAddress);
	int connect(const IpAddress& IpAddress);
	void close();

	void setNonBlock(bool on);
	void setNoDelay(bool on);
	void setReuseAddr(bool on);
	void setReusePort(bool on);
	void setKeepAlive(bool on);
	void setLinger(bool on, int time_out);
	
	void shutdownReceive();
	void shutdownSend();
	void shutdownBoth();

	static void closeSocket(int sockfd);
	static int getSocket();
	static int getError(int sockfd, int* error);
	static bool getLocalAddress(int sockfd, IpAddress* IpAddress);
	static bool getPeerAddress(int sockfd, IpAddress* IpAddress);
	static bool isSelfConnect(int sockfd);

private:
	int sockfd_;
};

}

#endif