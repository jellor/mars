/*========================================================
 *
 *   文件名称 ：Bootstrap.cpp
 *   创建日期 ：2016.7.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "Bootstrap.h"

using namespace mars;

Bootstrap::Bootstrap(const std::vector<IpAddress>& listen_address_list, const std::vector<IpAddress>& connect_address_list, 
	int acceptor_count, int worker_count, int worker_thread_count = 2):
acceptor_(nullptr),
connector_(nullptr),
threadPool_(worker_thread_count),
filter_callback_(nullptr),
connect_callback_(nullptr),
read_callback_(nullptr),
write_callback_(nullptr),
close_callback_(nullptr),
error_callback_(nullptr),
chain_factory_(nullptr)
{
	if (acceptor_count > 0) {
		acceptor_ = new Acceptor(listen_address_list, acceptor_count, worker_count);
		acceptor_->setFilterCallback(std::bind(&Bootstrap::handleFilter, 
			this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		acceptor_->setConnectCallback(std::bind(&Bootstrap::handleConnect, this, std::placeholders::_1));
		acceptor_->setReadCallback(std::bind(&Bootstrap::handleRead, this, std::placeholders::_1));
		acceptor_->setCloseCallback(std::bind(&Bootstrap::handleClose, this, std::placeholders::_1));
		acceptor_->setWriteCallback(std::bind(&Bootstrap::handleWrite, this, std::placeholders::_1));
		acceptor_->setErrorCallback(std::bind(&Bootstrap::handleError, this, std::placeholders::_1));
	}
	if (connect_address_list.size() > 0) {
		connector_ = new Connector(connect_address_list);
		connector_->setConnectCallback(std::bind(&Bootstrap::handleConnect, this, std::placeholders::_1));
		connector_->setReadCallback(std::bind(&Bootstrap::handleRead, this, std::placeholders::_1));
		connector_->setCloseCallback(std::bind(&Bootstrap::handleClose, this, std::placeholders::_1));
		connector_->setWriteCallback(std::bind(&Bootstrap::handleWrite, this, std::placeholders::_1));
		connector_->setErrorCallback(std::bind(&Bootstrap::handleError, this, std::placeholders::_1));
	}
}

Bootstrap::~Bootstrap() {
	acceptor_->join();
	connector_->join();
	if (acceptor_ != nullptr) {
		delete acceptor_;
	}
	if (connector_ != nullptr) {
		delete connector_;
	}
	DEBUG << "Bootstrap Destructor ...";
}

void Bootstrap::start() {
	if (acceptor_ != nullptr) {
		acceptor_->start();
	}
	if (connector_ != nullptr) {
		connector_->start();
	}
}

void Bootstrap::join() {
	acceptor_->join();
	connector_->join();
	DEBUG << "Join";
}

bool Bootstrap::handleFilter(int fd, const IpAddress& local_address, const IpAddress& peer_address) {
	DEBUG << "Handle Filter";
	if (filter_callback_ != nullptr) {
		return filter_callback_(fd, local_address, peer_address);
	} else {
		return true;
	}
}

void Bootstrap::handleConnect(const ChannelPtr& channel_ptr) {
	DEBUG << "Handle Connect";
	if (connect_callback_ != nullptr) {
		connect_callback_(channel_ptr);
	}

	if (chain_factory_ != nullptr) {
		HandlerChain* handler_chain = chain_factory_(channel_ptr);
		if (handler_chain != nullptr) {
			channel_ptr->setHandlerChain(handler_chain);
			handler_chain->fireActive(channel_ptr, nullptr);
		}
	}	
}

void Bootstrap::handleRead(const ChannelPtr& channel_ptr) {
	DEBUG << "Handle Read";

	if (read_callback_ != nullptr) {
		read_callback_(channel_ptr);
	}
	
	HandlerChain* handler_chain = channel_ptr->getChain();
	if (handler_chain != nullptr) {
		handler_chain->fireReceive(channel_ptr, nullptr);
	}
	
	// while (protobufCodec_.valid(channelPtr->getInBuffer())) {
	// 	Message* message = protobufCodec_.decode(channelPtr->getInBuffer());
	// 	if(message == nullptr) {
	// 		DEBUG << "Cannont Decode Message";
	// 		break;
	// 	}
	// }
}

void Bootstrap::handleClose(const ChannelPtr& channel_ptr) {
	DEBUG << "Handle Close";

	if (close_callback_ != nullptr) {
		close_callback_(channel_ptr);
	}

	HandlerChain* handler_chain = channel_ptr->getChain();
	if (handler_chain != nullptr) {
		handler_chain->fireInactive(channel_ptr);
	}
	// DEBUG << "errno => " << errno;
	// DEBUG << "errno => " << Log::getError();
}

void Bootstrap::handleWrite(const ChannelPtr& channel_ptr) {
	DEBUG << "Handle Write";

	if (write_callback_ != nullptr) {
		write_callback_(channel_ptr);
	}
}

void Bootstrap::handleError(const ChannelPtr& channel_ptr) {
	DEBUG << "Handle Error";

	if (error_callback_ != nullptr) {
		error_callback_(channel_ptr);
	}

	HandlerChain* handler_chain = channel_ptr->getChain();
	if (handler_chain != nullptr) {
		handler_chain->fireError(channel_ptr);
	}
}



