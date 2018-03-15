#include "TCPServer.h" 

struct message TCPServer::MSG;
//arg

void* TCPServer::Task(void *arg)
{
	int n;
    int newsockfd = (long)arg;

    struct message msg;
	pthread_detach(pthread_self());
	while(1)
	{        
        n=recv(newsockfd,reinterpret_cast<char*>(&MSG),sizeof(MSG),O_CLOEXEC);
		if(n==0)
		{
		   close(newsockfd);
		   break;
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
    struct message *msg;
    int length;
	while(1)
	{
		socklen_t sosize  = sizeof(clientAddress);
		newsockfd = accept(sockfd,(struct sockaddr*)&clientAddress,&sosize);

        str = inet_ntoa(clientAddress.sin_addr);
        length = str.length();

        std::cout << "length="<<length;
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
