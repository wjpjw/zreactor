//
// Created by jipeng on 6/12/18.
//

#include "timer_.h"

using namespace dop;

struct timer::meta{
    meta(int interval, timer_callback callback, void* data, bool periodic):
            fd_(0), interval_(interval),task_(callback), task_data_(data), periodic_(periodic)
    {}
    int             fd_;
    double          interval_;
    timer_callback  task_;
    void*           task_data_;
    bool            periodic_;
};

int             timer::fd()
{
    return meta_->fd_;
}

void*           timer::task_data()
{
    return meta_->task_data_;
}

timer_callback  timer::task()
{
    return meta_->task_;
}

timer::timer(double interval, timer_callback callback, void *data, bool periodic) :
    meta_(std::make_shared<meta>(interval, callback, data, periodic))
{
    meta_->fd_ = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    if(meta_->fd_ < 0)
    {
        throw std::runtime_error("Failed to create timerfd!");
    }
}

timer::timer(const timer& other) :meta_(other.meta_){}

timer & timer::operator =(const timer& other)
{
    meta_=other.meta_;
    return *this;
}

timer::~timer()
{
    close(meta_->fd_);
}

void timer::start()
{
    struct itimerspec timerspec_ = {0};
    timerspec_.it_value.tv_sec = (int) meta_->interval_;
    timerspec_.it_value.tv_nsec = (meta_->interval_ - (int) meta_->interval_)*1000000000;
    if(meta_->periodic_)
    {
        timerspec_.it_interval.tv_sec = timerspec_.it_value.tv_sec;
        timerspec_.it_interval.tv_nsec = timerspec_.it_value.tv_nsec;
    }
    timerfd_settime(meta_->fd_, 0, &timerspec_, nullptr);
}

void timer::start_with_new_interval(double interval)
{
    meta_->interval_=interval;
    start();
}

/*

int timerRunner::add_timer(timer& ptimer)
{
    int timer_id = ptimer.timer_get_id();
    struct epoll_event ev;
    ev.data.fd = timer_id;
    ev.events = EPOLLIN | EPOLLET;
    timers_map[timer_id] = ptimer; //add or modify
    epoll_ctl (epfd, EPOLL_CTL_ADD, timer_id, &ev);
    ptimer.start();

    return 0;
}
int timerRunner::del_timer(timer& ptimer)
{
    int timer_id = ptimer.timer_get_id();
    struct epoll_event ev;
    ev.data.fd = timer_id;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl (epfd, EPOLL_CTL_DEL, timer_id, &ev);
    timers_map.erase(timer_id);

    return 0;
}
int timerRunner::run()
{
    char buf[128] ={0};
    active = 1;
    for (; active ; )
    {
        struct epoll_event events[MAXFDS] ={0};
        int nfds = epoll_wait (epfd, events, MAXFDS, -1);
        for (int i = 0; i < nfds; ++i)
        {
            std::map<int, timer>::iterator itmp = timers_map.find(events[i].data.fd);
            if (itmp != timers_map.end())
            {
                //timer ptimer = itmp->second;
                while (read(events[i].data.fd, buf, 128) > 0);
                itmp->second.get_user_callback()(itmp->second);
            }
        }
    }
}

pthread_mutex_t timerRunner::mutex = PTHREAD_MUTEX_INITIALIZER;
timerRunner* timerRunner::m_instance = NULL;

timerRunner* timerRunner::getInstance(void)
{
    if(NULL == m_instance)
    {
        pthread_mutex_lock(&mutex);
        if(NULL == m_instance)
        {
            m_instance = new timerRunner(128);
        }
        pthread_mutex_unlock(&mutex);
    }
    return m_instance;
}

 */