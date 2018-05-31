//
// Created by jipeng on 5/30/18.
//

#ifndef DO_NOT_YARN_POLLERSERVER_H
#define DO_NOT_YARN_POLLERSERVER_H
#include "zreactor.h"

namespace wjp{
    class reactor {
    public:
        reactor(int port, callback cb,
                int nr_workerthreads=10, //重要参数，处理客户端请求的工作线程池大小
                int timeout_millisec=-1, //基本不需要调整，服务器上不应该有更高优先级的进程了，强势抢占
                int nr_iothreads=1       //zmq后台io线程池大小，1个和多个区别不大
                );
        void start();
    private:
        zmq::context_t  context_;
        callback        callback_;
        int             nr_workerthreads_;
        int             timeout_millisec_;
        int             port_;
    };
}


#endif //DO_NOT_YARN_POLLERSERVER_H
