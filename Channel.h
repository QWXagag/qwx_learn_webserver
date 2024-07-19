#pragma once
#include "Epoll.h"
#include "EventLoop.h"
#include <functional>

class EventLoop;
class Channel
{
private:
    EventLoop* elp;
    int fd;
    // 需要监听的事件
    uint32_t event;
    // 发生变化的事件
    uint32_t revent;
    // 该Channel是否已经上epoll树
    bool in_epoll;
    // 回调函数
    std::function<void()> callback;



public:
    Channel(EventLoop* elp, int _fd);
    ~Channel();

    void enable_reading();
    int get_fd();
    uint32_t get_events();
    uint32_t get_revents();
    bool get_in_poll();
    void set_in_poll();

    void set_revents(uint32_t op);
    void set_callback(std::function<void()>);
    void handle_event();
};