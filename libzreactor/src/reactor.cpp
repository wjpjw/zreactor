//
// Created by jipeng on 5/30/18.
//

#include "reactor.h"
#include "liaison.h"
#include "consignor.h"

using namespace wjp;

reactor::reactor(const std::string& port, callback cb, int nr_workerthreads, int timeout_millisec, int nr_iothreads)
        :timeout_millisec_(timeout_millisec),
         context_(nr_iothreads),
         port_(port),
         callback_(cb),
         nr_workerthreads_(nr_workerthreads)
{
}

void reactor::start() {
    liaison     liasion_   (context_, port_, callback_, nr_workerthreads_);
    consignor   consignor_ (context_, CONSIGNOR_INPROC_NAME);
    zmq_pollitem_t items[] = {
            {liasion_,   0, ZMQ_POLLIN, 0},
            {consignor_, 0, ZMQ_POLLIN, 0}
    };
    while ( true ) {
        zmq::poll(&items[0], 2, timeout_millisec_);
        if (items[0].revents & ZMQ_POLLIN) {
            liasion_.on_request_msg_arrival();
        }
        if (items[1].revents & ZMQ_POLLIN) {
            consignor_.on_response_msg_arrival();
        }
    }
}

