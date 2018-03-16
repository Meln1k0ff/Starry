#include <iostream>
#include <algorithm>
#include <signal.h>
#include <cstdint>
#include <string>
#include "TCPClient.h"

#define BUFSIZE 32768

TCPClient tcp;
char data[32768];

void sig_exit(int s)
{
	tcp.exit();
	exit(0);
}

void init_message(int rq,struct message msg,int payload,const char * data){
    switch (rq) {
    case 1:
        //ping
        msg.status_code = rq;
        msg.payload = 0;
        msg.data = 0;
        break;
    case 2:
        //get stats
        msg.status_code = rq;
        msg.payload = 0;
        msg.data = 0;
        break;
    case 3:
        //reset stats
        msg.status_code = rq;
        msg.payload = 0;
        msg.data = 0;
        break;
    case 4:
        //compress
        msg.status_code = rq;
        msg.payload = payload;

        memcpy(msg.data,data,strlen(data)+1);
        break;
    }
}
//char data - our string, length of string is a payload
//void serialize(struct message* msgPacket, char *data)
//{
//    uint32_t *q = (uint32_t*)data;

//    *q = msgPacket->magic;       q++;
//    *q = msgPacket->payload;
//    *q = *q << 16 + msgPacket->status_code;//
////    *q = msgPacket->status_code;     q++;

//    uint32_t *p = (uint32_t*)q;
//    int i = 0;
//    while (i < msgPacket->payload)
//    {
////        *p = *msgPacket->data;
//        p++;
//        i++;
//    }
//}

//void deserialize(char *data, struct message* msgPacket)
//{
//    int *q = (int*)data;
//    msgPacket->magic = *q;       q++;
//    msgPacket->payload = *q;   q++;
//    msgPacket->status_code = *q;     q++;

//    uint8_t *p = (uint8_t*)q;
//    int i = 0;
//    while (i < BUFSIZE)
//    {
//        msgPacket->data[i] = *p;
//        p++;
//        i++;
//    }
//}


int main(int argc, char *argv[])
{
	signal(SIGINT, sig_exit);
    std::string request;
    std::string str;
    if (argc < 2){
        //after connect
        //show help
        std::cout << "starry client" << std::endl;
        std::cout << "usage ./starry --ping or -p , --compress or -c,    --stats or -s --help or -h"<< std::endl;
    }
    else {
    //todo - not only for localhost. set address at start
    int port = atoi(argv[1]);
    tcp.setup("127.0.0.1",port);
    int rq = 0;
    struct message msg;
	while(1)
	{
        std::getline(std::cin, request);

        //get arg here
        std::cout << request <<std::endl;

        std::string space = " ";//delimiter

        std::string req= request.substr(0, request.find(space));
        std::cout << req <<std::endl;

        str = request.substr(request.find(" ") + 1);

        std::cout << str<<std::endl;

        //simple ping. Response is "OK"
        if ((request == "-p") || (request == "--ping")){
                std::cout << "ping";
                //send 1 to server
                rq = 1;
                init_message(rq,msg,0,0);
                tcp.Send(msg);
        }

        if ((request == "-s") || (request == "--stats")){
                std::cout << "stats";
                //send 3 to server
                rq = 2;
                init_message(rq,msg,0,0);
                tcp.Send(msg);
        }

        if ((request == "-h") || (request == "--help")){
                std::cout << "help";
                //send 3 to server
                rq = 3;
                init_message(rq,msg,0,0);
                tcp.Send(msg);
        }
        if ((req == "-c") || (req == "--compress")){
               //divide on substrings
                std::cout << "under request" << std::endl;

                std::cout << "compr=" << str<<endl;

                //validate string

                if( ( !(std::all_of( str.begin(), str.end(), &::islower ) ) ) && (str.find_first_of("0123456789") == std::string::npos) ){
                    std::cout << "wrong message format";
                }
                //send 4 to server
                rq = 4;

                init_message(rq,msg,str.length(),str.c_str());
                tcp.Send(msg);
        }

		srand(time(NULL));

//        tcp.Send(msg);
		string rec = tcp.receive();
        std::cout << "response=" << std::endl;
        int res = atoi(rec.c_str());
        switch (res){
            case 0:
                //ALL OK
                std::cout << "OK!"<<std::endl;
                //return string if it was compression
                break;
            case 1:
                //unknown error
                std::cout << "Unknown error!"<<std::endl;
                break;
            case 2:
                //too big message
                std::cout << "too big message!"<<std::endl;
                break;
            case 3:
                //unsuuported request
                std::cout << "unsuuported request!"<<std::endl;
                break;
        }
		sleep(1);
	}
    }
	return 0;
}
