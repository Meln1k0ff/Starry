#include "TCPServer.h" 

struct message TCPServer::MSG;

void * TCPServer::Task(void *arg)
{
    int n = 1;
    int newsockfd = (long)arg;
    uint16_t status;
    int payload_length;

    uint8_t header[8];

    while(n > 0)
    {
        pthread_detach(pthread_self());
        n=recv(newsockfd,header,8,0);//read our header
        payload_length = header[6]*8 + header[7];
        status = header[4]*8 + header[5];

        if(n<=0)
        {
           close(newsockfd);
        }

        void* str_buf = malloc(payload_length);
        n=recv(newsockfd,str_buf,payload_length,O_CLOEXEC);

        if(n<=0)
        {
           close(newsockfd);
        }

    }
    return 0;
}

message TCPServer::getMSG()
{
    return MSG;
}
//sets up server.
void TCPServer::setup(int port)
{
    sockfd=socket(AF_INET,SOCK_STREAM,0);
 	memset(&serverAddress,0,sizeof(serverAddress));
	serverAddress.sin_family=AF_INET;
	serverAddress.sin_addr.s_addr=htonl(INADDR_ANY);
	serverAddress.sin_port=htons(port);
	bind(sockfd,(struct sockaddr *)&serverAddress, sizeof(serverAddress));
 	listen(sockfd,5);
}
//receive is called after Task have done the actual receive and parsing of an incoming packet
string TCPServer::receive()
{
	string str;

	while(1)
	{
		socklen_t sosize  = sizeof(clientAddress);
		newsockfd = accept(sockfd,(struct sockaddr*)&clientAddress,&sosize);

        str = inet_ntoa(clientAddress.sin_addr);             
		pthread_create(&serverThread,NULL,&Task,(void *)newsockfd);
	}
	return str;
}


//only string response
void TCPServer::Send(string msg)
{
	send(newsockfd,msg.c_str(),msg.length(),0);
}

void TCPServer::clean()
{
//	Message = "";
//	memset(msg, 0, MAXPACKETSIZE);
}

void TCPServer::detach()
{
	close(sockfd);
	close(newsockfd);
} 
