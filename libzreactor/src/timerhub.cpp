#include "timerhub.h"
#include <unistd.h>
#include <sys/timerfd.h>
#include <iterator>     // std::back_inserter
#include <algorithm>    // std::copy

using namespace wjp;

int                 wjp::create_timerfd()
{
  int timerfd = timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK | TFD_CLOEXEC);
  return timerfd;
}

void                wjp::read_timerfd(int timerfd)
{
  uint64_t howmany;
  ssize_t n = read(timerfd, &howmany, sizeof howmany);
}

void                wjp::reset_timerfd(int timerfd, time_point expiration)
{
  // wake up loop by timerfd_settime()
  struct itimerspec newValue;
  struct itimerspec oldValue;
  bzero(&newValue, sizeof newValue);
  bzero(&oldValue, sizeof oldValue);
  newValue.it_value = timespec_from_now(expiration);
  timerfd_settime(timerfd, 0, &newValue, &oldValue);
}


void                timerhub::add_timer(timer_callback cb, int64_t delay_ms, int64_t interval_ms)
{
    timer timer_(cb, delay_ms, interval_ms);
    bool earliestChanged = insert(timer_);
    if (earliestChanged)
    {
        reset_timerfd(timerfd_, timer_.expire_time());
    }
}

void                timerhub::on_timerfd_event()
{
    time_point cur = wjp::now();
    read_timerfd(timerfd_);
    std::vector<entry> expired_entries = expired(cur);
    for(auto& i : expired_entries)
    {
        i.second.exec(); //or factory->process
    }
    reset(expired_entries, cur);
}

bool                timerhub::insert(timer)
{
    bool earliestChanged = false;
    time_point when = timer.expire_time();
    auto it = timers_.begin();
    if (it == timers_.end() || when < it->first) earliestChanged = true;
    timers_.insert(entry(when, timer));
    return earliestChanged;
}

void                timerhub::reset(const std::vector<entry>& expired, time_point now)
{
    time_point nextExpire;
    for (auto& it : expired)
    {
        if (it.second.periodic())
        {
            it.second.renew_expire_time();
            insert(it.second);
        }
    }
    if (!timers_.empty())
    {
        nextExpire = timers_.begin()->second.expire_time();
    }
    reset_timerfd(timerfd_, nextExpire);
}

std::vector<timerhub::entry>  timerhub::expired(time_point now)
{
    std::vector<entry> expired;
    entry sentry(now, timer([]{})); 
    auto end = timers_.lower_bound(sentry);
    std::copy(timers_.begin(), end, back_inserter(expired));
    timers_.erase(timers_.begin(), end);
    return expired;
} 
