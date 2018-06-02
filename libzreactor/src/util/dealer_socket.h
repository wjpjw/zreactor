//
// Created by jipeng on 6/1/18.
//

#ifndef DO_NOT_YARN_DEALER_SOCKET_H
#define DO_NOT_YARN_DEALER_SOCKET_H

#include "zreactor.h"

/*
    从dealer的视角来看，和router收发消息是很简单的，发content即可，router自然会收到id+content，
    router发给dealer的消息也是id+content，但dealer只会看到content。
    虽然简单，但对它封装还是有意义的——消除隐式耦合比提高易用性更重要。
*/

namespace wjp{
    class message;
    class dealer_socket {
    public:
        inline operator void *() noexcept { return socket_; }
        dealer_socket(zmq::context_t& context);
        void recv_from_router(message& content);
        void send_to_router(message content);
        void connect_tcp(const std::string& ip_port);
        void connect_inproc(const std::string& identifier);
    protected:
        zmq::socket_t socket_;
    };
}


#endif //DO_NOT_YARN_DEALER_SOCKET_H
