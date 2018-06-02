//
// Created by jipeng on 5/31/18.
//

#ifndef DO_NOT_YARN_CONSIGNOR_H
#define DO_NOT_YARN_CONSIGNOR_H

#include "zreactor.h"
#include "router_socket.h"

/*
    consignor（发货人）
    consignor负责协调liaison和内部的核心工作组件factory。
    它本质上是一个进程内线程间协调通信的router_socket。
    每当factory内的工作线程向它发送处理完的消息，它就把这个消息递交给liaison，让liaison再发回到客户端。
 */
namespace wjp {
    class consignor {
    public:
        inline operator void *() noexcept { return socket_; }
        consignor(zmq::context_t& context, const std::string& identifier);
        void on_response_msg_arrival();
    private:
        zmq::socket_t socket_;
    };
}

#endif //DO_NOT_YARN_CONSIGNOR_H
