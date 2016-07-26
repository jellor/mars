#include "Bootstrap.h"
#include "InboundHandlerAdapter.h"
#include "Log.h"
#include "AbstractInboundHandler.h"
#include "Thread.h"

using namespace mars;

class ServerHandler : public InboundHandlerAdapter {
public:
	ServerHandler() {
		DEBUG << "ServerHandler Constructor ...";
		setName("ServerHandler");
	}

	~ServerHandler() {
		DEBUG << getName() << " Destructor ...";
	}

	void onRead(ChannelPtr channel_ptr, void* object) override {
		DEBUG << "............. OK .............";
		Thread::sleep(10);
		if (object == nullptr) {
			DEBUG << "Object Is Nullptr";
		} else {
			DEBUG << "Object Is Not Nullptr";
		}
	}

};

class Server {
public:
	Server(const std::vector<IpAddress*>& listen_address_list, const std::vector<IpAddress*>& connect_address_list, 
		int acceptor_count, int worker_count, int worker_thread_count): 
		bootstrap_(listen_address_list, connect_address_list, acceptor_count, worker_count, worker_thread_count),
		http_chain_(),
		tcp_chain_()
	{
		DEBUG << "Server Constructor ...";
		http_chain_.setAvailableCallback([this] (const ChannelPtr& channel_ptr) {
			DEBUG << "available callback in http_chain";
			return false;
		});
		tcp_chain_.setAvailableCallback([this] (const ChannelPtr& channel_ptr) {
			DEBUG << "available callback in tcp_chain";
			return true;
		});
	}

	~Server() {
		DEBUG << "Server Destructor ...";
		bootstrap_.join();
		DEBUG << "Server Destructor OK";
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
	Bootstrap 	 bootstrap_;
	HandlerChain http_chain_;
	HandlerChain tcp_chain_;

};

int main() {
	IpAddress listen_address("127.0.0.1", 8090);

	std::vector<IpAddress*> listen_address_list;
	listen_address_list.push_back(&listen_address);
	listen_address_list.push_back(&listen_address);
	std::vector<IpAddress*> connect_address_list;

	//connectAddrList.push_back(&connectAddr);

	Server server(listen_address_list, connect_address_list, 2, 5, 2);

	ServerHandler* handler = new ServerHandler();

	server.addInHandler(handler);

	server.start();
	
	DEBUG << "STOP";
	return 0;
}



