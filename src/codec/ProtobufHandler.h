/*========================================================
 *
 *   文件名称 ：ProtobufHandler.h
 *   创建日期 ：2016.8.9
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *   描 述 ：
 *
 *=======================================================*/

#ifndef __PROTOBUF_CODEC_H__
#define __PROTOBUF_CODEC_H__

#include "RingBuffer.h"
#include "InboundHandlerAdapter.h"
#include "OutboundHandlerAdapter.h"
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <map>
#include <memory>
#include <functional>

namespace mars {

typedef std::function<void(void)> MessageCallback;
typedef std::shared_ptr<google::protobuf::Message> MessagePtr;
typedef google::protobuf::Message Message;
typedef google::protobuf::Descriptor Descriptor;

class ProtobufHandler : public OutboundHandlerAdapter, public InboundHandlerAdapter {
public:
     
	virtual void onReceive(const ChannelPtr& channel_ptr, void* object) override {
		DEBUG << "Handle Read";
		while (valid(channel_ptr->getMutableInBuffer())) {
			Message* message = decode(channel_ptr->getMutableInBuffer());
			if(message == nullptr) {
				DEBUG << "Cannot Decode Message";
				break;
			}
			DEBUG << "Get Message";
			// protobufDispatcher_.dispatcher(*message);
			// if (onMessageCb_ != nullptr) onMessageCb_(channel_ptr, message);
			onMessage(channel_ptr, message);
		}
	}

	virtual void onMessage(const ChannelPtr& channel_ptr, void* message) {

	}

	virtual const std::string getName() override {
		return "ProtobufHandler";
	}

	bool valid(RingBuffer* buffer);
	void encode(Message* message, RingBuffer** buffer);
	Message* decode(RingBuffer* buffer);

	void registerMessage();
	void sendMessage(Message* message, const ChannelPtr& channel_ptr);

private:
	bool parseFromBuffer(RingBuffer* buffer, Message* message);
	bool serializeToBuffer(Message* message, RingBuffer* buffer);

	std::map<Descriptor*, MessageCallback> map_;
};

}

#endif