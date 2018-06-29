#pragma once
#include "wjp.h"
#include "./util/clock.h"

namespace wjp{
/*支持ms精度的计时器。*/
class timer{
public:
    // static const 和const都可以不在class内初始化；而static constexpr类型的变量（非函数）则强制要求在class内初始化，保证不会在编译单元cpp文件里初始化。和宏相比，它又能避免冲突且享受namespace。因此它是最合理的c++常量定义途径。
    static constexpr int64_t NO_REPEAT=-1;
    timer(timer_callback cb, int64_t delay_ms=0, int64_t interval_ms=NO_REPEAT) noexcept; 
    timer(const timer&) noexcept;
    timer & operator=(const timer&) noexcept;
    void                    exec();
    time_point              expire_time() const noexcept;
    bool                    periodic() const noexcept;
    void                    renew_expire_time() noexcept;
    bool operator<(const timer  &b) const;
private:
    struct meta;
    std::shared_ptr<meta>   meta_;
};

}