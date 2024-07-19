#include "Connection.h"
#include "InetAddress.h"
#include "Socket.h"
#include <functional>
#include <string.h>
#include <unistd.h>


#define READ_BUF_SIZE 1024


Connection::Connection(EventLoop* _elp, Socket* _sock) : elp(_elp), sock(_sock), chl(nullptr)
{
    chl = new Channel(elp, sock->get_fd());
    std::function<void()> cb = std::bind(&Connection::echo, this, chl->get_fd());
    chl->set_callback(cb);

    chl->enable_reading();
}

Connection::~Connection()
{
    delete chl;
    delete sock;
}

void Connection::echo(int _fd)
{
    char buf[READ_BUF_SIZE];
    while (true)
    {
        bzero(buf, sizeof(buf));
        ssize_t read_bytes = read(_fd, buf, sizeof(buf));
        if (read_bytes > 0)
        {
            printf("message from client fd %d: %s\n", _fd, buf);
            write(_fd, buf, read_bytes);
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
            printf("finish reading once, errno: %d\n", errno);
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
