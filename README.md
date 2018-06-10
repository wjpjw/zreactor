# zreactor

高性能的数据库访问代理服务器。

在保证通过严格的测试之余，我还尽可能地保证代码的精巧优美。


## 可重用的工具类

1. wjp::threadpool（固定大小线程池）

    一个基于条件变量与unique_lock的简洁优雅的线程池实现。
    
    这个线程池功能上是极简的，以至于这个类并没有在项目中直接用到，但它很容易扩展成达到industry strength的各式各样的实用线程池——比如这个项目中的wjp::factory。

2. wjp::message（最优性能、最小接口、高易用性的zmq_msg封装）

    a) 使用值语义，也就是说，可以把wjp::message当基础类型来用。
    
    b) 零次数据拷贝——对于较大的消息，message(void*, size_t)构造函数夺取一块内存的所有权，绝不复制。
    
    c) 消息底层的msg对象会在失去最后一个引用后自动析构。
    
    d) 较小的数据（低于1kb）没必要malloc，完全可以把栈中复制一次，效率反而更高，调用接口也更优雅。
    
    e) message有自己的send/recv方法，zmqcpp官方的封装里把这个功能放到socket里面去了，不合理。让数据流向服务，还是让服务流向数据，在重数据的场景下，显然后者更合理。

    这是我的得意之作，比官方的zmq.hpp里封得好用多了。
    
3. wjp::router_socket（对router类型的socket的封装）

    router_socket支持和多个req_socket进行交互，其中router_socket是异步非阻塞的，req_socket则是同步阻塞的。
    
    router_socket也支持和多个router_socket进行交互，彼此都是异步非阻塞的。
    
    zreactor中的两大组件liaison和consignor都是router_socket的子类。
    
    通常来说组合比继承这种高耦合设计好，但这里用继承可以更明确地传递一个概念——liaison和consignor的本质是具备特殊功能的socket。用户在使用这个类时完全可以用组合不用继承。
    
4. wjp::dealer_socket（对dealer类型的socket的封装）
    
   dealer_socket目前只完成了和router_socket合作的功能。其他的通信模式还暂时用不到，以后有时间再将它完善。

## zreactor的主要组件

1. wjp::reactor（reactor模式的服务器）

    reactor是一个基于zmq::poll的异步非阻塞服务器。
    
    它看起来虽然非常简单，但的确实现了reactor的功能。复杂度被隐藏在两个子组件中了——liaison, consignor——二者各自维护一个socket，放在reactor的poller中进行epoll事件轮询监听。
    
    reactor捕获输入事件后会调用liaison的recv_from_req()方法读取客户端的消息，并压入多线程任务队列，让工作线程去处理。
         
    工作线程处理完后会通过自身绑定的socket用进程内通信渠道把处理完的消息发送给consignor。consignor接收到处理好的消息后再委托liaison寄送给客户端。


2. wjp::liaison（联络人）
     
    liaison负责与客户端的req类型的socket交互。拿到客户请求后，交给factory处理，并不等待消息处理结果。factory异步处理完也只能再通过consignor通知liaison发回给客户端。
     

3. wjp::consignor（交付人）

    consignor负责协调liaison和内部的核心工作组件factory。

        factory到consignor的dealer-router模式中的通信协议是自定义的。
        消息格式如下：
        |-- dealer_addr --|
        |-- client addr --|
        |--   result    --|
       
    其中dealer_addr用不到，client addr和result都转交给liaison即可发回给客户端。    

    
4. wjp::factory（多线程消息处理工厂） 
    
    factory的本质是一个更复杂的threadpool。它由多条流水线pipeline组成，每条流水线对应一个工作线程加一个router类型的socket（专门用来向consignor返回刚处理完的消息）。
    
    工作线程又依赖数据库连接池。
    
    
5. wjp::malfurion（数据库连接池）    
    
    mulfurion是一个简单的数据库连接池，之所以简单，是因为涉及Oracle性能的那个复杂的部分已经用Pro*C实现了。    
    

## 工具链：

zreactor：libzmq, zmqcpp, c++14(gcc5.3+), cmake, boost. 

Oracle高性能接口：C, Pro*C。

客户端zmq消息辅助模块：vs2010, libzmq


## 编译安装

只支持Linux或Mac，在/usr/local/include下准备好zmqcpp头文件，boost头文件；在/usr/local/lib下准备好zeromq的动态链接库即可。

        mkdir build
        cd build
        cmake ..
        make
