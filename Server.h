#pragma once
#include "Socket.h"
#include "EventLoop.h"
#include "Acceptor.h"
#include "Connection.h"
#include <map>

class Acceptor;
class EventLoop;
class Server
{
private:
    EventLoop* loop;
    Acceptor* apt;
    std::map<int, Connection*> connections;

public:
    Server(EventLoop*);
    ~Server();

    void new_connection(Socket*);
    void delete_new_connection(Socket*);
};