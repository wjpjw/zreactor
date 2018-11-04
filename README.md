# zreactor


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
