//
// Created by jipeng on 5/25/18.
//

#include "zapi.h"

int main() {
    wjp::reactor{"5559", [](){}}.start();
    return 0;
}