//
// Created by jipeng on 5/31/18.
//

#include "liaison.h"
#include "threadpool.h"
#include "message.h"
#include "factory.h"

using namespace wjp;

liaison::liaison(zmq::context_t& context, const std::string& port, callback cb, int nr_threads)
        :router_socket(context),
         factory_(std::make_unique<factory>(nr_threads, cb, context)),
         context_(context)
{
    bind_tcp(port);
}

//交付factory处理即可，哪怕再简单的任务也不等待处理结果，免得阻塞时间循环。
void liaison::on_request_msg_arrival()
{
    message addr, content;
    recv_from_req(addr, content);
    factory_->process(addr, content);
}
