#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
using namespace std;

#define MAXPACKETSIZE 4096

//struct message {
//    uint32_t magic = 0x53545259;
//    uint16_t payload;//length of a data
//    uint16_t status_code;
//    uint8_t *data;//fix
//}__attribute__((packed));

struct message {
    uint32_t magic = 0x53545259;
    uint16_t payload;//length of a data
    uint16_t status_code;
    std::string data;
}__attribute__((packed));

//struct message {
//    uint32_t magic = 0x53545259;
//    uint16_t payload;
//    uint16_t status_code;
//    uint8_t data[];
//}__attribute__((packed));


class TCPServer
{
	public:
	int sockfd, newsockfd, n, pid;
	struct sockaddr_in serverAddress;
	struct sockaddr_in clientAddress;
	pthread_t serverThread;
	char msg[ MAXPACKETSIZE ];
//	static string Message;

    static struct message MSG;

	void setup(int port);
	string receive();
	string getMessage();
	void Send(string msg);
	void detach();
	void clean();

    message getMSG();

private:
    static void * Task(void * argv);
};

#endif
