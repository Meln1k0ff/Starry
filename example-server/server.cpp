#include <iostream>
#include "TCPServer.h"
#include <string>
#include <sstream>
#include <cstdlib>

TCPServer tcp;

std::vector<std::string> stats_table;//has all our compressed strings

std::string length_encoding(const std::string& s)
{
    char c = ' ';
    int num = 0;
    std::string result;
    std::string::const_iterator it = s.begin();
    for(; it != s.end(); ++it)
    {
        if(*it!=c)
        {
            if(num!=0)
            {
                std::stringstream ss;
                ss << num;
                std::string num_s(ss.str());
                result += num_s;
            }

            c = *it;
            result.push_back(c);

            num = 1;
        }
        else
        {
            num++;
        }
    }

    std::stringstream ss;
    ss << num;
    std::string num_s(ss.str());
    result += num_s;

    return result;
}

void * loop(void * m)
{
    pthread_detach(pthread_self());
	while(1)
	{
        std::string lst;
        //?
        //received packet
        //incoming empty message. we need to put into MSG first
        struct message msg = tcp.getMSG();//message that was received via receive() function
        //parse code and magic number

        if (msg.status_code == 1){

            //error. leave            
        }

            //error. leave
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
            break;
        case 4:
            //compress
            //get data using payload
            break;
        }
//        if( str != "" )
//		{
//            int request = atoi(str.c_str());
//            switch (request){
//            case 1:
//                std::cout << "Ping"<<std::endl;
//                lst = "aaaaabbaaaa";
//                //send 0 if ok
//                tcp.Send(std::to_string(0));//send ping
////                stats_table.push_back(lst); // request stats in another thread
//                break;
//            case 2:
//                std::cout << "stats"<<std::endl;
//                tcp.Send(std::to_string(0));
//                break;
//            case 3:
//                std::cout << "remove Stats"<<std::endl;
//                tcp.Send("");
//                break;
//            case 4:
//                std::cout << "Compress"<<std::endl;
//                length_encoding(lst);
//                tcp.Send(" [client message: Reset Stats]");
//                break;
//            default:
//                std::cout << "Wrong request!"<<std::endl;
//            }

////			tcp.Send(" [client message: "+str+"] "+s);

//			tcp.clean();
//		}
		usleep(1000);
	}
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
