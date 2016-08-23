/*========================================================
 *
 *   文件名称 ：SinkHandler.h
 *   创建日期 ：2016.7.28
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *
 *=======================================================*/

#ifndef __SINK_HANDLER_H__
#define __SINK_HANDLER_H__

#include "AbstractOutboundHandler.h"
#include "IpAddress.h"

namespace mars {

class SinkHandler : public AbstractOutboundHandler {
public:
	SinkHandler() {
		// DEBUG << "SinkHandler Constructor ..."; 
	};

	virtual ~SinkHandler() { 
		DEBUG << "SinkHandler Destructor ..."; 
	};

	virtual void send(const ChannelPtr& channel_ptr, void* object) override;
	virtual void send(const ChannelPtr& channel_ptr, char* data, int len) override;
	virtual void sendTo(const ChannelPtr& channel_ptr, void* object) override;
	virtual void connect(const IpAddress& remote_address) override;
	virtual void shutdownSend(const ChannelPtr& channel_ptr) override;
	virtual void shutdownReceive(const ChannelPtr& channel_ptr) override;
	virtual void close(const ChannelPtr& channel_ptr) override;

	virtual const std::string getName() override { return "SinkHandler"; }

};

}

#endif