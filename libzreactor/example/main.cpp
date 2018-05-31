//
// Created by jipeng on 5/25/18.
//

#include "zapi.h"
#include <iostream>

int main()
{
    wjp::message msg;
    std::string tmp("duh");
    memcpy(msg.data(), tmp.data(), tmp.size());
    std::cout<<msg.str()<<" "<<msg.size();
    return 0;
}