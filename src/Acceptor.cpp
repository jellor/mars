/*========================================================
 *
 *   文件名称 ：Acceptor.cpp
 *   创建日期 ：2016.6.2
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "Acceptor.h"
#include "Log.h"
#include "ChannelPool.h"
#include <unistd.h> 
#include <fcntl.h>
#include <errno.h>

using namespace mars;

Acceptor::Acceptor(const std::vector<IpAddress*>& ip_address_list, int acceptor_count, int worker_count):
acceptor_group_(acceptor_count),
worker_group_(worker_count),
ip_address_list_(ip_address_list),
socket_acceptor_list_(),
filter_callback_(nullptr),
connect_callback_(nullptr),
read_callback_(nullptr),
write_callback_(nullptr),
close_callback_(nullptr),
error_callback_(nullptr)
{
}

Acceptor::~Acceptor() {
	DEBUG << "Acceptor Desstruct ...";
	join();
	for (int i = 0; i < socket_acceptor_list_.size(); i ++) {
		delete socket_acceptor_list_[i];
	}
	socket_acceptor_list_.clear();	
	DEBUG << "Acceptor Destructor Ok";
}

void Acceptor::start() {
	acceptor_group_.start();
	worker_group_.start();
	for (int i = 0; i < ip_address_list_.size(); i ++) {
		SocketAcceptor* element = new SocketAcceptor(acceptor_group_.next(), *ip_address_list_[i]);
		element->setAcceptCallback(
			std::bind(&Acceptor::handleAcceptEvent, this, 
				std::placeholders::_1,
				std::placeholders::_2, 
				std::placeholders::_3));
		element->listen();
		socket_acceptor_list_.push_back(element);
	}
}

bool Acceptor::join() {
	return acceptor_group_.join() && worker_group_.join();
}

int Acceptor::getThreadCount() {
	return acceptor_group_.size() + worker_group_.size();
}

void Acceptor::handleAcceptEvent(int fd, const IpAddress& local_address, const IpAddress& peer_address) {
	if (filter_callback_ != nullptr) {
		if (!filter_callback_(fd, local_address, peer_address)) {
			Socket::closeSocket(fd);
			DEBUG << "Peer Address => " << peer_address.toString() << " Are Refused";
			return;
		}
	}

	DEBUG << "Fd => " << fd << " Local Address => " << local_address.toString() << " Peer Address => " << peer_address.toString();

	ChannelPtr channel_ptr;

	EventLoop* event_loop = nullptr;

	if (worker_group_.size() > 0) {
		// worker_group_.push() 
		event_loop = worker_group_.next(); 
	} else {
		// acceptor_group_.push()
		event_loop = acceptor_group_.next();
	}

	if (event_loop == nullptr) {
		DEBUG << "Event Loop Should Not Nullptr";
		return ;
	}

	event_loop->doFunc(std::bind(&Acceptor::runInEventLoop, this, event_loop, fd, local_address, peer_address));
	
}

void Acceptor::runInEventLoop(EventLoop* event_loop, int fd, const IpAddress& local_address, const IpAddress& peer_address) {

	//ChannelPtr channel_ptr(new Channel(event_loop, fd, local_address, peer_address));
	ChannelPtr channel_ptr = (static_cast<ChannelPool*> (event_loop->getMutableContext()))
								->acquire(event_loop, fd, local_address, peer_address);

	handleConnect(channel_ptr);
	channel_ptr->setReadCallback(std::bind(&Acceptor::handleRead, this, std::placeholders::_1));
	channel_ptr->setCloseCallback(std::bind(&Acceptor::handleClose, this, std::placeholders::_1));
	channel_ptr->setWriteCallback(std::bind(&Acceptor::handleWrite, this, std::placeholders::_1));
	channel_ptr->setErrorCallback(std::bind(&Acceptor::handleError, this, std::placeholders::_1));

	channel_ptr->getHandler()->enableRead();
}



