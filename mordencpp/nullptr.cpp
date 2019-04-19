#include <iostream>
using namespace std;

///////////////////////////////////////
#include <vector>
#include <cassert>

// 一直以来，C/C++语言里空指针都是用宏NULL来表示，即 #define NULL 0
// 可以看到，NULL实际上是一个整数，而不是真正的指针，有时候会造成语义混淆（如重载函数时候的参数)
// C++11/14增加了新的关键字nullptr来替代NULL，nullptr明确的表示空指针
// 它可以隐式转化为任意类型的指针，也可以与指针进行比较运算，但决不能转换为非指针类型
// 需要注意的是，nullptr并不是指针，而是一个类型为nullptr_t的编译期常量对象实例
// 只是行为很像指针，所以可以使用nullptr_t任意定义与nullptr等价的空指针常量

void case1() {
    int* p1 = nullptr;
    vector<int>* p2 = nullptr;

    assert(!p1 && !p2);
    assert(p1 == nullptr);

    nullptr_t nil;
    double* p3 = nil;

    assert(nil == nullptr);
    assert(!p3);
}

///////////////////////////////////////

int main(void) {
    std::cout << "hello C++11/14 nullptr" << std::endl;

    case1();

    return 0;    
}