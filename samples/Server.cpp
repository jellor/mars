#include "Bootstrap.h"
#include "InboundHandlerAdapter.h"
#include "OutboundHandlerAdapter.h"
#include "Log.h"
#include "AbstractInboundHandler.h"
#include "Thread.h"
#include "HttpHandler.h"

using namespace mars;

class DefaultHandler : public OutboundHandlerAdapter {
public:
	DefaultHandler() {
		DEBUG << "DefaultHandler Constructor ...";
	}

	~DefaultHandler() {
		DEBUG << " Destructor ...";
	}

	virtual void send(const ChannelPtr& channel_ptr, char* data, int len) override {
		DEBUG << "DefaultHandler Send";
		getNext()->send(channel_ptr, data, len);
	}

};

class Server {
public:
	Server(const std::vector<IpAddress>& listen_address_list, const std::vector<IpAddress>& connect_address_list, 
		int acceptor_count, int worker_count, int worker_thread_count): 
		bootstrap_(listen_address_list, connect_address_list, acceptor_count, worker_count, worker_thread_count)
	{
		DEBUG << "Server Constructor ...";
	}

	~Server() {
		DEBUG << "Server Destructor ...";
		bootstrap_.join();
		DEBUG << "Server Destructor OK";
	}

	void start() {
		bootstrap_.setChainFactory([] (const ChannelPtr& channel_ptr) {

			HandlerChain* chain = new HandlerChain();

			chain->addInHandler(new HttpHandler());
			chain->addOutHandler(new DefaultHandler());

			return chain;
		});
		bootstrap_.start();
		bootstrap_.join();	
	}

private:
	Bootstrap 	 bootstrap_;

};

int main() {
	std::vector<IpAddress> listen_address_list;
	std::vector<IpAddress> connect_address_list;

	IpAddress listen_address_a("127.0.0.1", 8090);
	IpAddress listen_address_b("127.0.0.1", 8080);

	listen_address_list.push_back(listen_address_a);
	listen_address_list.push_back(listen_address_b);

	Server* server = new Server(listen_address_list, connect_address_list, 2, 2, 1);

	server->start();
	DEBUG << "STOP";

	return 0;
}



