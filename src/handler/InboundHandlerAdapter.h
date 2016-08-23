/*========================================================
 *
 *   文件名称 ：InboundHandlerAdapter.h
 *   创建日期 ：2016.5.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __INBOUND_HANDLER_ADAPTER_H__
#define __INBOUND_HANDLER_ADAPTER_H__

#include "AbstractInboundHandler.h"

namespace mars {

class InboundHandlerAdapter : public AbstractInboundHandler {
public:
	InboundHandlerAdapter() 		 = default;
	virtual ~InboundHandlerAdapter() = default;

	virtual void onActive(const ChannelPtr& channel_ptr) override {

		AbstractInboundHandler* next = getNext();

		if (next != nullptr) {
			next->onActive(channel_ptr);
		}

	}

	virtual void onReceive(const ChannelPtr& channel_ptr, void* object) override {

		AbstractInboundHandler* next = getNext();

		if (next != nullptr) {
			next->onReceive(channel_ptr, object);
		}

	}

	virtual void onInactive(const ChannelPtr& channel_ptr) override {

		AbstractInboundHandler* next = getNext();

		if (next != nullptr) {
			next->onInactive(channel_ptr);
		}

	}

	virtual void onError(const ChannelPtr& channel_ptr) override {

		AbstractInboundHandler* next = getNext();

		if (next != nullptr) {
			next->onError(channel_ptr);
		}

	}

	virtual const std::string getName() override {
		return "InboundHandlerAdapter";
	}
};

}

#endif

