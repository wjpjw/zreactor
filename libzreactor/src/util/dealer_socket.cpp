//
// Created by jipeng on 6/1/18.
//

#include "dealer_socket.h"
#include "message.h"

using namespace wjp;

dealer_socket::dealer_socket(zmq::context_t& context)
        :socket_(context, ZMQ_DEALER)
{
}

void dealer_socket::recv_from_router(message& content)
{
    content.recv(socket_);
}

void dealer_socket::send_to_router(message content)
{
    content.send(socket_);
}

void dealer_socket::connect_tcp(const std::string& ip_port)
{
    socket_.connect("tcp://"+ip_port);
}

void dealer_socket::connect_inproc(const std::string& identifier)
{
    socket_.connect("inproc://"+identifier);
}
