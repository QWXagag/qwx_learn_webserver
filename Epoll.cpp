#include "Epoll.h"
#include "util.h"
#include <string.h>
#include <unistd.h>


#define MAX_EVENTS 1000

Epoll::Epoll() : epfd(-1), events(nullptr)
{
    epfd = epoll_create(1);
    errif(epfd == -1, "epoll create err");
    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof(*events) * MAX_EVENTS);
}

Epoll::~Epoll()
{
    if (epfd != -1)
    {
        close(epfd);
        epfd = -1;
    }
    delete[] events;
}

std::vector<Channel*> Epoll::poll(int timeout)
{
    std::vector<Channel*> active_channel;
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    errif(nfds == -1, "epoll_wait err");
    for (int i = 0; i < nfds; i++)
    {
        Channel *chl = (Channel*)events[i].data.ptr;
        chl->set_revents(events[i].events);
        active_channel.push_back(chl);
    }
    return active_channel;
}

void Epoll::update_channel(Channel* chl)
{
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = chl;
    ev.events = chl->get_events();

    if (chl->get_in_poll())
    {
        errif(epoll_ctl(epfd, EPOLL_CTL_MOD, chl->get_fd(), &ev) == -1, "update channel epoll_mod err");
    }
    else
    {
        errif(epoll_ctl(epfd, EPOLL_CTL_ADD, chl->get_fd(), &ev) == -1, "update channel epoll_add err");
        chl->set_in_poll();
    }
}