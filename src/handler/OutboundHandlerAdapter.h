/*========================================================
 *
 *   文件名称 ：OutboundHandlerAdapter.h
 *   创建日期 ：2016.5.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __OUTBOUND_HANDLER_ADAPTER_H__
#define __OUTBOUND_HANDLER_ADAPTER_H__

#include "AbstractOutboundHandler.h"

namespace mars {

class OutboundHandlerAdapter : public AbstractOutboundHandler {
public:
	OutboundHandlerAdapter()		  = default;
	virtual ~OutboundHandlerAdapter() = default;

	virtual void send(const ChannelPtr& channel_ptr, void* object) override {
		AbstractOutboundHandler* next = getNext();

		if (next != nullptr) {
			next->send(channel_ptr, object);
		}
		
	};

	virtual void send(const ChannelPtr& channel_ptr, char* data, int len) override {

		AbstractOutboundHandler* next = getNext();

		if (next != nullptr) {
			next->send(channel_ptr, data, len);
		}

	}

	virtual void sendTo(const ChannelPtr& channel_ptr, void* object) override {

		AbstractOutboundHandler* next = getNext();

		if (next != nullptr) {
			next->sendTo(channel_ptr, object);
		}

	};

	virtual void connect(const IpAddress& remote_address) override {

		AbstractOutboundHandler* next = getNext();

		if (next != nullptr) {
			next->connect(remote_address);
		}

	};

	virtual void shutdownReceive(const ChannelPtr& channel_ptr) override {

		AbstractOutboundHandler* next = getNext();

		if (next != nullptr) {
			next->shutdownReceive(channel_ptr);
		}

	}

	virtual void shutdownSend(const ChannelPtr& channel_ptr) override {

		AbstractOutboundHandler* next = getNext();

		if (next != nullptr) {
			next->shutdownSend(channel_ptr);
		}

	}

	virtual void close(const ChannelPtr& channel_ptr) override {

		AbstractOutboundHandler* next = getNext();

		if (next != nullptr) {
			next->close(channel_ptr);
		}

	}

	virtual const std::string getName() override {
		return "OutboundHandlerAdapter";
	}

};

}

#endif