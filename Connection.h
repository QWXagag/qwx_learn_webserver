#pragma once
#include "Socket.h"
#include "EventLoop.h"
#include "Channel.h"
#include "Buffer.h"


class Connection
{
private:
    Socket* sock;
    EventLoop* elp;
    Channel* chl;
    std::function<void(Socket*)> delete_connection_callback;
    Buffer* read_buf;

public:
    Connection(EventLoop*, Socket*);
    ~Connection();
    void echo(int fd);
    void set_delete_connection_callback(std::function<void(Socket*)>);
};