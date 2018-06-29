#include "task.h"
#include "./util/dealer_socket.h"
using namespace wjp;

timer_task::timer_task(timer t) 
                        : timer_(t)
{}

void timer_task::run(dealer_socket&)
{
    timer_.exec();
}

/*====================================================================*/

client_task::client_task(message addr, message content, callback cb) 
                 :  addr_(addr), 
                    content_(content), 
                    cb_(cb)
{}

void client_task::run(dealer_socket& socket)
{
    auto resp_=cb_(addr_, content_); 
    addr_.send(socket, ZMQ_SNDMORE);   
    resp_.send(socket);
}
