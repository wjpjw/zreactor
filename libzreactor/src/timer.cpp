#include "timer.h"

using namespace wjp;

struct timer::meta{
    meta(timer_callback cb, int64_t delay_ms, int64_t interval_ms)
    :callback_(cb), expire_time_(now()+std::chrono::milliseconds(delay_ms)), interval_(interval_ms)
    {}
    timer_callback      callback_;
    time_point          expire_time_;
    int64_t             interval_;
};

timer::timer(timer_callback cb, int64_t delay_ms, int64_t interval_ms) noexcept
    : meta_(std::make_shared<meta>(cb, delay_ms, interval_ms))
{}

timer::timer(const timer& other) noexcept : meta_(other.meta_)
{
}

timer&  timer::operator=(const timer& other) noexcept
{
    meta_=other.meta_;
}

bool timer::operator<(const timer  &b) const {  
    return (meta_->expire_time_ <b.meta_->expire_time_);  
}  

void                    timer::exec()
{
    renew_expire_time();
    meta_->callback_();
}

time_point              timer::expire_time() const noexcept
{
    return meta_->expire_time_;
}

bool                    timer::periodic() const noexcept
{
    return meta_->interval_>=0;
}

//renew_expire_time()只会在timer到期时被立刻调用，meta_->expire_time_的值应该和now()的值非常接近。如果程序出现异常的阻塞，这个timer在一次renew后也会更新得到正确的meta_->expire_time_。
void                    timer::renew_expire_time() noexcept
{
  if (periodic()){
    meta_->expire_time_=now()+std::chrono::microseconds(meta_->interval_);
  }
}
