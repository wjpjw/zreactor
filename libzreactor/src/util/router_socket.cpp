//
// Created by jipeng on 5/31/18.
//

#include "router_socket.h"
#include "message.h"

using namespace wjp;

router_socket::router_socket(zmq::context_t& context)
        :socket_(context, ZMQ_ROUTER)
{
}

void router_socket::connect_tcp(const std::string& ip_port)
{
    socket_.connect("tcp://"+ip_port);
}

void router_socket::connect_inproc(const std::string& identifier)
{
    socket_.connect("inproc://"+identifier);
}

void router_socket::bind_inproc(const std::string& identifier)
{
    socket_.bind("inproc://"+identifier);
}

void router_socket::bind_tcp(const std::string& port) {
    socket_.bind("tcp://*:" + port);
}

/* 接收来自req类型socket消息（这种消息格式特殊，分为addr+""+content三段），放到addr和content里。 */
void router_socket::recv_from_req(message& addr, message& content)
{
    addr.recv(socket_);     // 第一帧是addr
    content.recv(socket_);  // 第二帧是空帧
    content.recv(socket_);  // 第三帧是content
}

/* 对req类型发送消息，同样遵循三段格式。 */
void router_socket::send_to_req(message addr, message content)
{
    addr.send(socket_, ZMQ_SNDMORE);
    message{(size_t)0}.send(socket_, ZMQ_SNDMORE);
    content.send(socket_);
}

/* 对router类型的socket发送消息，格式是addr紧跟content */
void router_socket::send_to_router(message addr, message content)
{
    addr.send(socket_, ZMQ_SNDMORE);
    content.send(socket_);
}

/* 接收来自router类型的socket的消息，格式是addr紧跟content */
void router_socket::recv_from_router(message &addr, message &content)
{
    addr.recv(socket_);
    content.recv(socket_);
}

/* 对dealer类型的socket发送消息，格式是addr紧跟content，和router类型socket一样 */
void router_socket::send_to_dealer(message addr, message content)
{
    addr.send(socket_, ZMQ_SNDMORE);
    content.send(socket_);
}

/* 接收来自dealer类型的socket的消息，格式是addr紧跟content，和router类型socket一样 */
void router_socket::recv_from_dealer(message &addr, message &content)
{
    addr.recv(socket_);
    content.recv(socket_);
}

/* 设置自身的addr，如果非要用router-router模式的话 */
void router_socket::set_addr(const std::string& myaddr)
{
    socket_.setsockopt(ZMQ_IDENTITY, myaddr.c_str(), myaddr.length());
}

message router_socket::recv_from_dealer_content_only()
{
    message tmp;
    tmp.recv(socket_);  //不关心addr
    tmp.recv(socket_);  //只取content
    return tmp;
}