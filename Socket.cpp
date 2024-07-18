#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "Socket.h"
#include "util.h"


// 无参构造
Socket::Socket() : fd(-1)
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd == -1, "socket create err");
}

// 有参构造
Socket::Socket(int _fd) : fd(_fd)
{
    errif(fd == -1, "socket create err");
} 

// 析构
Socket::~Socket()
{
    if (fd != -1)
    {
        close(fd);
        fd = -1;
    }
}


void Socket::bind(InetAddress* addr)
{
    errif(::bind(fd, (struct sockaddr*)&addr->addr, addr->addr_len) == -1, "socket bind err");
}

void Socket::listen()
{
    errif(::listen(fd, SOMAXCONN) == -1, "socket listen err");
}

void Socket::setnonblocking()
{
    printf("fd=%d\n", fd);
    int flag = fcntl(fd, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flag);
}

int Socket::accept(InetAddress* addr)
{
    int cfd = ::accept(fd, (sockaddr*)&addr->addr, &addr->addr_len);
    errif(cfd == -1, "socket accept err");
    return cfd;
}

int Socket::get_fd()
{
    return fd;
}