//
// Created by jipeng on 5/25/18.
//

#include "zapi.h"

int main() {
    wjp::reactor{"5559", [](wjp::message addr, wjp::message content){
        return wjp::message("Now I read: "+content.str());
    }}.start();
    return 0;
}