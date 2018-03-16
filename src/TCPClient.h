#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netdb.h> 
#include <vector>

using namespace std;

struct message {
    uint32_t magic = 0x53545259;
    uint16_t payload;
    uint16_t status_code;
    char *data;
}__attribute__((packed));

//struct message {
//    uint32_t magic = 0x53545259;
//    uint16_t payload;
//    uint16_t status_code;
//    std::string data;
//}__attribute__((packed));

//struct message {
//    uint32_t magic = 0x53545259;
//    uint16_t payload;
//    uint16_t status_code;
//    uint8_t data[];
//}__attribute__((packed));


class TCPClient
{
  private:
    int sock;
    std::string address;
    int port;
    struct sockaddr_in server;

  public:
    TCPClient();
    bool setup(string address, int port);
//    bool Send(string data);
    bool Send(message msg);
    string receive(int size = 4096);
    string read();
    void exit();
};

#endif
