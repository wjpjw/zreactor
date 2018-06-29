#pragma once
#include "wjp.h"
#include "./util/message.h"
#include "timer.h"
namespace wjp{
class dealer_socket;
class task
{
public:
    //如果要发送消息回去，就用这个dealer_socket！不发消息回去的话，无视即可。
    virtual void run(dealer_socket&) = 0;
};

class timer_task : public task
{
public:
    timer_task(timer t);
    virtual void run(dealer_socket&) override;
private:
    timer timer_;
};

class client_task : public task
{
public:
    client_task(message addr, message content, callback);
    virtual void run(dealer_socket&) override;
private:
    callback cb_;
    message addr_;
    message content_;
};

}