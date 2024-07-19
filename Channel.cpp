#include "Channel.h"


Channel::Channel(EventLoop* _elp, int _fd) : elp(_elp), fd(_fd), event(0), revent(0), in_epoll(false)
{

}

Channel::~Channel()
{

}

void Channel::enable_reading()
{
    event = EPOLLIN | EPOLLET;
    elp->update_channel(this);
}

int Channel::get_fd()
{
    return fd;
}

uint32_t Channel::get_events()
{
    return event;
}

uint32_t Channel::get_revents()
{
    return revent;
}

bool Channel::get_in_poll()
{
    return in_epoll;
}

void Channel::set_in_poll()
{
    in_epoll = true;
}

void Channel::set_revents(uint32_t _op)
{
    revent = _op;
}


void Channel::set_callback(std::function<void()> _cb)
{
    callback = _cb;
}

void Channel::handle_event()
{
    callback();
}



