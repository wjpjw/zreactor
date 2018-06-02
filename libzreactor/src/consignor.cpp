//
// Created by jipeng on 5/31/18.
//

#include "consignor.h"

using namespace wjp;

consignor::consignor(zmq::context_t& context, const std::string& identifier)
        :socket_(context, ZMQ_ROUTER)
{
    socket_.bind("inproc://"+identifier);
}

//    Message message;
//    socket.recv(&message);
//    socket.recv(&message);
//    socket.recv(&message);
//    auto message_str=str(message); //message_str是addr和content拼起来的
//    addr=message_str.substr(0,5);  //addr长度永远是5
//    content=message_str.substr(5);

void consignor::on_response_msg_arrival()
{

}
