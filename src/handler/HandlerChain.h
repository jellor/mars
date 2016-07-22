/*========================================================
 *
 *   文件名称 ：HandlerChain.h
 *   创建日期 ：2016.7.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#include "AbstractInboundHandler.h"
#include "AbstractOutboundHandler.h"

#ifndef __HANDLER_CHAIN_H__
#define __HANDLER_CHAIN_H__


namespace mars {

class HandlerChain {

	typedef std::function<bool(const ChannelPtr&)> AvailableCallback;

public:
	HandlerChain();
	~HandlerChain();

	void doInHandler(ChannelPtr channel_ptr, void* object);
	void doOutHandler(ChannelPtr channel_ptr, void* object);

	void addInHandler(AbstractInboundHandler* handler);
	void addOutHandler(AbstractOutboundHandler* handler);

	bool available(const ChannelPtr& channel_ptr) const;

	void setAvailableCallback(const AvailableCallback& available_cb) { available_callback_ = available_cb; }

private:
	AbstractInboundHandler* in_handler_chain_head_;
	AbstractInboundHandler* in_handler_chain_tail_;
	AbstractOutboundHandler* out_handler_chain_head_;
	AbstractOutboundHandler* out_handler_chain_tail_;

	AvailableCallback available_callback_;
};

}

#endif