#include "TCPClient.h"

TCPClient::TCPClient()
{
	sock = -1;
	port = 0;
	address = "";
}

bool TCPClient::setup(string address , int port)
{
  	if(sock == -1)
	{
		sock = socket(AF_INET , SOCK_STREAM , 0);
		if (sock == -1)
		{
      			cout << "Could not create socket" << endl;
    		}
        }
  	if(inet_addr(address.c_str()) == -1)
  	{
    		struct hostent *he;
    		struct in_addr **addr_list;
    		if ( (he = gethostbyname( address.c_str() ) ) == NULL)
    		{
		      herror("gethostbyname");
      		      cout<<"Failed to resolve hostname\n";
		      return false;
    		}
	   	addr_list = (struct in_addr **) he->h_addr_list;
    		for(int i = 0; addr_list[i] != NULL; i++)
    		{
      		      server.sin_addr = *addr_list[i];
		      break;
    		}
  	}
  	else
  	{
    		server.sin_addr.s_addr = inet_addr( address.c_str() );
  	}
  	server.sin_family = AF_INET;
  	server.sin_port = htons( port );
  	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
  	{
    		perror("connect failed. Error");
    		return false;
  	}
  	return true;
}

bool TCPClient::Send(struct message msg )
{
    uint8_t *buffer = (uint8_t *) malloc(msg.payload + 8);//allocate memory for a string
    uint8_t *p = (uint8_t*) buffer;

    memcpy(p,&msg.magic,4);

    memcpy(p+4,&msg.payload,2);
    memcpy(p+6,&msg.status_code,2);

   if (msg.data != NULL){
        std::cout << "some string is here"<<std::endl;
        memcpy(p+8,msg.data,msg.payload);
   }

    if(sock != -1)
    {
        std::cout << "SENDING" << std::endl;
        std::cout << "msg size=" << sizeof(msg) <<std::endl;

        //check if message has actual string to compress

        if( send(sock , (uint8_t *)buffer , msg.payload + 8, 0) < 0)
        {
            cout << "Send failed : " << std::hex<< msg.magic << endl;
            return false;
        }
    }
    else
        return false;
    return true;

}

string TCPClient::receive(int size)
{
  	char buffer[size];
	memset(&buffer[0], 0, sizeof(buffer));

  	string reply;
	if( recv(sock , buffer , size, 0) < 0)
  	{	    	
		return nullptr;
  	}
	buffer[size-1]='\0';
  	reply = buffer;
  	return reply;
}

string TCPClient::read()
{
    char buffer[1] = {};
    string reply;
    while (buffer[0] != '\n') {
            if( recv(sock , buffer , sizeof(buffer) , 0) < 0)
            {
                cout << "receive failed!" << endl;
            return nullptr;
            }
        reply += buffer[0];
    }
    return reply;
}

void TCPClient::exit()
{
    close( sock );
}
