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

	virtual void onRead(ChannelPtr channel_ptr, void* object) override {}
	virtual void onError(ChannelPtr channel_ptr) 			  override {}
};

}

#endif