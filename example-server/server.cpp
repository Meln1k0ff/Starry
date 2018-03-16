#include <iostream>
#include "TCPServer.h"
#include <string>
#include <sstream>
#include <cstdlib>

TCPServer tcp;

void * loop(void * m)
{
    pthread_detach(pthread_self());
	while(1)
	{      
        //?
        //received packet
        //incoming empty message. we need to put into MSG first
        struct message msg = tcp.getMSG();//message that was received via receive() function

        //notify main thread when status code is checked
        switch (msg.status_code){
        case 1:
            std::cout << "Ping"<<std::endl;

            tcp.Send("I am Alive");//
//                stats_table.push_back(lst); // request stats in another thread
            break;
        case 2:
            //get stats
            //serialize vector

            break;
        case 3:
            //reset stats
             tcp.Send("Reset Stats");
            //clear vector

            break;
        case 4:
            //compress
            //get data using payload
            break;
        }
//		usleep(1000);
//        break;
	}
	tcp.detach();
}

int main(int argc,char *argv)
{
	pthread_t msg;
    tcp.setup(4000);//from cmd 4000 port
	if( pthread_create(&msg, NULL, loop, (void *)0) == 0)
	{
		tcp.receive();
	}
	return 0;
}
