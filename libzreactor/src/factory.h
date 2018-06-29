//
// Created by jipeng on 5/31/18.
//

#ifndef DO_NOT_YARN_FACTORY_H
#define DO_NOT_YARN_FACTORY_H

#include "wjp.h"
#include "util/message.h"
#include "util/dealer_socket.h"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <thread>
#include "task.h"

/*
    基于mutex的多线程任务队列

    有空再尝试一下无锁队列

 */
namespace wjp {

    class factory {
    public:
        ~factory();
        factory(size_t nr_pipelines, zmq::context_t& context);
        void process(std::shared_ptr<task> task_);
    private:
        struct meta;
        std::shared_ptr<meta> meta_;
    };

}
#endif //DO_NOT_YARN_FACTORY_H
