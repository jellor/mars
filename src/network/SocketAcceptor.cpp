/*========================================================
 *
 *   文件名称 ：SocketAcceptor.cpp
 *   创建日期 ：2016.6.2
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "SocketAcceptor.h"
#include "Log.h"
#include <unistd.h> 
#include <fcntl.h>
#include <errno.h>

using namespace mars;

void defaultAcceptFunc(int fd, const IpAddress& local_address, const IpAddress& peer_address) {
	WARN << "Should Set AcceptCallback In SocketAccepter";
	WARN << "Fd => " << fd << " Local Address => " << local_address.toString() << " Peer Address => " << peer_address.toString();
}

SocketAcceptor::SocketAcceptor(EventLoop* event_loop, const IpAddress& ip_address):
event_loop_(event_loop),
ip_address_(ip_address),
socket_(),
handler_(event_loop, socket_.fd()),
null_fd_(open("/dev/null", O_CLOEXEC | O_WRONLY)),
accept_callback_(defaultAcceptFunc)
{
	socket_.setNoDelay(true);
	socket_.setReusePort(true);
	socket_.setReuseAddr(true);
}

SocketAcceptor::~SocketAcceptor() {
	close(null_fd_);
	handler_.remove();
}

int SocketAcceptor::listen() {
	event_loop_->doFunc(std::bind(&SocketAcceptor::listenInEventLoop, this));// if directly invoke listenInLoop() will happen what.
	return 0;
}

void SocketAcceptor::listenInEventLoop() {
    handler_.setReadCallback(std::bind(&SocketAcceptor::onAccept, this));
	handler_.enableRead();
	socket_.bind(ip_address_);
	socket_.listen();
}

void SocketAcceptor::onAccept() {
	IpAddress peer_address; // ? object in stack
	int fd = socket_.accept(&peer_address);
	DEBUG << "fd == > " << fd;
	if (fd != -1) {
		accept_callback_(fd, ip_address_, peer_address);
	} else {
		if (errno == ENOMEM) {
			close(null_fd_);
			fd = ::accept(socket_.fd(), 0, 0);
			close(fd);
			null_fd_ = open("/dev/null", O_CLOEXEC | O_WRONLY);
		}		
	}
}


