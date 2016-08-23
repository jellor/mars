/*========================================================
 *
 *   文件名称 ：HandlerChain.h
 *   创建日期 ：2016.7.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __HANDLER_CHAIN_H__
#define __HANDLER_CHAIN_H__

#include "AbstractInboundHandler.h"
#include "AbstractOutboundHandler.h"
#include "HandlerContext.h"

namespace mars {

class HandlerChain {
public:
	HandlerChain();
	~HandlerChain();

	void addInHandler(AbstractInboundHandler* handler);
	void addOutHandler(AbstractOutboundHandler* handler);

	void onRegistered(ChannelPtr channel_ptr, void* object);
	void onUnregistered(ChannelPtr channel_ptr, void* object);

	void fireActive(const ChannelPtr& channel_ptr, void* object);
	void fireReceive(const ChannelPtr& channel_ptr, void* object);
	void fireInactive(const ChannelPtr& channel_ptr);
	void fireError(const ChannelPtr& channel_ptr);

	void onReadComplete(ChannelPtr channel_ptr, void* object);
	void onWritablilityChanged(ChannelPtr channel_ptr, void* object);

	void doRead(void* object);
	void send(const ChannelPtr& channel_ptr, void* object);
	void send(const ChannelPtr& channel_ptr, char* data, int len);
	void sendTo(const ChannelPtr& channel_ptr, void* object);
	void doBind(void* object);
	void connect(const IpAddress& remote_address);
	void doDisconnect(void* object);
	void shutdownReceive(const ChannelPtr& channel_ptr);
	void shutdownSend(const ChannelPtr& channel_ptr);
	void close(const ChannelPtr& channel_ptr);
	void doUnregister(void* object);

private:
	AbstractInboundHandler* in_handler_chain_head_;
	AbstractInboundHandler* in_handler_chain_tail_;
	AbstractOutboundHandler* out_handler_chain_head_;
	AbstractOutboundHandler* out_handler_chain_tail_;

	AbstractOutboundHandler* sink_handler_;
	//HandlerContext handler_context_;
};

}

#endif