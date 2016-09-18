#include "protobuf/Im.Query.pb.h"
#include "Bootstrap.h"
#include "ProtobufHandler.h"
#include "AbstractInboundHandler.h"
#include "Thread.h"
#include "Handler.h"
#include "protobuf/Im.Query.pb.h"
#include <google/protobuf/message.h>
#include "Log.h"
#include "HttpHandler.h"

using namespace mars;
using namespace Im;

class DefaultHandler : public ProtobufHandler {
public:
	DefaultHandler() { DEBUG << "DefaultHandler Constructor ..."; }
	~DefaultHandler() { DEBUG << " Destructor ..."; }

	virtual void onInactive(const ChannelPtr& channel_ptr) override {
		channel_ptr->getChain()->close(channel_ptr);
	}
	virtual void onMessage(const ChannelPtr& channel_ptr, void* message) override {
		Query* query = (Query*)message;
        int type_name = query->type_name();
        int len       = query->len();
        DEBUG << "type name " << query->type_name();
        DEBUG << "len " << query->len();

        DEBUG << "Message size " << query->ByteSize() << " Message name " << query->GetTypeName();

        query->set_type_name(++type_name);
        query->set_len(++len);


        DEBUG << "Message size " << query->ByteSize() << " Message name " << query->GetTypeName();

        sendMessage(query, channel_ptr);

        query->Clear();
        delete query;
	}
	virtual const std::string getName() override {
		return "DefaultHandler";
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
            if ((channel_ptr->getLocalAddress().port() == 8080)) {
                HandlerChain* chain = new HandlerChain();
                chain->addInHandler(new HttpHandler());
                return chain;
            } else {
                HandlerChain* chain = new HandlerChain();
                DefaultHandler* default_handler = new DefaultHandler();
                chain->addInHandler(default_handler);
                chain->addOutHandler(default_handler);
                return chain;
            }
		});

		bootstrap_.start();
		bootstrap_.join();
	}

private:
	Bootstrap bootstrap_;
};

int main() {
	Query query;

	std::vector<IpAddress> listen_address_list;
	std::vector<IpAddress> connect_address_list;
	IpAddress listen_address_a("127.0.0.1", 8090);
	IpAddress listen_address_b("192.168.1.101", 8080);
	listen_address_list.push_back(listen_address_a);
	listen_address_list.push_back(listen_address_b);

	Server* server = new Server(listen_address_list, connect_address_list, 2, 2, 1);

	server->start();
	DEBUG << "STOP";

	return 0;
}
