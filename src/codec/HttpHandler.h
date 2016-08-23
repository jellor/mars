/*========================================================
 *
 *   文件名称 ：HttpHandler.h
 *   创建日期 ：2016.8.9
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __HTTP_HANDLER_H__
#define __HTTP_HANDLER_H__

#include "HttpContext.h"
#include "RingBuffer.h"
#include "InboundHandlerAdapter.h"
#include "OutboundHandlerAdapter.h"

namespace mars {

class HttpHandler : public InboundHandlerAdapter, public OutboundHandlerAdapter {
public:
	HttpHandler();
	~HttpHandler();

	virtual void onActive(const ChannelPtr& channel_ptr) override {

	}

	virtual void onInactive(const ChannelPtr& channel_ptr) override {

	}

	virtual void onReceive(const ChannelPtr& channel_ptr, void* object) override {
		DEBUG << "HttpContext";
		http_context_.decode(channel_ptr->getMutableInBuffer());
	}

private:
	HttpContext http_context_;
};

}

#endif