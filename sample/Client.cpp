#include "Bootstrap.h"
#include "InboundHandlerAdapter.h"
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

	void onRead(ChannelPtr channel_ptr, void* object) override {
		DEBUG << "............. OK .............";
		if (object == nullptr) {
			DEBUG << "Object Is Nullptr";
		} else {
			DEBUG << "Object Is Not Nullptr";
		}
	}

};

class Client {
public:
	Client(const std::vector<IpAddress*>& listen_address_list, const std::vector<IpAddress*>& connect_address_list, 
		int acceptor_count, int worker_count, int worker_thread_count): 
		bootstrap_(listen_address_list, connect_address_list, acceptor_count, worker_count, worker_thread_count),
		http_chain_(),
		tcp_chain_()
	{
		DEBUG << "Client Constructor ...";
		http_chain_.setAvailableCallback([this] (const ChannelPtr& channel_ptr) {
			DEBUG << "available callback in http_chain";
			return false;
		});
		tcp_chain_.setAvailableCallback([this] (const ChannelPtr& channel_ptr) {
			DEBUG << "available callback in tcp_chain";
			return true;
		});
	}

	~Client() {
		DEBUG << "Client Destructor ...";
		bootstrap_.join();
		DEBUG << "Client Destructor OK";
	}

	void addInHandler(AbstractInboundHandler* handler) {
		http_chain_.addInHandler(handler);
		tcp_chain_.addInHandler(handler);
	}

	void start() {
		bootstrap_.addChain(&http_chain_);
		bootstrap_.addChain(&tcp_chain_);
		bootstrap_.start();
	}

private:
	Bootstrap bootstrap_;
	HandlerChain http_chain_;
	HandlerChain tcp_chain_;
};

int main() {

	IpAddress ip_address("14.215.177.38", 80);

	cout << "Enter A Number (If 0 Will Connect To 127.0.0.1:8090, Otherwise To 121.42.37.52:8090)" << endl;
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
	DEBUG << "Begin " << Timestamp::now().toString();
	for (int i = 0; i < count; i ++) {
		connect_address_list.push_back(&ip_address);
	}

	Thread::sleep(5);

	DEBUG << "End   " << Timestamp::now().toString();

	
	Client Client(listen_address_list, connect_address_list, 1, 5, 2);

	ClientHandler* handler = new ClientHandler();

	Client.addInHandler(handler);

	Client.start();
	
	DEBUG << "STOP";
	return 0;
}



