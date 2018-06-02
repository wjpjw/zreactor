//
// Created by jipeng on 5/31/18.
//

#ifndef DO_NOT_YARN_ROUTER_SOCKET_H
#define DO_NOT_YARN_ROUTER_SOCKET_H

#include "zreactor.h"

/*
   router_socket是对原生socket的包装，让它支持具体通信模式下的消息收发、连接绑定。
   因此它隐藏这些通信模式的隐式约束，让用户（程序员）能更加轻松地实现诸如router-dealer之类的通信模式。
   具体怎么用可以看下面的几个example。
 */

namespace wjp{
    class message;
    class router_socket {
    public:
        inline operator void *() noexcept { return socket_; }
        router_socket(zmq::context_t& context);
        void recv_from_req(message& addr, message& content);
        void send_to_req(message addr, message content);
        void recv_from_router(message& addr, message& content);
        void send_to_router(message addr, message content);
        void recv_from_dealer(message& addr, message& content);
        message recv_from_dealer_content_only();
        void send_to_dealer(message addr, message content);
        void bind_tcp(const std::string& port);
        void bind_inproc(const std::string& identifier);
        void connect_tcp(const std::string& ip_port);
        void connect_inproc(const std::string& identifier);
        void set_addr(const std::string& myaddr);
    protected:
        zmq::socket_t socket_;
    };
}


#ifdef WJP_EXAMPLE

static void test_router_socket_and_dealer_socket_1() {
    zmq::context_t ctx;
    wjp::router_socket router(ctx);
    wjp::dealer_socket dealer(ctx);

    router.bind_inproc("routerid");
    dealer.connect_inproc("routerid");

    dealer.send_to_router(wjp::message{"hey! bobo!"});
    wjp::message addr,content;
    router.recv_from_dealer(addr, content);
    std::cout<<"addr: "<<addr.str()<<std::endl<<"content: "<<content.str()<<std::endl;

    dealer.send_to_router(wjp::message{"bo bo bo bo!"});
    router.recv_from_dealer(addr, content);
    std::cout<<"addr: "<<addr.str()<<std::endl<<"content: "<<content.str()<<std::endl;
}

//输出的是ONE,TWO，也就是说顺序地处理到来的消息信封(包含id+content两段)，要么全收到，要么全收不到。
static void test_router_socket_and_dealer_socket_2() {
    zmq::context_t ctx;
    wjp::router_socket router(ctx);
    wjp::dealer_socket dealer(ctx);
    router.bind_inproc("routerid");
    dealer.connect_inproc("routerid");

    wjp::message addr,content;  //msg containers for router

    dealer.send_to_router(wjp::message{"ONE"});
    dealer.send_to_router(wjp::message{"TWO"});
    dealer.send_to_router(wjp::message{"THREE"});
    dealer.send_to_router(wjp::message{"FOUR"});

    router.recv_from_dealer(addr, content);
    std::cout<<"addr: "<<addr.str()<<std::endl<<"content: "<<content.str()<<std::endl;

    router.recv_from_dealer(addr, content);
    std::cout<<"addr: "<<addr.str()<<std::endl<<"content: "<<content.str()<<std::endl;
}

//虽然说poller+router是异步非阻塞的，但前提是缓冲区里真的有消息，按下面代码来是会阻塞的。
static void test_router_socket_and_dealer_socket_3() {
    zmq::context_t ctx;
    wjp::router_socket router(ctx);
    wjp::dealer_socket dealer(ctx);
    router.bind_inproc("routerid");
    dealer.connect_inproc("routerid");

    wjp::message addr,content;  //msg containers for router

    router.recv_from_dealer(addr, content);
    std::cout<<"addr: "<<addr.str()<<std::endl<<"content: "<<content.str()<<std::endl;
    router.recv_from_dealer(addr, content);
    std::cout<<"addr: "<<addr.str()<<std::endl<<"content: "<<content.str()<<std::endl;

    dealer.send_to_router(wjp::message{"ONE"});
    dealer.send_to_router(wjp::message{"TWO"});
    dealer.send_to_router(wjp::message{"THREE"});
    dealer.send_to_router(wjp::message{"FOUR"});

    router.recv_from_dealer(addr, content);
    std::cout<<"addr: "<<addr.str()<<std::endl<<"content: "<<content.str()<<std::endl;

    router.recv_from_dealer(addr, content);
    std::cout<<"addr: "<<addr.str()<<std::endl<<"content: "<<content.str()<<std::endl;
}

#endif


/*
这是zmq默认赋予的addr的一个例子，同一个进程里120个线程起了120个客户端。
addr是发到router socket时才生成的，随机且互不相同的，是router类型socket独有的概念。

addr: 0 -128 0 65 -89  content: Task29 says: 0
addr: 0 -128 0 65 -88  content: Task31 says: 0
addr: 0 -128 0 65 -87  content: Task28 says: 0
addr: 0 -128 0 65 -86  content: Task14 says: 0
addr: 0 -128 0 65 -85  content: Task33 says: 0
addr: 0 -128 0 65 -84  content: Task6 says: 0
addr: 0 -128 0 65 -83  content: Task23 says: 0
addr: 0 -128 0 65 -82  content: Task17 says: 0
addr: 0 -128 0 65 -81  content: Task18 says: 0
addr: 0 -128 0 65 -80  content: Task4 says: 0
addr: 0 -128 0 65 -79  content: Task8 says: 0
addr: 0 -128 0 65 -78  content: Task11 says: 0
addr: 0 -128 0 65 -77  content: Task34 says: 0
addr: 0 -128 0 65 -76  content: Task36 says: 0
addr: 0 -128 0 65 -75  content: Task2 says: 0
addr: 0 -128 0 65 -74  content: Task12 says: 0
addr: 0 -128 0 65 -73  content: Task25 says: 0
addr: 0 -128 0 65 -72  content: Task19 says: 0
addr: 0 -128 0 65 -71  content: Task30 says: 0
addr: 0 -128 0 65 -70  content: Task20 says: 0
addr: 0 -128 0 65 -69  content: Task1 says: 0
addr: 0 -128 0 65 -68  content: Task13 says: 0
addr: 0 -128 0 65 -67  content: Task26 says: 0
addr: 0 -128 0 65 -66  content: Task7 says: 0
addr: 0 -128 0 65 -65  content: Task38 says: 0
addr: 0 -128 0 65 -64  content: Task15 says: 0
addr: 0 -128 0 65 -63  content: Task22 says: 0
addr: 0 -128 0 65 -62  content: Task10 says: 0
addr: 0 -128 0 65 -61  content: Task35 says: 0
addr: 0 -128 0 65 -59  content: Task27 says: 0
addr: 0 -128 0 65 -60  content: Task3 says: 0
addr: 0 -128 0 65 -58  content: Task40 says: 0
addr: 0 -128 0 65 -57  content: Task32 says: 0
addr: 0 -128 0 65 -56  content: Task16 says: 0
addr: 0 -128 0 65 -55  content: Task24 says: 0
addr: 0 -128 0 65 -54  content: Task0 says: 0
addr: 0 -128 0 65 -53  content: Task9 says: 0
addr: 0 -128 0 65 -52  content: Task37 says: 0
addr: 0 -128 0 65 -51  content: Task5 says: 0
addr: 0 -128 0 65 -50  content: Task39 says: 0
addr: 0 -128 0 65 -49  content: Task21 says: 0
addr: 0 -128 0 65 -48  content: Task47 says: 0
addr: 0 -128 0 65 -47  content: Task44 says: 0
addr: 0 -128 0 65 -46  content: Task51 says: 0
addr: 0 -128 0 65 -45  content: Task60 says: 0
addr: 0 -128 0 65 -44  content: Task41 says: 0
addr: 0 -128 0 65 -43  content: Task52 says: 0
addr: 0 -128 0 65 -42  content: Task55 says: 0
addr: 0 -128 0 65 -41  content: Task53 says: 0
addr: 0 -128 0 65 -40  content: Task48 says: 0
addr: 0 -128 0 65 -39  content: Task42 says: 0
addr: 0 -128 0 65 -38  content: Task43 says: 0
addr: 0 -128 0 65 -37  content: Task74 says: 0
addr: 0 -128 0 65 -36  content: Task45 says: 0
addr: 0 -128 0 65 -35  content: Task46 says: 0
addr: 0 -128 0 65 -34  content: Task49 says: 0
addr: 0 -128 0 65 -33  content: Task54 says: 0
addr: 0 -128 0 65 -32  content: Task56 says: 0
addr: 0 -128 0 65 -31  content: Task61 says: 0
addr: 0 -128 0 65 -30  content: Task57 says: 0
addr: 0 -128 0 65 -29  content: Task59 says: 0
addr: 0 -128 0 65 -28  content: Task65 says: 0
addr: 0 -128 0 65 -27  content: Task63 says: 0
addr: 0 -128 0 65 -25  content: Task50 says: 0
addr: 0 -128 0 65 -26  content: Task68 says: 0
addr: 0 -128 0 65 -24  content: Task69 says: 0
addr: 0 -128 0 65 -23  content: Task71 says: 0
addr: 0 -128 0 65 -22  content: Task70 says: 0
addr: 0 -128 0 65 -21  content: Task58 says: 0
addr: 0 -128 0 65 -19  content: Task62 says: 0
addr: 0 -128 0 65 -20  content: Task73 says: 0
addr: 0 -128 0 65 -18  content: Task66 says: 0
addr: 0 -128 0 65 -16  content: Task76 says: 0
addr: 0 -128 0 65 -15  content: Task64 says: 0
addr: 0 -128 0 65 -14  content: Task72 says: 0
addr: 0 -128 0 65 -17  content: Task75 says: 0
addr: 0 -128 0 65 -13  content: Task67 says: 0
addr: 0 -128 0 65 -10  content: Task80 says: 0
addr: 0 -128 0 65 -11  content: Task86 says: 0
addr: 0 -128 0 65 -9  content: Task81 says: 0
addr: 0 -128 0 65 -12  content: Task84 says: 0
addr: 0 -128 0 65 -8  content: Task77 says: 0
addr: 0 -128 0 65 -5  content: Task83 says: 0
addr: 0 -128 0 65 -7  content: Task85 says: 0
addr: 0 -128 0 65 -6  content: Task78 says: 0
addr: 0 -128 0 65 -4  content: Task79 says: 0
addr: 0 -128 0 65 -1  content: Task89 says: 0
addr: 0 -128 0 65 -2  content: Task82 says: 0
addr: 0 -128 0 66 0  content: Task87 says: 0
addr: 0 -128 0 65 -3  content: Task88 says: 0
addr: 0 -128 0 66 2  content: Task90 says: 0
addr: 0 -128 0 66 3  content: Task91 says: 0
addr: 0 -128 0 66 5  content: Task97 says: 0
addr: 0 -128 0 66 1  content: Task100 says: 0
addr: 0 -128 0 66 6  content: Task93 says: 0
addr: 0 -128 0 66 4  content: Task92 says: 0
addr: 0 -128 0 66 7  content: Task94 says: 0
addr: 0 -128 0 66 8  content: Task99 says: 0
addr: 0 -128 0 66 10  content: Task103 says: 0
addr: 0 -128 0 66 9  content: Task102 says: 0
addr: 0 -128 0 66 11  content: Task104 says: 0
addr: 0 -128 0 66 13  content: Task95 says: 0
addr: 0 -128 0 66 12  content: Task98 says: 0
addr: 0 -128 0 66 14  content: Task96 says: 0
addr: 0 -128 0 66 15  content: Task106 says: 0
addr: 0 -128 0 66 16  content: Task107 says: 0
addr: 0 -128 0 66 17  content: Task111 says: 0
addr: 0 -128 0 66 18  content: Task105 says: 0
addr: 0 -128 0 66 19  content: Task101 says: 0
addr: 0 -128 0 66 20  content: Task112 says: 0
addr: 0 -128 0 66 21  content: Task110 says: 0
addr: 0 -128 0 66 22  content: Task115 says: 0
addr: 0 -128 0 66 23  content: Task109 says: 0
addr: 0 -128 0 66 24  content: Task113 says: 0
addr: 0 -128 0 66 26  content: Task108 says: 0
addr: 0 -128 0 66 25  content: Task119 says: 0
addr: 0 -128 0 66 28  content: Task117 says: 0
addr: 0 -128 0 66 29  content: Task114 says: 0
addr: 0 -128 0 66 27  content: Task116 says: 0
addr: 0 -128 0 66 30  content: Task118 says: 0
 */

#endif //DO_NOT_YARN_ROUTER_SOCKET_H
