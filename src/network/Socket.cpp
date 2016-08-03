/*========================================================
 *
 *   文件名称 ：Socket.cpp
 *   创建日期 ：2016.5.21
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "Socket.h"
#include "Log.h"
#include <errno.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <assert.h>

using namespace mars;

Socket::Socket():
sockfd_(socket(AF_INET, SOCK_STREAM, 0))
{
	assert(sockfd_ > 0);
	
	setNonBlock(true);
	int flags = fcntl(sockfd_, F_GETFD, 0);
	flags |= FD_CLOEXEC;
	if (fcntl(sockfd_, F_SETFD, flags) < 0) {

	}
}

Socket::Socket(int sockfd):
sockfd_(sockfd)
{
	setNonBlock(true);
	int flags = fcntl(sockfd_, F_GETFD, 0);
	flags |= FD_CLOEXEC;
	if (fcntl(sockfd_, F_SETFD, flags) < 0) {

	}
}

Socket::~Socket() {
	DEBUG << "Socket Fd -> " << sockfd_ << " Closing ...";
	if (sockfd_ != -1) {
		::close(sockfd_);
	}
}

int Socket::bind(const IpAddress& IpAddress) {
	int ret = ::bind(sockfd_, reinterpret_cast<const struct sockaddr*>(&IpAddress.getAddr()), sizeof(struct sockaddr_in));
	return ret;
}

int Socket::listen() {
	int ret = ::listen(sockfd_, 1024);
	return ret;
}

int Socket::accept(IpAddress* IpAddress) {
	struct sockaddr_in addr;
	socklen_t len = static_cast<socklen_t>(sizeof(struct sockaddr_in));
	int ret = ::accept(sockfd_, (struct sockaddr*)&addr, (socklen_t*)&len);
	if (ret >= 0) {
		IpAddress->setAddr(addr);
	}
	return ret;
}

int Socket::connect(const IpAddress& IpAddress) {
	socklen_t len = static_cast<socklen_t>(sizeof(struct sockaddr_in));
	int ret = ::connect(sockfd_, reinterpret_cast<const struct sockaddr*>(&IpAddress.getAddr()), len);
	return ret;
}

void Socket::close() {
	if (sockfd_ != -1) {
		::close(sockfd_);
		sockfd_ = -1;
	}
}

void Socket::setNonBlock(bool on) {
	int flags = fcntl(sockfd_, F_GETFL, 0);
	if (on) {
		flags |= O_NONBLOCK;
	} else {
		flags &= ~O_NONBLOCK;
	}
	if (fcntl(sockfd_, F_SETFL, flags) < 0) {

	}
}

void Socket::setNoDelay(bool on) {
	int optval = on ? 1 : 0;
	int ret = setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char*>(&optval), sizeof(optval));
	if (ret < 0) {

	}
}

void Socket::setReuseAddr(bool on) {
	int optval = on ? 1 : 0;
	int ret = setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&optval), sizeof(optval));
	if (ret < 0) {

	}
}

void Socket::setReusePort(bool on) {
#ifdef SO_REUSEPORT
	int optval = on ? 1 : 0;
	int ret = setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT, reinterpret_cast<char*>(&optval), sizeof(optval));
	if (ret < 0) {

	}
#else
#error "Platform Not Supported"
#endif
}

void Socket::setKeepAlive(bool on) {
	int optval = on ? 1 : 0;
	int ret = setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE, reinterpret_cast<char*>(&optval), sizeof(optval));
}

void Socket::setLinger(bool on, int time_out) {

}

void Socket::shutdownReceive() {
	::shutdown(sockfd_, SHUT_RD);
}

void Socket::shutdownSend() {
	::shutdown(sockfd_, SHUT_WR);
}

void Socket::shutdownBoth() {
	::shutdown(sockfd_, SHUT_RDWR);
}

void Socket::closeSocket(int sockfd) {
	::close(sockfd);
}

int Socket::getSocket() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	int optval = 1;
	int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));
	if (ret < 0) {

	}
	int flags = fcntl(sockfd, F_GETFD, 0);
	flags |= FD_CLOEXEC;
	if (fcntl(sockfd, F_SETFD, flags) < 0) {

	}
	return sockfd;
}

int Socket::getError(int sockfd, int* error) {
	socklen_t len = static_cast<socklen_t>(sizeof(error));
	return getsockopt(sockfd, SOL_SOCKET, SO_ERROR, error, &len);
}


bool Socket::getLocalAddress(int sockfd, IpAddress* ip_address) {
	struct sockaddr_in addr;
	socklen_t address_len = static_cast<socklen_t>(sizeof(struct sockaddr_in));
	int ret = getsockname(sockfd, reinterpret_cast<struct sockaddr*>(&addr), &address_len);
	if (ret == 0) {
		ip_address->setAddr(addr);
		return true;
	} else {
		return false;
	}
}

bool Socket::getPeerAddress(int sockfd, IpAddress* ip_address) {
	struct sockaddr_in addr;
	socklen_t address_len = static_cast<socklen_t>(sizeof(struct sockaddr_in));
	int ret = getpeername(sockfd, reinterpret_cast<struct sockaddr*>(&addr), &address_len);
	if (ret == 0) {
		ip_address->setAddr(addr);
		return true;
	} else {
		return false;
	}
}

bool Socket::isSelfConnect(int sockfd) {
	IpAddress local_address;
	IpAddress peer_address;
	if (getLocalAddress(sockfd, &local_address) && getPeerAddress(sockfd, &peer_address)) {
		if (local_address == peer_address) {
			return true;
		} else {
			return false;
		}
	}
	return false;
}












