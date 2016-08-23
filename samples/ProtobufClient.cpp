#include "protobuf/Im.Query.pb.h"
#include <iostream>
#include "Bootstrap.h"
#include "ProtobufHandler.h"

#include "AbstractInboundHandler.h"
#include "Thread.h"
#include "Handler.h"
//#include "Server.h" // uncomment this line will happen some thing.
#include <google/protobuf/message.h>
#include "Log.h"
#include "Channel.h"
#include <functional>
#include <memory>

using namespace std;
using namespace mars;
using namespace Im;


class DefaultHandler : public ProtobufHandler {
public:
    DefaultHandler() { DEBUG << "DefaultHandler Constructor ..."; }
    ~DefaultHandler() { DEBUG << " Destructor ..."; }

    virtual void onActive(const ChannelPtr& channel_ptr) override {  
        int a;
        cin >> a;
        if (a == 0) return;
        Query* query = new Query();
        query->set_type_name(100);
        query->set_len(10);
        sendMessage(query, channel_ptr);
        DEBUG << "type_name " << query->type_name();
        DEBUG << "len " << query->len();
    }
    virtual void onMessage(const ChannelPtr& channel_ptr, void* message) override {
        Query* query = (Query*)message;
        int type_name = query->type_name();
        int len       = query->len();
        DEBUG << "type name " << query->type_name();
        DEBUG << "len " << query->len();
        query->set_type_name(++type_name);
        query->set_len(++len);
        sendMessage(query, channel_ptr);
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
            chain->addInHandler(new DefaultHandler());

            return chain;
        });
        
        bootstrap_.start();
        bootstrap_.join();  
    }

private:
    Bootstrap bootstrap_;
};

int main() {
    std::vector<IpAddress> listen_address_list;
    std::vector<IpAddress> connect_address_list;
    IpAddress listen_address_a("127.0.0.1", 8090);
    connect_address_list.push_back(listen_address_a);

    Server* server = new Server(listen_address_list, connect_address_list, 2, 2, 1);

    server->start();
    DEBUG << "STOP";
    return 0;
}

