//
// Created by jipeng on 6/12/18.
//

#ifndef ZREACTOR_CLOCK_H
#define ZREACTOR_CLOCK_H

#include <ctime>
#include <ratio>
#include <thread>
#include <chrono>

namespace wjp{
    using time_point = std::chrono::high_resolution_clock::time_point;
    static inline void sleep(int ms)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }

    static inline std::chrono::high_resolution_clock::time_point now()
    {
        return std::chrono::high_resolution_clock::now();
    }

    static inline int64_t duration_in_milliseconds(time_point start, time_point end)
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    }

    static inline int64_t duration_in_microsecond(time_point start, time_point end)
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }

    static inline int64_t microseconds_elapsed(time_point start)
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(now() - start).count();
    }

    static inline int64_t milliseconds_elapsed(time_point start)
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(now() - start).count();
    }
}

#endif //ZREACTOR_CLOCK_H
