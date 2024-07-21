#include "Acceptor.h"



Acceptor::Acceptor(EventLoop* _elp) : elp(_elp), chl(nullptr)
{
    serv = new Socket();
    InetAddress* serv_addr = new InetAddress("127.0.0.1", 8888);
    serv->bind(serv_addr);
    serv->listen();
    serv->setnonblocking();

    chl = new Channel(elp, serv->get_fd());
    std::function<void()> cb = std::bind(&Acceptor::accept_connection, this);
    chl->set_callback(cb);
    chl->enable_reading();
}

Acceptor::~Acceptor()
{
    delete serv;
    delete chl;
}

void Acceptor::accept_connection()
{
    InetAddress* cli_addr = new InetAddress();
    Socket* cli = new Socket(serv->accept(cli_addr));
    printf("new client fd %d! IP: %s Port: %d\n",
    cli->get_fd(),
    inet_ntoa(cli_addr->addr.sin_addr),
    ntohs(cli_addr->addr.sin_port));
    cli->setnonblocking();

    new_connection_callback(cli);
}

void Acceptor::set_new_connection_callback(std::function<void(Socket*)> _cb)
{
    new_connection_callback = _cb;
}
