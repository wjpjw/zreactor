//
// Created by jipeng on 5/31/18.
//

#include "factory.h"

using namespace wjp;

struct factory::meta {
    std::mutex mtx_;
    std::condition_variable cond_;
    bool is_shutdown_ = false;
    std::queue<std::shared_ptr<task>> tasks_;   //addr content
};

//注意，之前把callback&作为捕获对象放在thread函数内执行，效率不如从队列中pop出来的。很有可能相同std::function对象的operator()访问是加锁的。总之如果有data race，避免lambda捕获其引用或指针。目前只是猜测，有空还要做测试。
factory::factory(size_t nr_pipelines, zmq::context_t& context)
: meta_(std::make_shared<meta>())
{
    for (size_t i = 0; i < nr_pipelines; i++) {
        std::thread([metaptr = meta_, &context] {
            std::unique_lock<std::mutex> lk(metaptr->mtx_);
            dealer_socket socket(context);
            socket.connect_inproc(CONSIGNOR_INPROC_NAME);
            for (;;) {
                if (!metaptr->tasks_.empty()) {
                    auto task_ = metaptr->tasks_.front();
                    metaptr->tasks_.pop();
                    lk.unlock();
                    task_->run(socket);    
                    lk.lock();
                } else if (metaptr->is_shutdown_) {
                    break;
                } else {
                    metaptr->cond_.wait(lk);
                }
            }
        }).detach();
    }
}


factory::~factory() {
    if ((bool) meta_) {
        {
            std::lock_guard<std::mutex> lk(meta_->mtx_);
            meta_->is_shutdown_ = true;
        }
        meta_->cond_.notify_all();
    }
}

void factory::process(std::shared_ptr<task> task_)
{
    {
        std::lock_guard<std::mutex> lk(meta_->mtx_);
        meta_->tasks_.emplace(task_);
    }
    meta_->cond_.notify_one();
}
