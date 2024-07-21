#include <functional>
#include "Server.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Epoll.h"
#include "EventLoop.h"
#include "Connection.h"
#include <string.h>
#include <unistd.h>
#include <iostream>


#define READ_BUF_SIZE 1024

Server::Server(EventLoop* _main_reactor) : main_reactor(_main_reactor), apt(nullptr), thread_poll(nullptr)
{
    apt = new Acceptor(main_reactor);
    std::function<void(Socket*)> cb = std::bind(&Server::new_connection, this, std::placeholders::_1);
    apt->set_new_connection_callback(cb);

    int size = std::thread::hardware_concurrency();
    thread_poll = new Thread(size);
    for (int i = 0; i < size; i++)
    {
        sub_reactor.push_back(new EventLoop());
    }

    for (int i = 0; i < size; i++)
    {
        std::function<void()> sub_loop = std::bind(&EventLoop::loop, sub_reactor[i]);
        thread_poll->add(sub_loop);
    }
    
    
}

Server::~Server()
{
    delete apt;
}


void Server::new_connection(Socket* sock)
{
    int random = sock->get_fd() % sub_reactor.size();
    Connection* conn = new Connection(sub_reactor[random], sock);
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


