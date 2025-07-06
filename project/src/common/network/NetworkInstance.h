#pragma once

class NetworkInstance
{
public:
    typedef struct 
    {
        unsigned char d1;
        unsigned char d2;
        unsigned char d3;
        unsigned char d4;
    } IP;
    IP IpAddress;
    unsigned short Port;

    NetworkInstance() : IpAddress{ 0, 0, 0, 0 }, Port(0) {}
    NetworkInstance(unsigned char d1, unsigned char d2, unsigned char d3, unsigned char d4, unsigned short port)
        : IpAddress{ d1, d2, d3, d4 }, Port(port) {
    }

};