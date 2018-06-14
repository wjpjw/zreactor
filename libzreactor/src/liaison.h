//
// Created by jipeng on 5/31/18.
//

#ifndef DO_NOT_YARN_LIAISON_H
#define DO_NOT_YARN_LIAISON_H

#include "wjp.h"
#include "util/message.h"
#include "util/router_socket.h"
#include "factory.h"

/*
    liaison（联络人）
    liaison负责与客户端的req类型的socket交互。
    它本质上是一个同时具有factory和clients联系方式的router_socket。
    每当clients的req类型socket发送请求过来，它就将这个请求递交给factory，让它决定怎么处理——放入某个工作线程异步执行或阻塞等待空闲的工作进程。
 */
namespace wjp{
    class liaison : public router_socket{
    public:
        liaison(zmq::context_t& context, const std::string& port, callback cb, int nr_threads);
        void                        on_request_msg_arrival();
    private:
        std::unique_ptr<factory>    factory_;   //消息处理工厂的唯一联系人
        zmq::context_t&             context_;   //进程唯一的ctx
        callback                    cb_;
    };

}

#endif //DO_NOT_YARN_LIAISON_H
