//
// Created by jipeng on 5/25/18.
//
#include "wjp.h"

#define WJP_TEST

#ifdef WJP_TEST
#include "timer_.h"
using namespace wjp;
int main() {
    
    timer timer_(1.5, 
          [](timer&){
              std::cout<<"duh";
              return 0;
          },
          nullptr,
          true
    );
    return 0;
}
#else

#include "zreactor.h"

int main() {
    wjp::reactor{"5559", [](wjp::message addr, wjp::message content){
        return wjp::message("Now I read: "+content.str());
    }}.start();
    return 0;
}


#endif
