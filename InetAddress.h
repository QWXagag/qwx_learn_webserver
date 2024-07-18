#pragma once
#include <arpa/inet.h>


class InetAddress
{
public:
    struct sockaddr_in addr;
    socklen_t addr_len;
    InetAddress();
    InetAddress(const char*, u_int16_t);
    ~InetAddress();
};