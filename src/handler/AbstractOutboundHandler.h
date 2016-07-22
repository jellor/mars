/*========================================================
 *
 *   文件名称 ：AbstractOutboundHandler.h
 *   创建日期 ：2016.7.18
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __ABSTRACT_OUTBOUND_HANDLER_H__
#define __ABSTRACT_OUTBOUND_HANDLER_H__

#include "AbstractHandler.h"
#include <string>

namespace mars {

class AbstractOutboundHandler : public AbstractHandler {
public:
	AbstractOutboundHandler();
	virtual ~AbstractOutboundHandler();

	virtual void onWrite(ChannelPtr channel_ptr, void* object) = 0;
	virtual void onError(ChannelPtr channel_ptr)               = 0;

	void handle(ChannelPtr channel_ptr, void* object) override;

	void setNext(AbstractOutboundHandler* next);
	AbstractOutboundHandler* getNext() const;

private:
	AbstractOutboundHandler* next_;

};

}

#endif