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
#include "Timestamp.h"
#include <errno.h>

using namespace mars;

SocketConnector::SocketConnector(EventLoop* event_loop, const IpAddress& peer_address):
event_loop_(event_loop),
peer_address_(peer_address),
socket_(),
handler_(event_loop_, socket_.fd()),
connect_callback_(nullptr),
error_callback_(nullptr)
{
	socket_.setNoDelay(true);
}

SocketConnector::~SocketConnector() {
	
}

void SocketConnector::connect() {
	event_loop_->doFunc(std::bind(&SocketConnector::run, this));
}

void SocketConnector::reconnect() {
	handler_.reset();
	socket_.close();
	handler_.reset(event_loop_, socket_.fd());
	socket_.setNoDelay(true);
	connect();
}

void SocketConnector::run() {

	int ret = socket_.connect(peer_address_);

	int err = errno;
	if (ret == -1) {
		int sock_err;

		DEBUG << "errno " << Log::getError();

		if (err == EINPROGRESS || err == EINTR) {
			errno = 0;
			handler_.setWriteCallback(std::bind(&SocketConnector::onWritable, this));
			handler_.enableWrite();
		} else {
			DEBUG << "Connect errno = > " << err;
			handleErrorCallback();
		}
	} 
	
	if (ret == 0) {
		handler_.disableWrite();
		IpAddress local_address;
		DEBUG << "Fd => " << socket_.fd() << "Local Address " << local_address.toString() << " Peer  Address " << peer_address_.toString();
		Socket::getPeerAddress(socket_.fd(), &local_address);

		//ChannelPtr channelPtr(new Channel(event_loop_, socket_.fd(), local_address, peer_address_));
		ChannelPtr channel_ptr = (static_cast<ChannelPool*> (event_loop_->getMutableContext()))
									->acquire(event_loop_, socket_.fd(), local_address, peer_address_);

		handleConnectCallback(channel_ptr);
	}

}

void SocketConnector::onWritable() {
	IpAddress peer_address;

	int err = errno;
	if (Socket::getPeerAddress(socket_.fd(), &peer_address)) {
		handler_.disableWrite();
		IpAddress local_address;
		Socket::getLocalAddress(socket_.fd(), &local_address);
		DEBUG << "Fd => " << socket_.fd() << " Local Address " << local_address.toString() << " Peer  Address " << peer_address.toString();

		if (!Socket::isSelfConnect(socket_.fd())) {

			//ChannelPtr channel_ptr(new Channel(event_loop_, socket_.fd(), local_address, peer_address));
			ChannelPtr channel_ptr = (static_cast<ChannelPool*> (event_loop_->getMutableContext()))
								->acquire(event_loop_, socket_.fd(), local_address, peer_address);

			handleConnectCallback(channel_ptr);
		} else {
			int error;
			if (Socket::getError(socket_.fd(), &error) != 0 || error != 0) {
				WARN << Log::getError();
			}
			WARN << "Connect To Self";
		}
	} else {
		if (err == ETIMEDOUT || err == ECONNRESET || err == ECONNREFUSED || err == EINTR) {
			event_loop_->addTimer(std::bind(&SocketConnector::reconnect, this), Timestamp::now().macrosecond(), 3, 1);
		} else {
			assert(false);
			DEBUG << "Fd => " << socket_.fd() << " Peer  Address " << peer_address.toString();
			handleErrorCallback();
			handler_.remove();
		}
	}
}






