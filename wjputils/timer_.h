//
// Created by jipeng on 6/12/18.
//

#ifndef ZREACTOR_TIMER_H
#define ZREACTOR_TIMER_H

#include "wjp.h"
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/timerfd.h>
#include <unistd.h>

namespace dop{
    class timer;
    using timer_callback=std::function<int(timer&)>;
    class timer
    {
    public:
        timer(double, timer_callback, void*, bool);
        timer(const timer &ptimer);
        timer & operator=(const timer &ptimer);
        ~timer();
        void            start();
        void            start_with_new_interval(double);
        int             fd();
        void*           task_data();
        timer_callback  task();
    private:
        struct meta;
        std::shared_ptr<meta> meta_;
    } ;

    /* timerhub持有唯一的timerfd*/
    class timerhub{
    public:

    private:

    };

    class eventloop{
    public:
        eventloop():quit_(false),timerhub_(){}
        void run_every(timer_callback, double);
        void loop();
        void quit();
    private:
        bool            quit_;
        timerhub        timerhub_;
    };
/*
class timerRunner
{
public:
    timerRunner(int max_num=128)
    {
        active = 1;
        epfd = epoll_create(max_num);
    }
    ~ timerRunner()
    {
    }
    int add_timer(timer &ptimer);
    int del_timer(timer &ptimer);
    int run();
    int stop()
    {
        active = 0;
        return 0;
    }
    static timerRunner* getInstance(void);
private:
    static pthread_mutex_t mutex;
    static timerRunner* m_instance;
    int epfd;
    int active;
    std::map<int, timer> timers_map;
} ;

*/


}
#endif //ZREACTOR_TIMER_H
