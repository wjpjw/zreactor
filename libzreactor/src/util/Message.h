//
// Created by jipeng on 5/30/18.
//

#ifndef DO_NOT_YARN_MESSAGE_H
#define DO_NOT_YARN_MESSAGE_H

#include <zmq.h>
#include <memory>
#include "zreactor.h"

/*
    设计原则是：最优性能，最小接口。
    使用时遵循下列原则：
    1. 使用值语义，也就是说，请把wjp::message当基础类型来用。
    2. 初始化数据的构造函数message(void*, size_t)要求用户提供堆中某个malloc出来的内存指针，并把这块内存的所有权转移给message。默认用free(data)的方式释放内存，所以不允许new，一定要用malloc。
    3. 数据生命周期由shared_ptr控制，如果一份数据无message持有，就会析构。
 */

namespace wjp {

    class message {
    public:
        message();                          // 这种初始化没设置长度，数据只允许来自recv()
        message(size_t len);                // 设置好长度后，可直接操作data()指针指向的内存
        message(const void*, size_t);       // 请慎重使用，虽然方便，但是有一次复制！
        message(void*, size_t);             // 这是性能最高的初始化方式！数据所有权转移，零次复制！
        message(const message&) noexcept;   // 所有权共享，shared_ptr counter++
        ~message() noexcept;                // 无需进行任何操作，析构自身的shared_ptr
        message& operator= (message&&) noexcept; // 所有权转移，和右值message互换shared_ptr
        message& operator= (const message&) noexcept;  // 所有权共享，shared_ptr counter++
        size_t size() const noexcept;       // 消息的大小，除了recv()之外，不会改变。
        void* data() noexcept;              // 数据在内存中的指针。
        const void* data()const noexcept ;  // 数据的只读指针。
        std::string str() const noexcept;   // 生成字符串，进行一次复制！
        bool operator==(const message&) const noexcept; // 指向相同的zmq_msg
        bool operator!=(const message&) const noexcept; // 指向不同的zmq_msg
    private:
        struct zmq_msg_autoclose;           // 配合shared_ptr，最后一个所有者析构时再真正析构zmq_msg
        std::shared_ptr<zmq_msg_autoclose> msgptr;
    };

}

#endif //DO_NOT_YARN_MESSAGE_H
