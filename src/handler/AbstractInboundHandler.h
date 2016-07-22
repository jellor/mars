/*========================================================
 *
 *   文件名称 ：AbstractInboundHandler.h
 *   创建日期 ：2016.7.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __ABSTRACT_INBOUND_HANDLER_H__
#define __ABSTRACT_INBOUND_HANDLER_H__

#include "AbstractHandler.h"
#include <string>

namespace mars {

class AbstractInboundHandler : public AbstractHandler {
public:
	AbstractInboundHandler();
	virtual ~AbstractInboundHandler();

	virtual void onRead(ChannelPtr channel_ptr, void* object) = 0;
	virtual void onError(ChannelPtr channel_ptr) 			  = 0;

	void handle(ChannelPtr channel_ptr, void* object) override;

	void setNext(AbstractInboundHandler* next);
	AbstractInboundHandler* getNext() const;

private:
	AbstractInboundHandler* next_;

};

}

#endif