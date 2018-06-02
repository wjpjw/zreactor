//
// Created by jipeng on 5/31/18.
//

#include "factory.h"

using namespace wjp;

struct factory::task{
    task(message a, message c):addr_(a),content_(c){}
    message addr_;
    message content_;
};


struct factory::meta {
    std::mutex mtx_;
    std::condition_variable cond_;
    bool is_shutdown_ = false;
    std::queue<task> tasks_;   //addr content
};

factory::factory(size_t nr_pipelines, callback callback_, zmq::context_t& context)
: meta_(std::make_shared<meta>())
{
    for (size_t i = 0; i < nr_pipelines; i++) {
        std::thread([metaptr = meta_] {
            std::unique_lock<std::mutex> lk(metaptr->mtx_);
            dealer_socket socket(context);
            socket.connect_inproc(CONSIGNOR_INPROC_NAME);
            for (;;) {
                if (!metaptr->tasks_.empty()) {
                    auto task = metaptr->tasks_.front();
                    metaptr->tasks_.pop();
                    lk.unlock();
                    socket.send_to_router(callback_(task.addr_, task.content_));
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

void factory::process(message addr, message content)
{
    {
        std::lock_guard<std::mutex> lk(meta_->mtx_);
        meta_->tasks_.emplace(addr, content);
    }
    meta_->cond_.notify_one();
}
