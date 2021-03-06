//
// Created by jipeng on 5/25/18.
//

#pragma once

/* almost commonly used headers */
#include <zmq.hpp>
#include <iostream>
#include <memory>
#include <functional>
#include <exception>

#define CONSIGNOR_INPROC_NAME "csgnr"  //in-process inter-thread communication identifier
//#define WJP_TEST

namespace wjp{
using timer_callback=std::function<void(void)>;
class message;
using callback=std::function<message(message,message)>;  // addr, content --> response

}