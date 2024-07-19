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
#include "EventLoop.h"
#include "Server.h"



int main()
{
    EventLoop* elp = new EventLoop();
    Server* server = new Server(elp);
    elp->loop();
    delete server;
    delete elp;
    return 0;
}
