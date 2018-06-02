//
// Created by jipeng on 5/30/18.
//

#ifndef DO_NOT_YARN_POLLERSERVER_H
#define DO_NOT_YARN_POLLERSERVER_H
#include "zreactor.h"
#include "util/message.h"

/*
    这个头文件是整个数据库访问代理核心层中唯一暴露给用户的公开接口。
    用户可以完全忽视zreactor的内部实现，只需要掌握这个reactor类的使用方法。

    reactor是一个基于zmq::poll的异步非阻塞服务器。

    它看起来虽然非常简单，但的确实现了reactor的功能。复杂度被隐藏在两个子组件中了——liaison, consignor——二者各自维护一个socket，放在reactor的poller中进行epoll事件轮询监听。

    reactor捕获输入事件后会调用liaison的recv_from_req()方法读取客户端的消息，并压入多线程任务队列，让工作线程去处理。

    工作线程处理完后会通过自身绑定的socket用进程内通信渠道把处理完的消息发送给consignor。consignor接收到处理好的消息后再委托liaison寄送给客户端。
 */
namespace wjp{
    class reactor {
    public:
        reactor(const std::string& port,  //提供服务的端口
                callback cb,              //工厂对消息进行处理的回调函数
                int nr_workerthreads=10,  //重要参数，处理客户端请求的工作线程池大小
                int timeout_millisec=-1,  //基本不需要调整，服务器上不应该有更高优先级的进程了，强势抢占
                int nr_iothreads=1        //zmq后台io线程池大小，1个和多个区别不大
                );
        void start();
    private:
        zmq::context_t  context_;
        callback        callback_;
        int             nr_workerthreads_;
        int             timeout_millisec_;
        std::string     port_;
    };
}


#endif //DO_NOT_YARN_POLLERSERVER_H
