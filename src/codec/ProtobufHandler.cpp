/*========================================================
 *
 *   文件名称 ：ProtobufHandler.cpp
 *   创建日期 ：2016.8.9
 *   作 者 ：Zhu Guodong
 *   邮 箱 ：18814092650@163.com
 *   描 述 ：
 *
 *=======================================================*/

#include "ProtobufHandler.h"
#include "BufferStream.h"
#include "Log.h"
#include <string>
#include <unistd.h>
#include <google/protobuf/message_lite.h>

using namespace mars;


bool ProtobufHandler::valid(RingBuffer* buffer)
{
    if (buffer->size() < 16)
    {
        return false;
    }
    char* head   = const_cast<char*>(buffer->head());
    int32_t size = *reinterpret_cast<int32_t*>(head);
    if (size <= buffer->size())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void ProtobufHandler::encode(Message* message, RingBuffer** buf)
{
    std::string name_str = message->GetTypeName();
    int16_t name_length  = strlen(name_str.c_str());
    int32_t size 		 = message->ByteSize() + name_length + 6;
    RingBuffer* buffer   = new RingBuffer(size);

    DEBUG << message->ByteSize();
    DEBUG << name_str ;
    DEBUG << name_length;
    DEBUG << size;

    buffer->write(reinterpret_cast<const char*>(&size), 4);
    buffer->write(reinterpret_cast<const char*>(&name_length), 2);
    buffer->write(name_str.c_str(), name_length);

    DEBUG << "Buffer size " << buffer->size() << " Buffer capitiy " << buffer->capacity();
    serializeToBuffer(message, buffer);
    DEBUG << "Buffer size " << buffer->size() << " Buffer capitiy " << buffer->capacity();
    *buf = buffer;
}

Message* ProtobufHandler::decode(RingBuffer* buffer)
{
    buffer->adjust();
    char* head  	    = const_cast<char*>(buffer->head());
    int32_t size 	    = *reinterpret_cast<int32_t*>(head);
    int16_t name_length = *reinterpret_cast<int16_t*>(head + 4);

    DEBUG << "Size " << size << " name_length " << name_length;

    std::string name_str(head + 6, name_length);
    DEBUG << "Buffer size " << buffer->size() << " Buffer capitiy " << buffer->capacity();
    // buffer->headSkip(6 + name_length);
    buffer->skipFromHead(6 + name_length);
    DEBUG << "name_str => " << name_str << " name_length => " << name_length;

    const Descriptor* descriptor = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(name_str);
    if (descriptor == nullptr)
    {
        WARN << "Cannot Create Descriptor";
        return nullptr;
    }
    const Message* prototype = google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
    if (prototype == nullptr)
    {
        WARN << "Cannot Create Prototype";
        return nullptr;
    }
    Message* message = prototype->New();
    if (message == nullptr)
    {
        WARN << "Cannon Create Message";
    }
    else
    {
        DEBUG << "Created Message";
    }
    DEBUG << "Head Index " << buffer->headIndex() << " Tail Index " << buffer->tailIndex();
    DEBUG << "Buffer size " << buffer->size() << " Buffer Capacity " << buffer->capacity();
    DEBUG << "Message size " << message->ByteSize() << " Message name " << message->GetTypeName();
    // DEBUG << parseFromBuffer(buffer, message);
    message->ParseFromArray(buffer->head(), buffer->size());
    //message->ParseFromArray()
    // buffer->headSkip(size - name_length - 6);
    buffer->skipFromHead(size - name_length - 6);
    DEBUG << "Message size " << message->ByteSize() << " Message name " << message->GetTypeName();
    DEBUG << "Buffer size " << buffer->size() << " Buffer Capacity " << buffer->capacity();

    return message;
}


void ProtobufHandler::registerMessage()
{

}

void ProtobufHandler::sendMessage(Message* message, const ChannelPtr& channel_ptr)
{
    DEBUG << "Message size " << message->ByteSize() << " Message name " << message->GetTypeName();
    RingBuffer* buffer;
    encode(message, &buffer);
    DEBUG << "RingBuffer Size " << buffer->size() << " Capacity " << buffer->capacity();
    channel_ptr->send(buffer);
}

bool ProtobufHandler::parseFromBuffer(RingBuffer* buffer, Message* message)
{
    BufferInputStream inputStream(buffer);
    return message->ParseFromZeroCopyStream(&inputStream);
}

bool ProtobufHandler::serializeToBuffer(Message* message, RingBuffer* buffer)
{
    BufferOutputStream outputStream(buffer);
    return message->SerializeToZeroCopyStream(&outputStream);
}


