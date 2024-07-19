#include <functional>
#include "Server.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Epoll.h"
#include "EventLoop.h"
#include "Connection.h"
#include <string.h>
#include <unistd.h>


#define READ_BUF_SIZE 1024

Server::Server(EventLoop* _loop) : loop(_loop), apt(nullptr)
{
    apt = new Acceptor(loop);
    std::function<void(Socket*)> cb = std::bind(&Server::new_connection, this, std::placeholders::_1);
    apt->set_new_connection_callback(cb);
}

Server::~Server()
{
    delete apt;
}


void Server::new_connection(Socket* sock)
{
    Connection* conn = new Connection(loop, sock);
    connections[sock->get_fd()] = conn;

    std::function<void(Socket*)> cb = std::bind(&Server::delete_new_connection, this, std::placeholders::_1);
    conn->set_delete_connection_callback(cb);
}

void Server::delete_new_connection(Socket* sock)
{
    Connection* conn = connections[sock->get_fd()];
    connections.erase(sock->get_fd());
    delete conn;
}


