//
// Created by jipeng on 5/25/18.
//

#ifndef DO_NOT_YARN_ZREACTOR_H
#define DO_NOT_YARN_ZREACTOR_H

#include <zmq.hpp>
#include <sys/time.h>
#include <time.h>
#include <iostream>

/* linux high resolution sleep */
static inline void linux_sleep_msecs(int msecs) {
    struct timespec t;
    t.tv_sec = msecs / 1000;
    t.tv_nsec = (msecs % 1000) * 1000000;
    nanosleep (&t, NULL);
}

/*  linux current time */
static inline int64_t linux_clock() {
    struct timeval tv;
    gettimeofday (&tv, NULL);
    return (int64_t) (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}


#endif //DO_NOT_YARN_ZREACTOR_H
