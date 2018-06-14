//
// Created by jipeng on 5/31/18.
//

#include "consignor.h"
#include "liaison.h"
#include "util/message.h"

using namespace wjp;

consignor::consignor(zmq::context_t& context, const std::string& identifier, liaison& liaison_)
        :router_socket(context), liaison_(liaison_)
{
    bind_inproc(identifier);
}

/*
    factory到consignor的dealer-router模式中的通信协议是自定义的。
    消息格式如下：
    |-- dealer_addr --|
    |-- client addr --|
    |--   result    --|
    consignor不作应答，只forward。
    这种设计比之前的消息拼接+根据addr长度取消息子序列的算法更可靠也更高效。
 */
void consignor::on_job_is_done()
{
    message client_addr, result;
    client_addr.recv(socket_); // dealer_addr, obsolete in this case
    client_addr.recv(socket_); // client_addr in the 2nd frame
    result.recv(socket_);      // result in the 3rd frame
#ifdef WJP_TEST
    std::cout<<"client addr: "<<client_addr.str()<<" result: "<<result.str()<<std::endl;
#endif
    liaison_.send_to_req(client_addr, result);
}
