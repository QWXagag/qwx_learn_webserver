#pragma once
#include "Epoll.h"
#include "Channel.h"

class Epoll;
class Channel;
class EventLoop
{
private:
    Epoll* ep;
    bool quit;
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void update_channel(Channel*);
};