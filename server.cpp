#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include "util.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Epoll.h"


#define READ_BUF_SIZE 1024


void handle_read_event(int);

int main()
{

    Socket* serv = new Socket();
    InetAddress* serv_addr = new InetAddress("127.0.0.1", 8888);
    printf("test\n");
    serv->bind(serv_addr);
    printf("test\n");
    serv->listen();

    Epoll* ep = new Epoll();
    serv->setnonblocking();
    ep->add_fd(serv->get_fd(), EPOLLIN | EPOLLET);

    while (true)
    {
        std::vector<epoll_event> events = ep->poll();
        for (int i = 0; i < events.size(); i++)
        {
            if (events[i].data.fd == serv->get_fd())
            {
                InetAddress* cli_addr = new InetAddress();
                Socket* cli = new Socket(serv->accept(cli_addr));
                printf("new client fd %d! IP: %s Port: %d\n",
                cli->get_fd(),
                inet_ntoa(cli_addr->addr.sin_addr),
                ntohs(cli_addr->addr.sin_port));

                cli->setnonblocking();
                ep->add_fd(cli->get_fd(), EPOLLIN | EPOLLET);
            }
            else if (events[i].events & EPOLLIN)
            {
                handle_read_event(events[i].data.fd);
            }
            else
            {
                printf("something eles happended\n");
            }
        }
    }
    delete serv;
    delete serv_addr;
    return 0;
}



void handle_read_event(int fd)
{
    char buf[READ_BUF_SIZE];
    while (true)
    {
        bzero(buf, sizeof(buf));
        ssize_t read_bytes = read(fd, buf, sizeof(buf));
        if (read_bytes > 0)
        {
            printf("message from client fd %d: %s\n", fd, buf);
            write(fd, buf, read_bytes);
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
            printf("EOF, client fd %d disconnected\n", fd);
            close(fd);
            break;
        }
    }
}