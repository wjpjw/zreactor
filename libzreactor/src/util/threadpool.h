//
// Created by jipeng on 5/26/18.
//

#ifndef DO_NOT_YARN_threadpool_H
#define DO_NOT_YARN_threadpool_H

#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
#include <thread>

namespace wjp {

    class threadpool {
    public:
        explicit threadpool(size_t nr_thread) : meta_(std::make_shared<meta>()) {
            for (size_t i = 0; i < nr_thread; i++) {
                std::thread([metaptr = meta_] {
                    std::unique_lock<std::mutex> lk(metaptr->mtx_);
                    for (;;) {
                        if (!metaptr->tasks_.empty()) {
                            auto current = std::move(metaptr->tasks_.front());
                            metaptr->tasks_.pop();
                            lk.unlock();
                            current();
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

        threadpool() = delete;

        threadpool(threadpool &&) = default;

        ~threadpool() {
            if ((bool) meta_) {
                {
                    std::lock_guard<std::mutex> lk(meta_->mtx_);
                    meta_->is_shutdown_ = true;
                }
                meta_->cond_.notify_all();
            }
        }

        template<class F>
        void execute(F &&task) {
            {
                std::lock_guard<std::mutex> lk(meta_->mtx_);
                meta_->tasks_.emplace(std::forward<F>(task));
            }
            meta_->cond_.notify_one();
        }

    private:
        struct meta {
            std::mutex mtx_;
            std::condition_variable cond_;
            bool is_shutdown_ = false;
            std::queue<std::function<void()>> tasks_;
        };
        std::shared_ptr<meta> meta_;
    };
}

#endif //DO_NOT_YARN_threadpool_H
