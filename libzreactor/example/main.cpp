//
// Created by jipeng on 5/25/18.
//

#include "zapi.h"
#include <iostream>

int main() {
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
    while(true);
    return 0;
}