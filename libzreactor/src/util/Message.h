//
// Created by jipeng on 5/30/18.
//

#ifndef DO_NOT_YARN_MESSAGE_H
#define DO_NOT_YARN_MESSAGE_H

#include <zmq.hpp>
#include <memory>

/*

    1. 可以完全按值语义使用，copy ctor, copy assignment都允许用，只拷贝shared_ptr不拷贝数据。
    2. 把数据取出来：
            a) 可以用str()，           [字符串消息，或者测试用 | 因为这个过程有一次数据拷贝]
            b) 也可以用data()，        [二进制消息，生产环境用]
    3. 数据的产生：
            a) socket对象的recv，
            b) 对data指针进行操作，比如memcpy。

 */

namespace wjp {

    class message {
    public:
        message();               // data from recv()
        message(ssize_t size);   // data from memcpy() or direct data() handling
        message(const message&); // shared pointer counter +1
        ~message() noexcept;     // close msg, it could fail but cannot throw
        message&   operator= (message&& msg) noexcept;  // shared pointer content exchange
        ssize_t                       size() noexcept;
        void*                         data() noexcept;
        std::string                    str() noexcept;
    private:
        std::shared_ptr<zmq_msg_t> msgptr;
    };

}

#endif //DO_NOT_YARN_MESSAGE_H
