#pragma once
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "EventLoop.h"


class Socket;
class InetAddress;
class Channel;
class EventLoop;

class Acceptor
{
private:
    Socket* serv;
    InetAddress* serv_addr;
    Channel* chl;
    EventLoop* elp;

public:
    Acceptor(EventLoop*);
    ~Acceptor();
    void accept_connection();

    std::function<void(Socket*)> new_connection_callback;
    void set_new_connection_callback(std::function<void(Socket*)>);  
};