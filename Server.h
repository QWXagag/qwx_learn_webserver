#pragma once
#include "Socket.h"
#include "EventLoop.h"
#include "Acceptor.h"
#include "Connection.h"
#include "Thread.h"
#include <map>

class Acceptor;
class EventLoop;
class Server
{
private:
    EventLoop* main_reactor;
    std::vector<EventLoop*> sub_reactor;
    Acceptor* apt;
    std::map<int, Connection*> connections;
    Thread* thread_poll;
public:
    Server(EventLoop*);
    ~Server();

    void new_connection(Socket*);
    void delete_new_connection(Socket*);
};