//
// Created by jipeng on 5/30/18.
//

#include "Message.h"

using namespace wjp;

message::message()
{
    int rc = zmq_msg_init (msgptr.get());
    if (rc != 0) throw zmq::error_t();
}

message::message(const message& msg)
{
    msgptr=msg.msgptr;
}

message::~message() noexcept
{
    zmq_msg_close (msgptr.get());
}

std::string                     message::str()
{
    return std::string(static_cast<char*>(data()), size());
}

message&                        message::operator= (message&& msg) noexcept
{
    msgptr.swap(msg.msgptr);
    return *this;
}

ssize_t                          message::size() noexcept
{
    return zmq_msg_size(msgptr.get());
}

void*                            message::data () noexcept
{
    return zmq_msg_data(msgptr.get());
}
