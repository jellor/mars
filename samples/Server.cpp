#include "Bootstrap.h"
#include "InboundHandlerAdapter.h"
#include "OutboundHandlerAdapter.h"
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

	virtual ~ServerHandler() {
		DEBUG << getName() << " Destructor ...";
	}

	virtual void onActive(const ChannelPtr& channel_ptr) override {

	}

	virtual void onReceive(const ChannelPtr& channel_ptr, void* object) override {
		DEBUG << "............. OK .............";
	
		char response[] = "Response ....\n";
		channel_ptr->getChain()->send(channel_ptr, response, strlen(response));
		channel_ptr->getChain()->shutdownSend(channel_ptr);

		//channel_ptr->getChain()->shutDown();
	}

	void onInactive(const ChannelPtr& channel_ptr) override {
		DEBUG << "............. " << channel_ptr->isOpen();
		DEBUG << "Use Count = > " << channel_ptr.use_count();
		channel_ptr->getChain()->close(channel_ptr);
	}

};

class DefaultHandler : public OutboundHandlerAdapter {
public:
	DefaultHandler() {
		DEBUG << "DefaultHandler Constructor ...";
		setName("DefaultHandler");
	}

	~DefaultHandler() {
		DEBUG << getName() << " Destructor ...";
	}

	virtual void send(const ChannelPtr& channel_ptr, char* data, int len) override {
		DEBUG << "DefaultHandler Send";
		getNext()->send(channel_ptr, data, len);
	}

};





class Server {
public:
	Server(const std::vector<IpAddress*>& listen_address_list, const std::vector<IpAddress*>& connect_address_list, 
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

	void addInHandler(AbstractInboundHandler* handler) {
		bootstrap_.setChainFactory([] (const ChannelPtr& channel_ptr) {

			HandlerChain* chain = new HandlerChain();
			ServerHandler* handler = new ServerHandler;
			DefaultHandler* default_handler = new DefaultHandler();
			chain->addInHandler(handler);
			chain->addOutHandler(default_handler);
			chain->addOutHandler(default_handler);

			return chain;
		});
	}

	void start() {
		bootstrap_.start();
		bootstrap_.join();	
	}

private:
	Bootstrap 	 bootstrap_;

};


// take case main function clear objects.
int main() {
	IpAddress listen_address("127.0.0.1", 8090);

	std::vector<IpAddress*> listen_address_list;
	listen_address_list.push_back(&listen_address);
	listen_address_list.push_back(&listen_address);
	std::vector<IpAddress*> connect_address_list;

	//connectAddrList.push_back(&connectAddr);
	//ServerHandler* handler = new ServerHandler();
	std::shared_ptr<ServerHandler> handler(new ServerHandler);

	Server* server = new Server(listen_address_list, connect_address_list, 2, 5, 2);



	server->addInHandler(handler.get());

	server->start();
	DEBUG << "STOP";
	return 0;
}



