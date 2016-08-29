/*========================================================
 *
 *   文件名称 ：HandlerChain.cpp
 *   创建日期 ：2016.7.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "HandlerChain.h"
#include "SinkHandler.h"
#include "Log.h"

using namespace mars;

HandlerChain::HandlerChain():
in_handler_chain_head_(nullptr),
in_handler_chain_tail_(nullptr),
out_handler_chain_head_(nullptr),
out_handler_chain_tail_(nullptr),
sink_handler_(new SinkHandler())
{
	DEBUG << "HandlerChain Constructor ...";
}

HandlerChain::~HandlerChain() {
	DEBUG << "HandlerChain Destructor ...";
	while (in_handler_chain_head_ != nullptr) {
		AbstractInboundHandler* temp = in_handler_chain_head_;
		in_handler_chain_head_ = in_handler_chain_head_->getNext();

		bool deletable = true;

		for (AbstractOutboundHandler* it = out_handler_chain_head_; it != nullptr; it = it->getNext()) {
			if (((AbstractHandler*)it)->getName() == ((AbstractHandler*)temp)->getName()) {
				deletable = false;
				break;
			}
		}
		if (deletable) {
			delete temp;
		}

	}

	// delete in_handler_chain_;
	// if (in_handler_chain_head_ != nullptr) {
	// 	delete in_handler_chain_head_;
	// }

	// delete out_handler_chain_;
	// if (out_handler_chain_head_ != nullptr) {
	// 	delete out_handler_chain_head_;
	// } else {
	// 	delete sink_handler_;
	// }

	if (out_handler_chain_head_ == nullptr) {
		delete sink_handler_;
	} else {
		while (out_handler_chain_head_ != nullptr) {
			AbstractOutboundHandler* temp = out_handler_chain_head_;
			out_handler_chain_head_ = out_handler_chain_head_->getNext();
			delete temp;
		}
	}

}


void HandlerChain::addInHandler(AbstractInboundHandler* handler) {

	if (handler == nullptr) {
		DEBUG << "Argument handler Should Don't nullptr";
		return ;
	}

	for (AbstractInboundHandler* it = in_handler_chain_head_; it != nullptr; it = it->getNext()) {
		if (it == handler) {
			WARN << "Handler Already Exsits In Handler Chain";
			return ;
		}
	}

	if (in_handler_chain_head_ == nullptr) {
		in_handler_chain_head_ = handler;
	} else {
		in_handler_chain_tail_->setNext(handler);
	}

	in_handler_chain_tail_ = handler;

}

void HandlerChain::addOutHandler(AbstractOutboundHandler* handler) {

	if (handler == nullptr) {
		DEBUG << "Argument handler Should Don't nullptr";
		return ;
	}

	for (AbstractOutboundHandler* it = out_handler_chain_head_; it != nullptr; it = it->getNext()) {
		if (it == handler) {
			WARN << "Handler Already Exsits In Handler Chain";
			return ;
		}
	}

	if (out_handler_chain_head_ == nullptr) {
		out_handler_chain_head_ = handler;
	} else {
		out_handler_chain_tail_->setNext(handler);
	}

	out_handler_chain_tail_ = handler;
	out_handler_chain_tail_->setNext(sink_handler_);

}

/*---------------------------------  Up Stream  ----------------------------------*/

void HandlerChain::onRegistered(ChannelPtr channel_ptr, void* object) {
	DEBUG << "On onRegistered";
}

void HandlerChain::onUnregistered(ChannelPtr channel_ptr, void* object) {
	DEBUG << "On Unregister";
}

void HandlerChain::fireActive(const ChannelPtr& channel_ptr, void* object) {
	DEBUG << "Fire Active";

	if (in_handler_chain_head_ != nullptr) {
		in_handler_chain_head_->onActive(channel_ptr);
	}
}

void HandlerChain::fireReceive(const ChannelPtr& channel_ptr, void* object) {
	DEBUG << "Fire Receive";

	if (in_handler_chain_head_ != nullptr) {
		in_handler_chain_head_->onReceive(channel_ptr, object);
	}

}

void HandlerChain::fireInactive(const ChannelPtr& channel_ptr) {
	DEBUG << "Fire Inactive";

	if (in_handler_chain_head_ != nullptr) {
		in_handler_chain_head_->onInactive(channel_ptr);
	}
}

void HandlerChain::fireError(const ChannelPtr& channel_ptr) {
	DEBUG << "Fire Error";

	if (in_handler_chain_head_ != nullptr) {
		in_handler_chain_head_->onError(channel_ptr);
	}
}

void HandlerChain::onReadComplete(ChannelPtr channel_ptr, void* object) {
	DEBUG << "On Read Complete";
}

void HandlerChain::onWritablilityChanged(ChannelPtr channel_ptr, void* object) {
	DEBUG << "On Writablility Changed";
}

/*---------------------------------- Down Stream ----------------------------------*/

void HandlerChain::doRead(void* object) {
	DEBUG << "Do Read";
}

void HandlerChain::send(const ChannelPtr& channel_ptr, void* object) {
	DEBUG << "Send";

	if (out_handler_chain_head_ != nullptr) {
		out_handler_chain_head_->send(channel_ptr, object);
	} else {
		sink_handler_->send(channel_ptr, object);
	}

}


void HandlerChain::send(const ChannelPtr& channel_ptr, const char* data, int len) {
	DEBUG << "Send";

	if (out_handler_chain_head_ != nullptr) {
		out_handler_chain_head_->send(channel_ptr, data, len);
	} else {
		sink_handler_->send(channel_ptr, data, len);
	}
}


void HandlerChain::sendTo(const ChannelPtr& channel_ptr, void* object) {
	DEBUG << "SendTo";

	if (out_handler_chain_head_ != nullptr) {
		out_handler_chain_head_->sendTo(channel_ptr, object);
	} else {
		sink_handler_->sendTo(channel_ptr, object);
	}

}

void HandlerChain::doBind(void* object) {
	DEBUG << "Do Bind";
}

void HandlerChain::connect(const IpAddress& remote_address) {
	DEBUG << "Connect";

	if (out_handler_chain_head_ != nullptr) {
		out_handler_chain_head_->connect(remote_address);
	} else {
		sink_handler_->connect(remote_address);
	}

}

void HandlerChain::doDisconnect(void* object) {
	DEBUG << "Do Disconnect";
}

void HandlerChain::shutdownReceive(const ChannelPtr& channel_ptr) {
	DEBUG << "Shutdown Receive";
	if (out_handler_chain_head_ != nullptr) {
		out_handler_chain_head_->shutdownReceive(channel_ptr);
	} else {
		sink_handler_->shutdownReceive(channel_ptr);
	}
}

void HandlerChain::shutdownSend(const ChannelPtr& channel_ptr) {
	if (out_handler_chain_head_ != nullptr) {
		out_handler_chain_head_->shutdownSend(channel_ptr);
	} else {
		sink_handler_->shutdownSend(channel_ptr);
	}
}

void HandlerChain::close(const ChannelPtr& channel_ptr) {
	DEBUG << "Close Channel";

	if (out_handler_chain_head_ != nullptr) {
		out_handler_chain_head_->close(channel_ptr);
	} else {
		sink_handler_->close(channel_ptr);
	}

}

void HandlerChain::doUnregister(void* object) {
	DEBUG << "Do Unregister";
}

// bool HandlerChain::available(const ChannelPtr& channel_ptr) const {
// 	if (available_callback_ != nullptr) {
// 		return available_callback_(channel_ptr);
// 	} else {
// 		return false;
// 	}
// }


