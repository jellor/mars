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

	virtual onWrite(ChannelPtr channel_ptr, void* object) override {}
	virtual onError(ChannelPtr channel_ptr)				  override {}
};

}

#endif