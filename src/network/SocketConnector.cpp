/*========================================================
 *
 *   文件名称 ：SocketConnector.cpp
 *   创建日期 ：2016.6.2
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *   描 述 ：
 *
 *=======================================================*/

#include "SocketConnector.h"
#include "Log.h"
#include <errno.h>

using namespace mars;

SocketConnector::SocketConnector(EventLoop* event_loop, const IpAddress& peer_address):
event_loop_(event_loop),
peer_address_(peer_address),
socket_(new Socket()),
handler_(new Handler(event_loop_, socket_->fd())),
connect_callback_(nullptr),
error_callback_(nullptr)
{
	socket_->setNoDelay(true);
}

SocketConnector::~SocketConnector() {
	
}

void SocketConnector::connect() {
	event_loop_->doFunc(std::bind(&SocketConnector::run, this));
}

void SocketConnector::run() {
	int ret = socket_->connect(peer_address_);
	int err = errno;
	if (ret == -1) {
		if (err == EINPROGRESS || err == EINTR) {
			handler_->setWriteCallback(std::bind(&SocketConnector::onWritable, this));
			handler_->enableWrite();
		} else {
			handleErrorCallback();
		}
	} 
	if (ret == 0) {
		handler_->disableWrite();
		IpAddress local_address;
		DEBUG << "Fd => " << socket_->fd();
		DEBUG << "Local Address " << local_address.toString();
		DEBUG << "Peer  Address " << peer_address_.toString();
		Socket::getPeerAddress(socket_->fd(), &local_address);
		ChannelPtr channelPtr(new Channel(event_loop_, socket_->fd(), local_address, peer_address_));
		handleConnectCallback(channelPtr);
	}
}

void SocketConnector::onWritable() {
	IpAddress peer_address;
	int error;
	if (Socket::getPeerAddress(socket_->fd(), &peer_address)) {
		handler_->disableWrite();
		IpAddress local_address;
		Socket::getLocalAddress(socket_->fd(), &local_address);
		DEBUG << "Fd => " << socket_->fd();
		DEBUG << "Local Address " << local_address.toString();
		DEBUG << "Peer  Address " << peer_address.toString();

		if (!Socket::isSelfConnect(socket_->fd())) {
			ChannelPtr channel_ptr(new Channel(event_loop_, socket_->fd(), local_address, peer_address));
			handleConnectCallback(channel_ptr);
		} else {
			int error;
			if (Socket::getError(socket_->fd(), &error) != 0 || error != 0) {
				WARN << Log::getError();
			}
			WARN << "Connect To Self";
		}
	} else {
		DEBUG << "Fd => " << socket_->fd();
		DEBUG << "Peer  Address " << peer_address.toString();
		handleErrorCallback();
		handler_->remove();
	}
}






