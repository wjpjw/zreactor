//
// Created by jipeng on 5/28/18.
//

#include "cosignor.h"

SimpleRouterSocket::SimpleRouterSocket(Context& context, int port)
        :socket(context, ZMQ_ROUTER), port(port)
{
    socket.bind("inproc://"+boost::lexical_cast<String>(port));
}

