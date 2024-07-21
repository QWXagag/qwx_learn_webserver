#include "EventLoop.h"


EventLoop::EventLoop() : ep(nullptr), quit(false)
{
    ep = new Epoll();
}

EventLoop::~EventLoop()
{
    if (!quit)
    {
        quit = true;
    }
    delete ep;
}


void EventLoop::loop()
{
    while (!quit)
    {
        std::vector<Channel*> chls;
        chls = ep->poll();
        for (auto it = chls.begin(); it != chls.end(); it++)
        {
            (*it)->handle_event();
        }
    }
}

void EventLoop::update_channel(Channel* chl)
{
    ep->update_channel(chl);
}
