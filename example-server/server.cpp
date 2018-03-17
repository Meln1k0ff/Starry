#include <iostream>
#include "TCPServer.h"
#include <string>
#include <sstream>
#include <cstdlib>

TCPServer tcp;

void * loop(void * m)
{
    std::cout << "server loop" << std::endl;
    std::cout.flush();
    while(1)
    {
        //?
        //received packet
        //incoming empty message. we need to put into MSG first
        struct message msg = tcp.getMSG();//message that was received via receive() function
        struct stats_t stats;// = tcp.getSTATS();
        std::string ping = "Server is Alive";
        std::string reset = "Stats were reset";
        std::cout << "server get message - " << msg.status_code<<std::endl;
        std::cout.flush();
        //notify main thread when status code is checked
        switch (msg.status_code){
        case 1:
            std::cout << "Ping"<<std::endl;
            tcp.Send(ping);
            //response OK
            break;
        case 2:
            //get stats

            stats.recv_bytes = recv_cnt;
            stats.sent_bytes = sent_cnt;
            stats.compr_ratio = 20;
            std::cout << "Stats="<< stats.compr_ratio <<std::endl;
            std::cout.flush();
            tcp.Send(&stats);
            break;
        case 3:
            //reset stats
            recv_cnt = 0;
            sent_cnt = 0;

            tcp.Send(reset);
            //clear vector
            break;
        case 4:
            tcp.Send(&msg);
            break;
        }
        usleep(10000);
        break;
    }
    pthread_detach(pthread_self());
    tcp.detach();
}

int main(int argc,char *argv)
{
	pthread_t msg;
    tcp.setup(11999);//from cmd 4000 port
	if( pthread_create(&msg, NULL, loop, (void *)0) == 0)
	{
		tcp.receive();
	}
	return 0;
}
