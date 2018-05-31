//
// Created by jipeng on 5/30/18.
//

#include "Message.h"
#include <iostream>

using namespace wjp;

struct message::zmq_msg_autoclose{
    ~zmq_msg_autoclose() {  zmq_msg_close(&msg); }
    zmq_msg_t msg;
};

message::message() : msgptr(std::make_shared<zmq_msg_autoclose>())
{
    int rc = zmq_msg_init(&msgptr->msg);
    if (rc != 0) throw zmq::error_t();
}

message::message(size_t len) : msgptr(std::make_shared<zmq_msg_autoclose>())
{
    int rc = zmq_msg_init_size(&msgptr->msg, len);
    if (rc != 0) throw zmq::error_t ();
}

message::message(void* data, size_t size) : msgptr(std::make_shared<zmq_msg_autoclose>())
{
    auto dealloc=[](void* data, void* hint){free(data);};
    int rc = zmq_msg_init_data (&msgptr->msg, data, size, dealloc, nullptr);
    if (rc != 0) throw zmq::error_t ();
}

message::message(const void* data_, size_t size_):msgptr(std::make_shared<zmq_msg_autoclose>())
{
    int rc = zmq_msg_init_size (&msgptr->msg, size_);
    if (rc != 0) throw zmq::error_t ();
    memcpy(data(), data_, size_);
}

message::message(const message& msg) noexcept : msgptr(msg.msgptr)
{
}

message::~message() noexcept {}

// 注意，这里存在一次数据拷贝！
std::string                     message::str() const noexcept
{
    return std::string(static_cast<const char*>(data()), size());
}

message&                        message::operator= (message&& msg) noexcept
{
    msgptr.swap(msg.msgptr);
    return *this;
}

message&                        message::operator= (const message& msg) noexcept
{
    msgptr=msg.msgptr;
    return *this;
}

size_t                           message::size() const noexcept
{
    return zmq_msg_size(&msgptr->msg);
}

void*                            message::data () noexcept
{
    return zmq_msg_data(&msgptr->msg);
}

const void*                      message::data () const noexcept
{
    return zmq_msg_data (const_cast<zmq_msg_t*>(&msgptr->msg));
}

bool                             message::operator==(const message &other) const noexcept
{
    return msgptr.get()==other.msgptr.get();
}

bool                             message::operator!=(const message &other) const noexcept
{
    return !(*this == other);
}
