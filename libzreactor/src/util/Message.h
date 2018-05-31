//
// Created by jipeng on 5/30/18.
//

#ifndef DO_NOT_YARN_MESSAGE_H
#define DO_NOT_YARN_MESSAGE_H

#include "zreactor.h"

/*
    三个设计目标：最优性能，最小接口，高易用性。
    使用时遵循下列原则：
    1. 使用值语义，也就是说，请把wjp::message当基础类型来用。
    2. 初始化数据的构造函数message(void*, size_t)要求用户提供堆中某个malloc出来的内存指针，并把这块内存的所有权转移给message。默认用free(data)的方式释放内存，所以不允许new，一定要用malloc。
    3. 数据生命周期由shared_ptr控制，如果一份数据无message持有，就会析构。
    4. 较小的数据没必要malloc，完全可以把栈中复制一次，效率反而更高，调用接口也更优雅。
    5. 较大的数据必须用malloc自行创建内存指针再调用message(void*, size_t)零次拷贝构造函数。如果超过1KB还想用有数据拷贝的ctor构造，就会直接报错。
    6. message应该有自己的send/recv方法，zmqcpp官方的封装里把这个功能放到socket里面去了，不合理。让数据流向服务，还是让服务流向数据，在重数据的场景下，显然后者更合理。

    具体怎么用还可以参考这个头文件末尾的example代码。
 */

namespace wjp {
    class message {
    public:
        message();                          // 这种初始化没设置长度，数据只允许来自recv()
        message(size_t len);                // 设置好长度后，可直接操作data()指针指向的内存
        message(const void*, size_t);       // 请慎重使用，虽然方便，但是有一次复制！size必须小于1KB。
        message(const char*);               // 用字符串初始化，有一次复制！len < 1024，否则报错！
        message(const std::string&);        // 同上！
        message(void*, size_t);             // 这是性能最高的初始化方式！数据所有权转移，零次复制！
        message(const message&) noexcept;   // 所有权共享，shared_ptr counter++
        ~message() noexcept;                // 无需进行任何操作，析构自身的shared_ptr
        message& operator= (message&&) noexcept; // 所有权转移，和右值message互换shared_ptr
        message& operator= (const message&) noexcept;  // 所有权共享，shared_ptr counter++
        size_t size() const noexcept;       // 消息的大小，除了recv()之外，不会改变。
        void* data() noexcept;              // 数据在内存中的指针。
        const void* data()const noexcept ;  // 数据的只读指针。
        std::string str() const noexcept;   // 生成字符串，进行一次复制！这个方法最好只用于测试打印！
        bool operator==(const message&) const noexcept; // 指向相同的zmq_msg则相等
        bool operator!=(const message&) const noexcept; // 指向不同的zmq_msg则不等
        bool send(void* socket, int flags_ = 0);  //send之后message清空。
        bool recv(void* socket, int flags_ = 0);  //这也是一种数据初始化的方法，接收来自socket的消息。
    private:
        struct zmq_msg_autoclose;           // 配合shared_ptr，最后一个所有者析构时再真正析构zmq_msg
        std::shared_ptr<zmq_msg_autoclose> msgptr_;
    };


#ifdef WJP_EXAMPLE
    static void how_to_use_message()
    {
        // [0] 关于拷贝赋值、生命周期、data()、右值赋值的实验
        wjp::message msg{2};
        std::string tmp("duh");
        memcpy(msg.data(), tmp.data(), tmp.size());
        std::cout << msg.str() << std::endl;
        {
            wjp::message aa{3};
            std::string kde("aaa");
            memcpy(aa.data(), kde.data(), kde.size());
            msg = std::move(aa);
            std::cout<<"aaa: "<<aa.str()<<std::endl;
        }
        std::cout<<"msg: "<<msg.str()<<std::endl;

        // [1] 不超过1KB数据的初始化
        wjp::message ww("wjp",3); // 本质上是message(const void*, size_t)初始化
        std::cout<<ww.str()<<std::endl;

        wjp::message ee("wjpjpw", 3);
        std::cout<<ee.str()<<std::endl;

        wjp::message ff("wjpjpw"); // message(const char*)初始化
        std::cout<<ff.str()<<std::endl;

        std::string oo("wjpstr"); // message(const std::string&)初始化
        wjp::message ss(oo);
        std::cout<<ss.str()<<std::endl;

        // [2] 零次拷贝初始化，最好用在较大的数据上
        char* buf=(char*)malloc(300);
        for(int i=0;i<256;i++)
        {
            *(buf+i)=i;
        }
        wjp::message ewe(buf, 300);
        std::cout<<ewe.str()<<std::endl;

        // [3] 析构正确性实验，结果是两次message dtor，一次free
        char *tmp = (char *) malloc(300);
        wjp::message ss(tmp, 300);
        auto hh=ss;

        // [4] send/recv 示例代码

        zmq::context_t ctx;
        zmq::socket_t Alice(ctx, ZMQ_REP);
        zmq::socket_t Bob(ctx, ZMQ_REQ);
        Alice.bind("inproc://duh");
        Bob.connect("inproc://duh");

        wjp::message BobLetter("Watching from afar I feel amused.");
        std::cout<<"Bob writes: "<<BobLetter.str()<<std::endl;
        BobLetter.send(Bob);
        std::cout<<"After send(), the letter is now empty, blank, whitespace, whatever. "<<BobLetter.str()<<std::endl;

        wjp::message AliceReads;
        AliceReads.recv(Alice);
        std::cout<<"Alice reads: "<<AliceReads.str()<<std::endl;

    }
#endif


}

#endif //DO_NOT_YARN_MESSAGE_H
