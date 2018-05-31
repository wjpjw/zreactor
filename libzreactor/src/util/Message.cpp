//
// Created by jipeng on 5/30/18.
//

#include "Message.h"

using namespace wjp;

struct message::zmq_msg_autoclose{
    ~zmq_msg_autoclose() {  zmq_msg_close(&msg); }
    zmq_msg_t msg;
};

message::message() : msgptr_(std::make_shared<zmq_msg_autoclose>())
{
    int rc = zmq_msg_init(&msgptr_->msg);
    if (rc != 0) throw zmq::error_t();
}

message::message(size_t len) : msgptr_(std::make_shared<zmq_msg_autoclose>())
{
    int rc = zmq_msg_init_size(&msgptr_->msg, len);
    if (rc != 0) throw zmq::error_t ();
}

message::message(void* data, size_t size) : msgptr_(std::make_shared<zmq_msg_autoclose>())
{
    auto dealloc=[](void* data, void* hint){std::cout<<"free!";free(data);};
    int rc = zmq_msg_init_data (&msgptr_->msg, data, size, dealloc, nullptr);
    if (rc != 0) throw zmq::error_t ();
}

message::message(const void* data_, size_t size_):msgptr_(std::make_shared<zmq_msg_autoclose>())
{
    assert(size_<=1024);
    int rc = zmq_msg_init_size (&msgptr_->msg, size_);
    if (rc != 0) throw zmq::error_t ();
    memcpy(data(), data_, size_);
}

message::message(const char* data_):msgptr_(std::make_shared<zmq_msg_autoclose>())
{
    int size_=std::strlen(data_);
    assert(size_<=1024);
    int rc = zmq_msg_init_size (&msgptr_->msg, size_);
    if (rc != 0) throw zmq::error_t ();
    memcpy(data(), data_, size_);
}

message::message(const std::string& data_):msgptr_(std::make_shared<zmq_msg_autoclose>())
{
    int size_= data_.length();
    assert(size_<=1024);
    int rc = zmq_msg_init_size (&msgptr_->msg, size_);
    if (rc != 0) throw zmq::error_t ();
    memcpy(data(), data_.data(), size_);
}

message::message(const message& msg) noexcept : msgptr_(msg.msgptr_)
{
}

message::~message() noexcept
{
}

// 注意，这里存在一次数据拷贝！这个函数只是方便测试打印，不用于生产环境！
std::string                     message::str() const noexcept
{
    return std::string(static_cast<const char*>(data()), size());
}

message&                        message::operator= (message&& msg) noexcept
{
    msgptr_.swap(msg.msgptr_);
    return *this;
}

message&                        message::operator= (const message& msg) noexcept
{
    msgptr_=msg.msgptr_;
    return *this;
}

size_t                           message::size() const noexcept
{
    return zmq_msg_size(&msgptr_->msg);
}

void*                            message::data () noexcept
{
    return zmq_msg_data(&msgptr_->msg);
}

const void*                      message::data () const noexcept
{
    return zmq_msg_data (const_cast<zmq_msg_t*>(&msgptr_->msg));
}

bool                             message::operator==(const message &other) const noexcept
{
    return msgptr_.get()==other.msgptr_.get();
}

bool                             message::operator!=(const message &other) const noexcept
{
    return !(*this == other);
}

// flags: ZMQ_SNDMORE | ZMQ_DONTWAIT
bool                             message::send (void* socket, int flags_)
{
    int nbytes = zmq_msg_send (&msgptr_->msg, socket, flags_);
    if (nbytes >= 0)
        return true;
    if (zmq_errno () == EAGAIN)  // resource temporarily unavailable
        return false;
    throw zmq::error_t ();
}

bool                             message::recv (void* socket, int flags_)
{
    int nbytes = zmq_msg_recv (&msgptr_->msg, socket, flags_);
    if (nbytes >= 0)
        return true;
    if (zmq_errno () == EAGAIN)
        return false;
    throw zmq::error_t ();
}
