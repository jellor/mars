/*========================================================
 *
 *   文件名称 ：Connector.cpp
 *   创建日期 ：2016.6.2
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "Connector.h"
#include "Log.h"
#include <errno.h>

using namespace mars;

Connector::Connector(const std::vector<IpAddress*>& ip_address_list):
started_(false),
event_loop_(nullptr),
ip_address_list_(ip_address_list),
socket_connector_list_(),
thread_(),
connect_callback_(nullptr),
read_callback_(nullptr),
write_callback_(nullptr),
close_callback_(nullptr),
error_callback_(nullptr)
{
	
}

Connector::~Connector() {
	thread_.join();
	event_loop_->stop();	
	if (started_) {
		for (int i = 0; i < socket_connector_list_.size(); i ++) {
			delete socket_connector_list_[i];
		}
	}
	socket_connector_list_.clear();
}

void Connector::start() {
	thread_.setThreadFunc(std::bind(&Connector::runInThread, this));
	thread_.setThreadName("Connector Thread");
	thread_.start();

	while (started_ == false) {}

	for (int i = 0; i < ip_address_list_.size(); i ++) {
		SocketConnector* element = new SocketConnector(event_loop_, *ip_address_list_[i]);
		element->setConnectCallback(std::bind(&Connector::handleConnectEvent, this, std::placeholders::_1));
		element->connect();
		//element->setConnectCallback(std::bind(&Connector::handleConnectEvent, this, std::placeholders::_1));
		socket_connector_list_.push_back(element);
	}
}

void Connector::connect(const IpAddress& remote_address) {
	SocketConnector* element = new SocketConnector(event_loop_, remote_address);
	element->setConnectCallback(std::bind(&Connector::handleConnectEvent, this, std::placeholders::_1));
	element->connect();
	//element->setConnectCallback(std::bind(&Connector::handleConnectEvent, this, std::placeholders::_1));
	socket_connector_list_.push_back(element);
}

void Connector::join() {
	thread_.join();
}

void Connector::handleConnectEvent(const ChannelPtr& channel_ptr) {
	DEBUG << "Handle Connect Event";

	handleConnect(channel_ptr);
	//channel_ptr->setConnectCallback(std::bind(&Connector::handleConnect, this, channel_ptr));
	channel_ptr->setReadCallback(std::bind(&Connector::handleRead,   this, std::placeholders::_1));
	channel_ptr->setCloseCallback(std::bind(&Connector::handleClose, this, std::placeholders::_1));
	channel_ptr->setWriteCallback(std::bind(&Connector::handleWrite, this, std::placeholders::_1));
	channel_ptr->setErrorCallback(std::bind(&Connector::handleError, this, std::placeholders::_1));

	// channel_ptr->getHandler()->enableRead();
}

void Connector::runInThread() {
	event_loop_      			= new EventLoop();
	ChannelPool* channel_pool	= new ChannelPool(event_loop_);
	event_loop_->setContext(channel_pool);

	started_ = true;

	event_loop_->start();

	delete event_loop_;
	delete channel_pool;
	event_loop_		 = nullptr;
	channel_pool	 = nullptr;
	started_ 		 = false;
}








