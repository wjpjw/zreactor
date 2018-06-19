/*
    wjputils是用来测试、实现独立无环境依赖的算法或数据结构的临时项目。
    仅提供源码供其他项目复用。
*/

#include "skiplist.h"
#include <iostream>

int main()
{
    using namespace wjp;
    std::cout << "skiplist test now starts..." << std::endl;
    //给一个最大的整型值
    SkipList<int, int> skipList(0x7fffffff); //2^31-1
    int length = 10;
    for (int i = 1; i <= length; ++i) {
        skipList.insert(i, i + 200);
    }
    std::cout << "The number of elements in skiplist is:" << skipList.size() << std::endl;
    if (skipList.size() != length) {
        std::cout << "insert failur." << std::endl;
    } else {
        std::cout << "insert success." << std::endl;
    }
    //测试查找
    int value = -1;
    int key = 9;
    Node<int, int> *searchResult = skipList.search(key);
    if (searchResult != nullptr) {
        value = searchResult->getValue();
        std::cout << "search result for key " << key << ":" << value << std::endl;
    } else {
        std::cout << "search failure for key " << key << std::endl;
    }
    //重置value
    value = -1;
    //测试移除,测试不通过
    key = 6;
    std::cout<<std::endl<<"start to remove"<<std::endl;
    bool removeResult = skipList.remove(key, value);
    if (removeResult) {
        std::cout << "removed node whose key is " << key << " and value is " << value << std::endl;
    } else {
        std::cout << "removed failure" << std::endl;
    }
    return 0;
}