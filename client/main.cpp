//
// Created by jipeng on 5/22/18.
//

//
// Created by jipeng on 5/22/18.
//

#include "wjp.h"
#include <future>
#include "zhelper.h"
#include "clock.h"
#include <boost/lexical_cast.hpp>

void test_tmp() {
    std::vector<std::future<std::string>> tasks;
    for(int task_id=0;task_id<50;task_id++)
    {
        auto ten_requests=[task_id](){
            std::stringstream ss;
            zmq::context_t context(1);
            zmq::socket_t requester(context, ZMQ_REQ);
            requester.connect("tcp://localhost:5559");
            std::string id="Task"+boost::lexical_cast<std::string>(task_id);
            for( int request_id = 0; request_id < 10; request_id++) {
                auto start=wjp::now();
                s_send (requester, id+" says: "+boost::lexical_cast<std::string>(request_id));
                auto recvd = s_recv (requester);
                ss << "Response No. "<< request_id << " [" << recvd << "] "
                   << wjp::microseconds_elapsed (start)<<" μs\n";
                wjp::sleep(200);
            }
            return ss.str();
        };
        tasks.push_back(std::async(std::launch::async, ten_requests));
    }
    for(int i=0;i<tasks.size();i++)
    {
        std::cout<<i<<":"<<std::endl<<tasks[i].get()<<std::endl<<std::endl<<std::endl;
    }
}

int main (int argc, char** argv)
{
    try
    {
        test_tmp();
    }
    catch(const std::exception& e)
    {
        std::cerr << "Standard Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_FAILURE;
}
