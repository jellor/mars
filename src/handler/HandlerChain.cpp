/*========================================================
 *
 *   文件名称 ：HandlerChain.cpp
 *   创建日期 ：2016.7.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "HandlerChain.h"
#include "Log.h"

using namespace mars;

HandlerChain::HandlerChain():
in_handler_chain_head_(nullptr),
in_handler_chain_tail_(nullptr),
out_handler_chain_head_(nullptr),
out_handler_chain_tail_(nullptr),
available_callback_(nullptr)
{

}

HandlerChain::~HandlerChain() {
	while (in_handler_chain_head_ != nullptr) {
		AbstractInboundHandler* temp = in_handler_chain_head_;
		in_handler_chain_head_ = in_handler_chain_head_->getNext();
		delete temp;
	}
	if (out_handler_chain_head_ != nullptr) {
		AbstractOutboundHandler* temp = out_handler_chain_head_;
		out_handler_chain_head_ = out_handler_chain_head_->getNext();
		delete temp;
	}
	DEBUG << "HandlerChain Destructor ...";
}

void HandlerChain::doInHandler(ChannelPtr channel_ptr, void* object) {
	if (in_handler_chain_head_ != nullptr) {
		in_handler_chain_head_->handle(channel_ptr, object);
	}
}

void HandlerChain::doOutHandler(ChannelPtr channel_ptr, void* object) {
	if (out_handler_chain_head_ != nullptr) {
		out_handler_chain_head_->handle(channel_ptr, object);
	}
}

void HandlerChain::addInHandler(AbstractInboundHandler* handler) {
	if (in_handler_chain_head_ == nullptr) {
		in_handler_chain_head_ = handler;
		in_handler_chain_tail_ = handler;
	} else {
		in_handler_chain_tail_->setNext(handler);
		in_handler_chain_tail_ = handler;
	}
}

void HandlerChain::addOutHandler(AbstractOutboundHandler* handler) {
	if (out_handler_chain_head_ == nullptr) {
		out_handler_chain_head_ = handler;
		out_handler_chain_tail_ = handler;
	} else {
		out_handler_chain_tail_->setNext(handler);
		out_handler_chain_tail_ = handler;
	}
}

bool HandlerChain::available(const ChannelPtr& channel_ptr) const {
	if (available_callback_ != nullptr) {
		return available_callback_(channel_ptr);
	} else {
		return false;
	}
}


