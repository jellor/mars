#include "Bootstrap.h"
#include "InboundHandlerAdapter.h"
#include "OutboundHandlerAdapter.h"
#include "Log.h"
#include "AbstractInboundHandler.h"
#include "Timestamp.h"
#include "Thread.h"
#include <iostream>

using namespace mars;
using namespace std;

class ClientHandler : public InboundHandlerAdapter {
public:
	ClientHandler() {
		DEBUG << "ClientHandler Constructor ...";
		setName("ClientHandler");
	}

	~ClientHandler() {
		DEBUG << getName() << " Destructor ...";
	}

	void onActive(const ChannelPtr& channel_ptr) override {
		DEBUG << "............. onActive .............";
		char request[] = "Response ....\n";
		channel_ptr->getChain()->send(channel_ptr, request, strlen(request));
	}

	void onReceive(const ChannelPtr& channel_ptr, void* object) override {
		DEBUG << "............. OK .............";
		if (object == nullptr) {
			DEBUG << "Object Is Nullptr";
		} else {
			DEBUG << "Object Is Not Nullptr";
		}
	}

	void onInactive(const ChannelPtr& channel_ptr) override {
		channel_ptr->getChain()->close(channel_ptr);
	}

};

class EncoderHandler : public InboundHandlerAdapter {
public:
	EncoderHandler() {
		DEBUG << "EncoderHandler Constructor ...";
		setName("EncoderHandler");
	}

	virtual ~EncoderHandler() {
		DEBUG << "EncoderHandler Destructor ...";
	}

	virtual void onReceive(const ChannelPtr& channel_ptr, void* object) override {
		// while (protobufCodec_.valid(channelPtr->getInBuffer())) {
		// 	Message* message = protobufCodec_.decode(channelPtr->getInBuffer());
		// 	if(message == nullptr) {
		// 		DEBUG << "Cannont Decode Message";
		// 		break;
		// 	}
		// }
	}

};

class DecoderHandler : public OutboundHandlerAdapter {
public:
	DecoderHandler() {
		DEBUG << "DecoderHandler Constructor ...";
		setName("DecoderHandler");
	}

	virtual ~DecoderHandler() {
		DEBUG << "DecoderHandler Destructor ...";
	}

	virtual void sendTo(const ChannelPtr& channel_ptr, void* object) override {

	}

};

class Client {
public:
	Client(const std::vector<IpAddress*>& listen_address_list, const std::vector<IpAddress*>& connect_address_list,
		int acceptor_count, int worker_count, int worker_thread_count):
		bootstrap_(listen_address_list, connect_address_list, acceptor_count, worker_count, worker_thread_count)
	{
		DEBUG << "Client Constructor ...";
	}

	~Client() {
		DEBUG << "Client Destructor ...";
		bootstrap_.join();
		DEBUG << "Client Destructor OK";
	}

	void addInHandler(AbstractInboundHandler* handler) {
		bootstrap_.setChainFactory([] (const ChannelPtr& channel_ptr) {
			HandlerChain* chain = new HandlerChain();
			ClientHandler* handler = new ClientHandler();
			chain->addInHandler(handler);
			return chain;
		});
	}

	void start() {
		bootstrap_.start();
	}

private:
	Bootstrap bootstrap_;
};

int main() {

	IpAddress ip_address("127.0.0.1", 8090);

	cout << "Enter A Number (If 0 Will Connect To " << ip_address.toString() << ", Otherwise To 121.42.37.52:8090)" << endl;
	int flag;
	cin >> flag;
	if (flag != 0) {
		IpAddress address("121.42.37.52", 8090);
		ip_address = address;
	}

	cout << "Enter A Connection Number" << endl;;
	int count;
	cin >> count;

	std::vector<IpAddress*> listen_address_list;
	std::vector<IpAddress*> connect_address_list;

	DEBUG << ip_address.toString();

	for (int i = 0; i < count; i ++) {
		connect_address_list.push_back(&ip_address);
	}


	Client Client(listen_address_list, connect_address_list, 1, 5, 2);

	ClientHandler* handler = new ClientHandler();

	Client.addInHandler(handler);

	Client.start();

	DEBUG << "STOP";
	return 0;
}



