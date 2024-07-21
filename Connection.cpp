#include "Connection.h"
#include "InetAddress.h"
#include "Socket.h"
#include "util.h"
#include "Thread.h"
#include <functional>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>




#define READ_BUF_SIZE 1024


Connection::Connection(EventLoop* _elp, Socket* _sock) : elp(_elp), sock(_sock), chl(nullptr), read_buf(nullptr)
{
    chl = new Channel(elp, sock->get_fd());
    std::function<void()> cb = std::bind(&Connection::echo, this, chl->get_fd());
    chl->set_callback(cb);
    chl->enable_reading();
    read_buf = new Buffer();
}

Connection::~Connection()
{
    delete chl;
    delete sock;
    delete read_buf;
}

void Connection::echo(int _fd)
{   
    // 在ehco中打印线程id
    std::cout << "Connection::echo, thread_id:" << std::this_thread::get_id() << std::endl;
    char buf[READ_BUF_SIZE];
    while (true)
    {
        bzero(buf, sizeof(buf));
        ssize_t read_bytes = read(_fd, buf, sizeof(buf));
        if (read_bytes > 0)
        {
            read_buf->append(buf, read_bytes);
        }
        // 被系统信号打断
        else if (read_bytes == -1 && errno == EINTR)
        {
            printf("continue reading\n");
            continue;
        }
        // 所有信息被读取完
        else if (read_bytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
        {
            printf("message from client fd %d: %s\n", _fd, read_buf->c_str());
            printf("finish reading once, errno: %d\n", errno);
            errif(write(_fd, read_buf->c_str(), read_buf->size()) == -1, "socket write err");
            read_buf->clear();
            break;
        }
        // 客户端关闭
        else if (read_bytes == 0)
        {
            printf("EOF, client fd %d disconnected\n", _fd);
            delete_connection_callback(sock);
            break;
        }
    }
}


void Connection::set_delete_connection_callback(std::function<void(Socket*)> _cb)
{
    delete_connection_callback = _cb;
}
