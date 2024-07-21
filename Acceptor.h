#pragma once
#include "Socket.h"
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
    Channel* chl;
    EventLoop* elp;
    std::function<void(Socket*)> new_connection_callback;

public:
    Acceptor(EventLoop*);
    ~Acceptor();
    void accept_connection();
    void set_new_connection_callback(std::function<void(Socket*)>);  
};