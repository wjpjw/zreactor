#pragma once
#include "wjp.h"
#include "./util/clock.h"
#include "timer.h"
#include <set>

namespace wjp
{
class eventloop;
class timerhub
{
public:
    timerhub(eventloop&);
    void                add_timer(timer_callback cb, int64_t delay_ms, int64_t interval_ms);
private:
    using entry=std::pair<time_point, timer>;

    void                on_timerfd_event();
    bool                insert(timer);
    void                reset(const std::vector<entry>& expired, time_point now);
    std::vector<entry>  expired(time_point now); 

    int                 timerfd_;
    std::set<entry>     timers_;
    eventloop&          loop_;
    
};

extern int create_timerfd();
extern void read_timerfd(int timerfd);
extern void reset_timerfd(int timerfd, time_point expiration);

}