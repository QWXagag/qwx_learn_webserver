#pragma once
#include <sys/epoll.h>
#include <vector>
#include "Channel.h"


class Channel;
class Epoll
{
private:
    int epfd;
    struct epoll_event* events;

public:
    Epoll();
    ~Epoll();
    // void add_fd(int fd, uint32_t op);
    std::vector<Channel*> poll(int timeout = -1);
    void update_channel(Channel* chl);
};